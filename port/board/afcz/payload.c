/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015  Piotr Miedzik  <P.Miedzik@gsi.de>
 *   Copyright (C) 2015-2016  Henrique Silva <henrique.silva@lnls.br>
 *   Copyright (C) 2021  Krzysztof Macias <krzysztof.macias@creotech.pl>
 *   Copyright (C) 2021  Wojciech Ruclo <wojciech.ruclo@creotech.pl>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#include "stopwatch.h"

/* Project Includes */
#include "port.h"
#include "payload.h"
#include "ipmi.h"
#include "task_priorities.h"
#include "hotswap.h"
#include "utils.h"
#include "fru.h"
#include "led.h"
#include "i2c_mapping.h"
#include "board_led.h"
#include "tca9539.h"

#ifdef MODULE_BOARD_CONFIG
#include "board_config.h"
#endif


TaskHandle_t vTaskPayload_Handle;
TaskHandle_t vTaskPayloadStatus_Handle;

#define PG_STATUS_UPDATE_RATE 200

bool power_enabled = false;

/* payload states
 *   0 - no power
 *   1 - power switching on
 *       Power Up sequence
 *
 *   2 - power good wait
 *       Since power supply switching
 *       Until detect power good
 *
 *   3 - power good
 *       Here you can configure devices such as clock crossbar and others
 *       We have to reset pin state program b
 *
 *   4 - fpga booting
 *       Since DCDC converters initialization
 *       Until FPGA DONE signal
 *       about 30 sec
 *
 *   5 - fpga working
 *
 *   6 - power switching off
 *       Power-off sequence
 *
 *   7 - power QUIESCED
 *       It continues until a power outage on the line 12v
 *       or for 30 seconds (???)
 *
 * 255 - power fail
 */

const external_gpio_t ext_gpios[16] = {
        [EXT_GPIO_EN_FMC2_P12V]    = { 1, 7 },
        [EXT_GPIO_EN_RTM_PWR]      = { 1, 6 },
        [EXT_GPIO_EN_FMC1_P12V]    = { 1, 5 },
        [EXT_GPIO_MMC_FPGA_PROG_B] = { 1, 4 },
        [EXT_GPIO_EN_1V2]          = { 1, 3 },
        [EXT_GPIO_EN_VCCINT]       = { 1, 2 },
        [EXT_GPIO_EN_RTM_MP]       = { 1, 1 },
        [EXT_GPIO_MMC_FPGA_INIT_B] = { 1, 0 },
        [EXT_GPIO_EN_1V8]          = { 0, 7 },
        [EXT_GPIO_EN_P5V0]         = { 0, 6 },
        [EXT_GPIO_EN_MGT]          = { 0, 5 },
        [EXT_GPIO_EN_3V3]          = { 0, 4 },
        [EXT_GPIO_EN_1V2_MGT]      = { 0, 3 },
        [EXT_GPIO_FPGA_MUX_RESET]  = { 0, 2 },
        [EXT_GPIO_FPGA_RESETn]     = { 0, 1 },
        [EXT_GPIO_PS_POR_B]        = { 0, 0 }
};

#define ext_gpio_set_pin( gpio, state )    tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[gpio].port_num, ext_gpios[gpio].pin_num, state);



//============= TODO Remove after testing prototype
uint8_t payload_continue = 0;

void suspendPayload(char* msg_return)
{
  while(1)
  {
    if(payload_continue == 1)
    {
      printf("%s\n", msg_return);
      payload_continue = 0;
      return;
    }
  }
}
//============= TODO Remove END

/**
 * @brief Reset FPGA
 *
 */

static void fpga_soft_reset(void)
{
    // gpio_set_pin_low(PIN_PORT(GPIO_FPGA_RESETn), PIN_NUMBER(GPIO_FPGA_RESETn));
    ext_gpio_set_pin(EXT_GPIO_FPGA_RESETn, false);
    asm("NOP");
    // gpio_set_pin_high(PIN_PORT(GPIO_FPGA_RESETn), PIN_NUMBER(GPIO_FPGA_RESETn));
    ext_gpio_set_pin(EXT_GPIO_FPGA_RESETn, true);

    /* Blink RED LED to indicate to the user that the Reset was performed */
    LEDUpdate(FRU_AMC, LED1, LEDMODE_LAMPTEST, LEDINIT_ON, 5, 0);
}

static void check_fpga_reset(void)
{
    static TickType_t edge_time;
    static uint8_t reset_lock;
    static uint8_t last_state = 1;

    TickType_t diff;
    TickType_t cur_time = xTaskGetTickCount();

    uint8_t cur_state = gpio_read_pin(PIN_PORT(GPIO_FRONT_BUTTON), PIN_NUMBER(GPIO_FRONT_BUTTON));

    if ((cur_state == 0) && (last_state == 1)) {
        /* Detects the falling edge of the front panel button */
        edge_time = cur_time;
        reset_lock = 0;
    }

    diff = getTickDifference(cur_time, edge_time);

    if ((diff > pdMS_TO_TICKS(2000)) && (reset_lock == 0) && (cur_state == 0)) {
        fpga_soft_reset();

        /* If the user continues to press the button after the 2s, prevent this action to be repeated */
        reset_lock = 1;
    }

    last_state = cur_state;
}

/**
 * @brief Set AFCZ's DCDC Converters state
 *
 */

/*
static bool set_exar_ON()
{
	uint8_t exar1_programmed = 0;
	uint8_t exar2_programmed = 0;
	uint16_t exar1_PowerGood = 0;
	uint16_t exar2_PowerGood = 0;

	// Make sure that power supply is turned OFF
	set_exar_OFF(false);

	//
	// EXAR1
	//

	// Enable EXAR
    tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_VCCINT].port_num, ext_gpios[EXT_GPIO_EN_VCCINT].pin_num, true);

    // Turn EXAR off in case of FLASH configuration detection
    xr77129_read_value(&exar_data[EXAR1], XR77129_PWR_GET_STATUS, &exar1_PowerGood);
    if (exar1_PowerGood & 0xFF) {
        printf("Detected FLASH configuration. Turning off EXAR 1...\n");
        tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_VCCINT].port_num, ext_gpios[EXT_GPIO_EN_VCCINT].pin_num, false);
        set_exar_OFF(false);
        return false;
    }

    // Load EXAR configuration
    printf("Loading EXAR 1 configuration...\n");
	exar1_programmed = xr77129_runtime_load(&exar_data[EXAR1], xr7724_afcz_exar1_runtime_p5_cfg, 430);
	if (!exar1_programmed) {
	    printf("Failed to program EXAR 1.\n");
	}

	StopWatch_DelayMs(600);
	xr77129_read_value(&exar_data[EXAR1], XR77129_PWR_GET_STATUS, &exar1_PowerGood);

    //
    // EXAR2
    //

    if (exar1_programmed && (exar1_PowerGood == 0x1F00)) {

        // Enable EXAR
		tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_P5V0].port_num, ext_gpios[EXT_GPIO_EN_P5V0].pin_num, true);

		// Turn EXAR off in case of FLASH configuration detection
		xr77129_read_value(&exar_data[EXAR2], XR77129_PWR_GET_STATUS, &exar2_PowerGood);
		if (exar2_PowerGood & 0xFF) {
		    tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_P5V0].port_num, ext_gpios[EXT_GPIO_EN_P5V0].pin_num, false);
		    printf("Detected FLASH configuration. Turning off EXAR 2...\n");
	        set_exar_OFF(false);
	        return false;
		}

		// Load EXAR configuration
		printf("Loading EXAR 2 configuration...\n");
		exar2_programmed = xr77129_runtime_load(&exar_data[EXAR2], xr7724_afcz_exar2_runtime_p4_cfg, 431);
	    if (!exar1_programmed) {
	        printf("Failed to program EXAR 2.\n");
	    }

		StopWatch_DelayMs(800);
		xr77129_read_value(&exar_data[EXAR2], XR77129_PWR_GET_STATUS, &exar2_PowerGood);
	}

	// Turn power supply OFF if errors are detected
    if (!exar1_programmed || (exar1_PowerGood != 0x1F00) || !exar2_programmed || (exar2_PowerGood != 0x1F00)) {
        printf("EXAR1_PROG: %d, EXAR1_PG: %d, EXAR2_PROG: %d, EXAR2_PG: %d\n", exar1_programmed, exar1_PowerGood, exar2_programmed, exar2_PowerGood);
        set_exar_OFF(true);
        return false;
    }

	tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_FMC1_P12V].port_num, ext_gpios[EXT_GPIO_EN_FMC1_P12V].pin_num, true);
	StopWatch_DelayMs(100);
	tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[EXT_GPIO_EN_FMC2_P12V].port_num, ext_gpios[EXT_GPIO_EN_FMC2_P12V].pin_num, true);

	return true;
}
*/


/**
 * @brief Set AFCKU's DCDC Converters state
 *
 * @param on DCDCs state
 *
 */

uint8_t enablePowerSupply(bool enable)
{
    uint8_t power_pins[] = {
      EXT_GPIO_EN_VCCINT,
      EXT_GPIO_EN_1V2,
      EXT_GPIO_EN_1V8,
      EXT_GPIO_EN_1V2_MGT,
      EXT_GPIO_EN_MGT,
      EXT_GPIO_EN_3V3,
      EXT_GPIO_EN_P5V0,
      EXT_GPIO_EN_FMC1_P12V,
      EXT_GPIO_EN_FMC2_P12V,
      EXT_GPIO_EN_RTM_MP,
      EXT_GPIO_EN_RTM_PWR
    };

    uint8_t pin;
    if (enable) 
    {
      printf("Enable Power\n");

      for (uint8_t i = 0; i < (sizeof(power_pins) / sizeof(power_pins[0])); i++) 
      {
        pin = power_pins[i];

        //============= TODO Remove after testing prototype
        printf("Send \'continue\' to enable power_pins[%d]\n\r", i);
        suspendPayload("OK");
        //============= TODO Remove END

        // tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[pin].port_num, ext_gpios[pin].pin_num, true);
        ext_gpio_set_pin(pin, true);
        vTaskDelay(50);
      }
    } 
    else 
    {
      printf("Disable Power\n");

      for (uint8_t i = (sizeof(power_pins) / sizeof(power_pins[0])); i > 0; i--) 
      {
        pin = power_pins[i];
        // tca9539_output_pin_set(CHIP_ID_TCA9539_PM, ext_gpios[pin].port_num, ext_gpios[pin].pin_num, false);
        ext_gpio_set_pin(pin, false);
        
        vTaskDelay(10);
      }
    }
    return 1;
}


EventGroupHandle_t amc_payload_evt = NULL;
#ifdef MODULE_RTM
EventGroupHandle_t rtm_payload_evt = NULL;
#endif

void payload_send_message(uint8_t fru_id, EventBits_t msg)
{
    if ((fru_id == FRU_AMC) && amc_payload_evt) {
        xEventGroupSetBits(amc_payload_evt, msg);
#ifdef MODULE_RTM
    } else if ( (fru_id == FRU_RTM) && rtm_payload_evt ) {
        xEventGroupSetBits( rtm_payload_evt, msg );
#endif
    }
}

void payload_init(void)
{
    if (!bench_test) {
        /* Wait until ENABLE# signal is asserted ( ENABLE == 0) */
        while ( gpio_read_pin( PIN_PORT(GPIO_MMC_ENABLE), PIN_NUMBER(GPIO_MMC_ENABLE) ) == 1) {};
    }

    /* PM GPIO expander initialization */
    tca9539_output_port_set(CHIP_ID_TCA9539_PM, 0, 0);
    tca9539_output_port_set(CHIP_ID_TCA9539_PM, 1, 0);
    tca9539_port_dir_set(CHIP_ID_TCA9539_PM, 0, 0);
    tca9539_port_dir_set(CHIP_ID_TCA9539_PM, 1, 0);

    xTaskCreate(vTaskPayload, "Payload", 240, NULL, tskPAYLOAD_PRIORITY, &vTaskPayload_Handle);

    amc_payload_evt = xEventGroupCreate();
#ifdef MODULE_RTM
    rtm_payload_evt = xEventGroupCreate();
#endif

    gpio_set_pin_state(PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), GPIO_LEVEL_LOW);
    // gpio_set_pin_state(PIN_PORT(GPIO_FPGA_PROGRAM_B), PIN_NUMBER(GPIO_FPGA_PROGRAM_B), GPIO_LEVEL_LOW);
    ext_gpio_set_pin(GPIO_FPGA_PROGRAM_B, false);

    // Create power good monitoring task
    xTaskCreate(vTaskPayloadStatus, "PayloadStatus", 256, NULL, tskXR77129_PRIORITY, &vTaskPayloadStatus_Handle);
}

void vTaskPayload(void *pvParameters)
{
    uint8_t state = PAYLOAD_NO_POWER;
    uint8_t new_state = -1;

    /* Payload power good flag */
    uint8_t PP_good = 0;

    /* Payload DCDCs good flag */
    uint8_t DCDC_good = 0;

    uint8_t QUIESCED_req = 0;
    EventBits_t current_evt;

    extern sensor_t * hotswap_amc_sensor;

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    /* Wait for first DCDC Power Good readout */
    do {
        current_evt = xEventGroupGetBits(amc_payload_evt);
        vTaskDelay(PAYLOAD_BASE_DELAY);
    } while (!(current_evt & (PAYLOAD_MESSAGE_DCDC_PGOOD | PAYLOAD_MESSAGE_DCDC_PGOODn)));

    for (;;) {

        check_fpga_reset();

        new_state = state;

        current_evt = xEventGroupGetBits(amc_payload_evt);

        if (current_evt & PAYLOAD_MESSAGE_PPGOOD) {
            PP_good = 1;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_PPGOOD);
        }

        if (current_evt & PAYLOAD_MESSAGE_PPGOODn) {
            PP_good = 0;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_PPGOODn);
        }

        if (current_evt & PAYLOAD_MESSAGE_DCDC_PGOOD) {
            DCDC_good = 1;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_DCDC_PGOOD);
        }

        if (current_evt & PAYLOAD_MESSAGE_DCDC_PGOODn) {
            DCDC_good = 0;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_DCDC_PGOODn);
        }

        if (current_evt & PAYLOAD_MESSAGE_QUIESCE) {
            QUIESCED_req = 1;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_QUIESCE);
        }

        if (current_evt & PAYLOAD_MESSAGE_COLD_RST) {
            state = PAYLOAD_SWITCHING_OFF;
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_COLD_RST);
        }

        if (current_evt & PAYLOAD_MESSAGE_REBOOT) {
            ext_gpio_set_pin(EXT_GPIO_FPGA_RESETn, false);
            asm("NOP");
            ext_gpio_set_pin(EXT_GPIO_FPGA_RESETn, true);
            xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_REBOOT);
        }

        PP_good = gpio_read_pin(PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK));

        switch (state) {

        case PAYLOAD_NO_POWER:

            if (PP_good) {
                new_state = PAYLOAD_POWER_GOOD_WAIT;
            }
            QUIESCED_req = 0;
            break;

        case PAYLOAD_POWER_GOOD_WAIT:

            if (!DCDC_good) {
                /* Turn DCDC converters ON */
                enablePowerSupply(true);
            }

            vTaskDelay(100);
            power_enabled = true;

            /* Clear hotswap sensor backend power failure bits */
            hotswap_clear_mask_bit( HOTSWAP_AMC, HOTSWAP_BACKEND_PWR_SHUTDOWN_MASK );
            hotswap_clear_mask_bit( HOTSWAP_AMC, HOTSWAP_BACKEND_PWR_FAILURE_MASK );

            new_state = PAYLOAD_STATE_FPGA_SETUP;
            break;

        case PAYLOAD_STATE_FPGA_SETUP:

            if (QUIESCED_req || (PP_good == 0)) {
                new_state = PAYLOAD_SWITCHING_OFF;
            } else if (DCDC_good == 1) {

                new_state = PAYLOAD_FPGA_ON;

                ext_gpio_set_pin(GPIO_FPGA_RESETn, true);
                ext_gpio_set_pin(GPIO_FPGA_PROGRAM_B, false);
                vTaskDelay(10);
                ext_gpio_set_pin(GPIO_FPGA_PROGRAM_B, true);

#ifdef MODULE_RTM
                payload_send_message(FRU_RTM, PAYLOAD_MESSAGE_RTM_READY);
#endif

#ifdef MODULE_BOARD_CONFIG
                board_config();
#endif
            }
            break;

        case PAYLOAD_FPGA_ON:

          if (QUIESCED_req == 1 || PP_good == 0 || DCDC_good == 0) {
            new_state = PAYLOAD_SWITCHING_OFF;
          }
          break;

        case PAYLOAD_SWITCHING_OFF:

#ifdef MODULE_RTM
            rtm_quiesce();
            rtm_disable_payload_power();
#endif

            ext_gpio_set_pin(GPIO_FPGA_RESETn, false);
            gpio_set_pin_state(PIN_PORT(GPIO_PHY_RESETn), PIN_NUMBER(GPIO_PHY_RESETn), false);

            /* Turn DCDC converters OFF */
            power_enabled = false;
            enablePowerSupply(false);

            if (QUIESCED_req) {
                hotswap_set_mask_bit(HOTSWAP_AMC, HOTSWAP_QUIESCED_MASK);
                if (hotswap_send_event(hotswap_amc_sensor, HOTSWAP_STATE_QUIESCED) == ipmb_error_success) {
                    QUIESCED_req = 0;
                    hotswap_clear_mask_bit(HOTSWAP_AMC, HOTSWAP_QUIESCED_MASK);
                    new_state = PAYLOAD_QUIESCED;
                }
            } else {
                new_state = PAYLOAD_QUIESCED;
            }

            /* Reset the power good flags to avoid the state machine to start over without a new read from the sensors */
            break;

        case PAYLOAD_QUIESCED:
            if (PP_good == 0) {
                new_state = PAYLOAD_NO_POWER;
            }
            break;

        default:
            break;
        }

        state = new_state;
        vTaskDelayUntil(&xLastWakeTime, PAYLOAD_BASE_DELAY);
    }
}

void vTaskPayloadStatus(void *Parameters)
{
    const uint8_t POWER_GOOD = 0x03;

    /* Power Good flags */
    enum
    {
        DCDC_PG,
        SDRAM_PG,
        PG_CNT
    };

    const char *power_good_str[] = { "DCDC_PGOOD", "SDRAM" };
    bool power_supply_msg = true;

    uint8_t PG_flag = 0;

    static uint8_t old_flag = 0xFF;

    TickType_t xLastWakeTime;
    /* Task will run every 100ms */
    const TickType_t xFrequency = PG_STATUS_UPDATE_RATE / portTICK_PERIOD_MS;

    /* Initialize the xLastWakeTime variable with the current time. */
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {

        /* Payload power good flag */
        if (gpio_read_pin(PIN_PORT(GPIO_P12V0_OK), PIN_NUMBER(GPIO_P12V0_OK))) {

            if (gpio_read_pin(PIN_PORT(GPIO_PGOOD), PIN_NUMBER(GPIO_PGOOD))) {
                PG_flag |= 1 << DCDC_PG;
            } else {
                PG_flag &= ~(1 << DCDC_PG);
            }

            if (gpio_read_pin(PIN_PORT(GPIO_SDRAM_PGOOD), PIN_NUMBER(GPIO_SDRAM_PGOOD))) {
                PG_flag |= 1 << SDRAM_PG;
            } else {
                PG_flag &= ~(1 << SDRAM_PG);
            }
        }

        if (PG_flag ^ old_flag) {
            if (PG_flag == POWER_GOOD) {
                /* Let Payload Task know that Power is OK */
                xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_DCDC_PGOODn);
                payload_send_message(FRU_AMC, PAYLOAD_MESSAGE_DCDC_PGOOD);
                power_supply_msg = true;
            } else {
                xEventGroupClearBits(amc_payload_evt, PAYLOAD_MESSAGE_DCDC_PGOOD);
                payload_send_message(FRU_AMC, PAYLOAD_MESSAGE_DCDC_PGOODn);
                power_supply_msg = true;
            }
            old_flag = PG_flag;
        }

        /* Display power supply status */
        if (power_supply_msg && power_enabled) {
            if (PG_flag == POWER_GOOD) {
                printf("\nPower supply status - OK.\n");
            } else {
                printf("\nPOWER GOOD error. [");
                uint8_t add_space = 0;
                for (uint8_t i = 0; i < PG_CNT; i++) {
                    if (!((PG_flag >> i) & 0x1)) {
                        if (add_space++) {
                            printf(" %s", power_good_str[i]);
                        }
                        else {
                            printf("%s", power_good_str[i]);
                        }
                    }
                }
                printf("]\n");
            }
            power_supply_msg = false;
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/* HPM Functions */
#ifdef MODULE_HPM

#include "flash_spi.h"
#include "string.h"

uint8_t hpm_page[256];
uint8_t hpm_pg_index;
uint32_t hpm_page_addr;

uint8_t payload_hpm_prepare_comp( void )
{
    /* Initialize variables */
    memset(hpm_page, 0xFF, sizeof(hpm_page));
    hpm_pg_index = 0;
    hpm_page_addr = 0;

    /* Initialize flash */
    ssp_init( FLASH_SPI, FLASH_SPI_BITRATE, FLASH_SPI_FRAME_SIZE, SSP_MASTER, SSP_INTERRUPT );

    /* Prevent the FPGA from accessing the Flash to configure itself now */
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, true)
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, false)
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, true)
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, false)

    /* Erase FLASH */
    flash_bulk_erase();

    return IPMI_CC_COMMAND_IN_PROGRESS;
}

uint8_t payload_hpm_upload_block( uint8_t * block, uint16_t size )
{
    /* TODO: Check DONE pin before accessing the SPI bus, since the FPGA may be reading it in order to boot */
    uint8_t remaining_bytes_start;

    if ( sizeof(hpm_page) - hpm_pg_index > size ) {
        /* Our page is not full yet, just append the new data */
        memcpy(&hpm_page[hpm_pg_index], block, size);
        hpm_pg_index += size;

        return IPMI_CC_OK;

    } else {
        /* Complete the remaining bytes on the buffer */
        memcpy(&hpm_page[hpm_pg_index], block, (sizeof(hpm_page) - hpm_pg_index));
        remaining_bytes_start = (sizeof(hpm_page) - hpm_pg_index);

        /* Program the complete page in the Flash */
        flash_program_page( hpm_page_addr, &hpm_page[0], sizeof(hpm_page));

        hpm_page_addr += sizeof(hpm_page);

        /* Empty our buffer and reset the index */
        memset(hpm_page, 0xFF, sizeof(hpm_page));
        hpm_pg_index = 0;

        /* Save the trailing bytes */
        memcpy(&hpm_page[hpm_pg_index], block+remaining_bytes_start, size-remaining_bytes_start);

        hpm_pg_index = size-remaining_bytes_start;

        return IPMI_CC_COMMAND_IN_PROGRESS;
    }
}

uint8_t payload_hpm_finish_upload( uint32_t image_size )
{
    /* Check if the last page was already programmed */
    if (!hpm_pg_index) {
        /* Program the complete page in the Flash */
        flash_program_page( hpm_page_addr, &hpm_page[0], (sizeof(hpm_page)-hpm_pg_index));
        hpm_pg_index = 0;
        hpm_page_addr = 0;

        return IPMI_CC_COMMAND_IN_PROGRESS;
    }

    return IPMI_CC_OK;
}

uint8_t payload_hpm_get_upgrade_status( void )
{
    if (is_flash_busy()) {
        return IPMI_CC_COMMAND_IN_PROGRESS;
    } else {
        return IPMI_CC_OK;
    }
}

uint8_t payload_hpm_activate_firmware( void )
{
    /* Reset FPGA - Pulse PROGRAM_B pin */
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, false)
    ext_gpio_set_pin(EXT_GPIO_MMC_FPGA_PROG_B, true)

    return IPMI_CC_OK;
}
#endif

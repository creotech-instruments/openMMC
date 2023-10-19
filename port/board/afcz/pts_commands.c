/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

/* Project includes */
#include "port.h"
#include "uart_debug.h"
#include "sdr.h"
#include "i2c.h"
#include "led.h"
#include "fru.h"
#include "pin_mapping.h"
#include "pts_commands.h"
#include "board_led.h"
#include "i2c_mapping.h"
#include "at24mac.h"


void pts_command(char *cmd, char *write_buffer)
{
	SDR_type_01h_t *p_sdr1;
	sensor_t *sensor;
	uint32_t sensor_val;
	uint8_t temp[2] = {0};
	uint8_t i2c_bytes_recv;
	uint8_t i2c_bus = 3;
	uint8_t i2c_interface;
	char buff[4];
	char tmp_buff[32] = {0};
	uint8_t ipmb_addr;
	uint8_t payload_count;
	uint8_t on = 1;
	uint8_t pin_state = 0xff;
	uint8_t eeprom_id[8] = {0};
	uint8_t rtc_sec = 0;
	uint8_t suspended = 0;
	uint8_t led = 1;

	pin_state = gpio_read_pin(2, 8);

	if (strcmp(cmd, "suspend") == 0){
		if (!suspended)
		{
			vTaskSuspendAll();
			suspended = 1;
		}
	}
	else if (strcmp(cmd, "resume") == 0){
		if (suspended)
		{
			xTaskResumeAll();
			suspended = 0;
		}
	}

	else if (strcmp(cmd, "uart") == 0){
		uint8_t uart = 'm';
		itoa(uart, write_buffer, 10);
	}

	else if (strcmp(cmd, "sensors") == 0){
		for (sensor = sdr_head; sensor != NULL ; sensor = sensor -> next)
		{
			if (sensor->sdr_type == 0x1)
			{
				p_sdr1 = (SDR_type_01h_t*) sensor -> sdr;
				if ( p_sdr1 -> sensortype == SENSOR_TYPE_TEMPERATURE )
				{
					sensor_val = ( (uint32_t) sensor->readout_value ) >> 1;
					// if ( sensor->num == 14)
					// 	sensor_val = ( (uint32_t) sensor->readout_value );

				}
				else if ( p_sdr1 -> sensortype == SENSOR_TYPE_VOLTAGE )
					sensor_val = (64 * ( (uint32_t) sensor->readout_value ));
				else
					sensor_val = ( (int8_t) sensor->readout_value );


				if (p_sdr1 -> sensortype == SENSOR_TYPE_CURRENT)
				{
					//uart_send_int ( (int8_t) sensor->readout_value, 10);
				}
				else
				{
					itoa(sensor_val, tmp_buff, 10);
					strcat(write_buffer, tmp_buff);
					strcat(write_buffer, "\r\n");
					memset(tmp_buff,0,strlen(tmp_buff));
				}
			}
		}

	}

	else if (strcmp(cmd, "ipmb") == 0){
		ipmb_addr = get_ipmb_addr();
		itoa(ipmb_addr, write_buffer, 10);
	}

	else if (strcmp(cmd, "hotswap") == 0){
		pin_state = gpio_read_pin(PIN_PORT(GPIO_HOT_SWAP_HANDLE), PIN_NUMBER(GPIO_HOT_SWAP_HANDLE));
		itoa(pin_state, write_buffer, 10);
	}

	else if (strcmp(cmd, "eeprom") == 0){

		at24mac_read_eui(CHIP_ID_EEPROM, eeprom_id, 8, 0);

		for(uint8_t i=0; i<8; i++){
			itoa(eeprom_id[i], tmp_buff, 16);
			strcat(write_buffer, "0x");
			strcat(write_buffer, tmp_buff);
			strcat(write_buffer, "\r\n");
			memset(tmp_buff,0,strlen(tmp_buff));
		}
	}

	else if (strcmp(cmd, "pin_enable") == 0){ //pin enable state
		pin_state = gpio_read_pin(2, 8);
		itoa(pin_state, write_buffer, 10);
	}

	else if (strcmp(cmd, "amc_led_red") == 0){
		LEDUpdate( 0, LED_BLUE, LEDMODE_OVERRIDE, LEDINIT_OFF, 0, 0 );
		LEDUpdate( 0, LED1, LEDMODE_OVERRIDE, LEDINIT_ON, 0xff, 0 );
	}

	else if (strcmp(cmd, "amc_led_blue") == 0){
		LEDUpdate( 0, LED_BLUE, LEDMODE_OVERRIDE, LEDINIT_ON, 0, 0 );
		LEDUpdate( 0, LED1, LEDMODE_OVERRIDE, LEDINIT_OFF, 0xff, 0 );
	}

	else if (strcmp(cmd, "rtm_led_red") == 0){
		LEDUpdate( 1, LED_BLUE, LEDMODE_OVERRIDE, LEDINIT_OFF, 0, 0 );
		LEDUpdate( 1, LED1, LEDMODE_OVERRIDE, LEDINIT_ON, 0xff, 0 );
	}

	else if (strcmp(cmd, "rtm_led_blue") == 0){
		LEDUpdate( 0, LED_BLUE, LEDMODE_OVERRIDE, LEDINIT_ON, 0, 0 );
		LEDUpdate( 0, LED1, LEDMODE_OVERRIDE, LEDINIT_OFF, 0xff, 0 );
	}

	// else if (strcmp(cmd, "rtc") == 0){
	// 	uint8_t r_i2c_bus = 1;
	// 	uint8_t r_i2c_interface = 1;
	// 	uint8_t r_i2c_addr, r_len, r_i2c_bytes_sent, r_i2c_bytes_recv;
	// 	uint8_t r_cmd[2] = {0};

	// 	I2C_XFER_T r_xfer = { 0 };
	// 	r_xfer.slaveAddr = 0x57;
	// 	r_xfer.txBuff = r_cmd;
	// 	r_xfer.txSz = 2;
	// 	r_xfer.rxBuff = &rtc_sec;
	// 	r_xfer.rxSz = 0;

	// 	r_i2c_addr = 0x57;
	// 	r_cmd[0] = 0x00;
	// 	r_cmd[1] = 0x81;

	// 	if( i2c_take_by_busid( r_i2c_bus, &r_i2c_interface, (TickType_t)10) )
	// 	{
	// 		while (Chip_I2C_MasterTransfer(r_i2c_interface, &r_xfer) == I2C_STATUS_ARBLOST) {}
	// 		i2c_give(r_i2c_interface);
	// 	}
	// 	rtc_sec = 0;

	// 	r_xfer.slaveAddr = 0x57;
	// 	r_xfer.txBuff = r_cmd;
	// 	r_xfer.rxBuff = &rtc_sec;
	// 	r_xfer.txSz = 1;
	// 	r_xfer.rxSz = 1;

	// 	vTaskDelay(2000 / portTICK_PERIOD_MS);
	// 	if( i2c_take_by_busid( r_i2c_bus, &r_i2c_interface, (TickType_t)10) )
	// 	{
	// 		while (Chip_I2C_MasterTransfer(r_i2c_interface, &r_xfer) == I2C_STATUS_ARBLOST) {}
	// 		i2c_give(r_i2c_interface);
	// 	}
	// 	rtc_sec = rtc_sec & 0x7F;
	// 	rtc_sec = ( (rtc_sec & 0xF0) >> 4 )*10 + (rtc_sec & 0x0F);
	// 	itoa(rtc_sec, write_buffer, 10);
	// }

	else if (strcmp(cmd, "fmc_io_low") == 0){

		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PRSNT_M2C), PIN_NUMBER(GPIO_FMC1_PRSNT_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_M2C), PIN_NUMBER(GPIO_FMC1_PG_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_C2M), PIN_NUMBER(GPIO_FMC1_PG_C2M), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PRSNT_M2C), PIN_NUMBER(GPIO_FMC2_PRSNT_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_M2C), PIN_NUMBER(GPIO_FMC2_PG_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_C2M), PIN_NUMBER(GPIO_FMC2_PG_C2M), GPIO_DIR_OUTPUT);

		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PRSNT_M2C), PIN_NUMBER(GPIO_FMC1_PRSNT_M2C), GPIO_LEVEL_LOW);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PG_M2C), PIN_NUMBER(GPIO_FMC1_PG_M2C), GPIO_LEVEL_LOW);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PG_C2M), PIN_NUMBER(GPIO_FMC1_PG_C2M), GPIO_LEVEL_LOW);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PRSNT_M2C), PIN_NUMBER(GPIO_FMC2_PRSNT_M2C), GPIO_LEVEL_LOW);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PG_M2C), PIN_NUMBER(GPIO_FMC2_PG_M2C), GPIO_LEVEL_LOW);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PG_C2M), PIN_NUMBER(GPIO_FMC2_PG_C2M), GPIO_LEVEL_LOW);
	}

	else if (strcmp(cmd, "fmc_io_high") == 0){
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PRSNT_M2C), PIN_NUMBER(GPIO_FMC1_PRSNT_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_M2C), PIN_NUMBER(GPIO_FMC1_PG_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_C2M), PIN_NUMBER(GPIO_FMC1_PG_C2M), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PRSNT_M2C), PIN_NUMBER(GPIO_FMC2_PRSNT_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_M2C), PIN_NUMBER(GPIO_FMC2_PG_M2C), GPIO_DIR_OUTPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_C2M), PIN_NUMBER(GPIO_FMC2_PG_C2M), GPIO_DIR_OUTPUT);

		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PRSNT_M2C), PIN_NUMBER(GPIO_FMC1_PRSNT_M2C), GPIO_LEVEL_HIGH);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PG_M2C), PIN_NUMBER(GPIO_FMC1_PG_M2C), GPIO_LEVEL_HIGH);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC1_PG_C2M), PIN_NUMBER(GPIO_FMC1_PG_C2M), GPIO_LEVEL_HIGH);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PRSNT_M2C), PIN_NUMBER(GPIO_FMC2_PRSNT_M2C), GPIO_LEVEL_HIGH);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PG_M2C), PIN_NUMBER(GPIO_FMC2_PG_M2C), GPIO_LEVEL_HIGH);
		gpio_set_pin_state( PIN_PORT(GPIO_FMC2_PG_C2M), PIN_NUMBER(GPIO_FMC2_PG_C2M), GPIO_LEVEL_HIGH);
	}

	else if (strcmp(cmd, "fmc_io_input") == 0){

		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PRSNT_M2C), PIN_NUMBER(GPIO_FMC1_PRSNT_M2C), GPIO_DIR_INPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_M2C), PIN_NUMBER(GPIO_FMC1_PG_M2C), GPIO_DIR_INPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC1_PG_C2M), PIN_NUMBER(GPIO_FMC1_PG_C2M), GPIO_DIR_INPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PRSNT_M2C), PIN_NUMBER(GPIO_FMC2_PRSNT_M2C), GPIO_DIR_INPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_M2C), PIN_NUMBER(GPIO_FMC2_PG_M2C), GPIO_DIR_INPUT);
		gpio_set_pin_dir( PIN_PORT(GPIO_FMC2_PG_C2M), PIN_NUMBER(GPIO_FMC2_PG_C2M), GPIO_DIR_INPUT);
	}

	else if (strcmp(cmd, "pin_done") == 0){
		pin_state = gpio_read_pin(PIN_PORT(GPIO_FPGA_DONE_B), PIN_NUMBER(GPIO_FPGA_DONE_B));
		itoa(pin_state, write_buffer, 10);
	}

	else if (strcmp(cmd, "rtm_enable") == 0){
//		rtm_enable_payload_power();
	}

	else if (strcmp(cmd, "rtm_disable") == 0){
//		rtm_disable_payload_power();
	}

	else if (strcmp(cmd, "rtm_write_fru") == 0){
//		rtm_write_fru();
	}

	else if (strcmp(cmd, "amc_write_fru") == 0){
//		amc_write_fru();
	}
 };

/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2015  Piotr Miedzik  <P.Miedzik@gsi.de>
 *   Copyright (C) 2015-2016  Henrique Silva <henrique.silva@lnls.br>
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
#include "FreeRTOS_CLI.h"
//#include "ctype.h"
//#include "stdlib.h"

/* Project Includes */

#include "port.h"

#include "cli_commands.h"
#include "idt_8v54816.h"
#include "ipmi.h"
#include "task_priorities.h"
#include "payload.h"
#include "hotswap.h"
#include "utils.h"
#include "fru.h"
#include "led.h"
#include "GitSHA1.h"
#include "i2c_mapping.h"

#include "pts_commands.h"


static BaseType_t GpioReadCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    uint8_t port, pin, value;
    BaseType_t param_length;

    // Get GPIO value
    port = atoi(FreeRTOS_CLIGetParameter((const char *) pcCommandString, 1, &param_length));

    // Get GPIO value
    pin = atoi(FreeRTOS_CLIGetParameter((const char *) pcCommandString, 2, &param_length));

    value = gpio_read_pin(port, pin);

    // Write gpio value to the output buffer
    itoa(value, (char *) pcWriteBuffer, 10);

    return pdFALSE;
}

static BaseType_t ReadVersionCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    strcpy(pcWriteBuffer, g_GIT_SHA1);
    return pdFALSE;
}

static BaseType_t ResetCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    NVIC_SystemReset();
    return pdFALSE;
}

void clock_config_to_str(uint8_t *clk_data, char *str_buf) {
    char COLUMN_NAMES[5][14] = {"CHANNEL", "DIRECTION", "TERMINATION", "POLARITY", "SOURCE"};
    char config_str[5][14] = {{0}};

    for (uint8_t i = 0; i < 5; i++) {
        sprintf(COLUMN_NAMES[i], "%-12s", COLUMN_NAMES[i]);
        strcat(str_buf, COLUMN_NAMES[i]);
        strcat(str_buf, "|");
    }
    strcat(str_buf, "\r\n");
    for ( uint8_t chan=0; chan < 16; chan++ ) {

        memset(config_str, 0, sizeof(config_str));

        sprintf(config_str[0], "chan%d", chan);

        if (clk_data[chan] & IDT_DIR_OUT) {
            strcpy(config_str[1], "OUT");
        } else {
            strcpy(config_str[1], "IN");
        }

        if (clk_data[chan] & IDT_TERM_ON) {
            strcpy(config_str[2], "ON");
        } else {
            strcpy(config_str[2], "OFF");
        }

        if (clk_data[chan] & IDT_POL_P) {
            strcpy(config_str[3], "NON-INV");
        } else {
            strcpy(config_str[3], "INV");
        }

        if (clk_data[chan] & IDT_DIR_OUT) {
            sprintf(config_str[4], "ch%d", clk_data[chan] & IDT_MASK_SRC);
        }

        for ( uint8_t i=0; i < 5; i++){
            sprintf(config_str[i], "%-12s", config_str[i]);
            strcat(config_str[i], "|");
        }

        for (uint8_t i = 0; i < 6; i++){
            strcat(str_buf, config_str[i]);
        }
        strcat(str_buf, "\r\n");
    }
}

static BaseType_t ClockSwitchCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    char *param = NULL;
    char *parameters[5] = {NULL};
    BaseType_t param_length;

    uint8_t channel;
    uint8_t value;
    uint8_t *rx_data = pvPortMalloc(sizeof *rx_data);
    uint8_t num;
    char **endptr = pvPortMalloc(sizeof **endptr);;

    const char i2c_error[] = "I2C communication error";


    if (!clock_switch_read_reg(rx_data)) {
        strcpy(pcWriteBuffer, i2c_error);
        return pdFALSE;
    }

    param = FreeRTOS_CLIGetParameter((const char *) pcCommandString, 1, &param_length);

    if (!param) {
        clock_config_to_str(rx_data, pcWriteBuffer);
        return pdFALSE;
    }

    if (strstr(param, "ch")) {
        channel = atoi(&param[2]);
    } else {
        strcpy(pcWriteBuffer, "First parameter must be channel number eg. 'ch5' ");
        return pdFALSE;
    }

    if (!(clock_switch_read_single_channel(channel, &value))) {
        strcpy(pcWriteBuffer, i2c_error);
        return pdFALSE;
    }

    for (uint8_t i=1; i < 5; i++){
        parameters[i] = FreeRTOS_CLIGetParameter((const char *) pcCommandString, i+1, &param_length);
        param = parameters[i];

        // Convert parameter name to lower-case
        for (int i = 0; param[i]; i++) {
            param[i] = tolower((int) param[i]);
        }

        if (strstr(param, "save")) {
//            save_clk_config_to_eeprom();
        } else if (strstr(param, "load")) {
//            load_clk_config_from_eeprom();
        }

        if (strstr(param, "ch")) {
            channel = atoi(&param[2]);
        } else if (strstr(param, "in")) {
            value &= ~(IDT_MASK_DIR);
        } else if (strstr(param, "out")) {
            value |= IDT_MASK_DIR;
        } else if (strstr(param, "pol_n")) {
            value &= ~(IDT_MASK_POL);
        } else if (strstr(param, "pol_p")) {
            value |= IDT_MASK_POL;
        } else if (strstr(param, "term_off")) {
            value &= ~(IDT_MASK_TERM);
        } else if (strstr(param, "term_on")) {
            value |= IDT_MASK_TERM;
        } else if (isdigit(param[0] && param_length < 2)) {
            num = (uint8_t) strtol(param, endptr, 10);
            value |= num | IDT_MASK_SRC;
        }
    }

    if (!(clock_switch_set_single_channel(channel, value))) {
        strcpy(pcWriteBuffer, i2c_error);
        return pdFALSE;
    }
    clock_config_to_str(rx_data, pcWriteBuffer);
    return pdFALSE;
}

static BaseType_t PtsCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    char *command = NULL;
    char response[128] = {0};
    BaseType_t param_length;

    // Get command
    command = FreeRTOS_CLIGetParameter((const char *) pcCommandString, 1, &param_length);

    memset(pcWriteBuffer,0,strlen(pcWriteBuffer));

    strcat(pcWriteBuffer, "RSP_START");
    pts_command(command, response);
    strcat(pcWriteBuffer, response);
    strcat(pcWriteBuffer, "RSP_END");

    return pdFALSE;
}

static const CLI_Command_Definition_t GpioReadCommandDefinition = {
        "gpio_read",
        "\r\ngpio_read <port_num> <pin_num>:\r\n Reads <gpio> value\r\n",
        GpioReadCommand,
        2
};

static const CLI_Command_Definition_t ReadVersionCommandDefinition = {
        "version",
        "\r\nversion:\r\n Returns firmware version (GIT SHA)\r\n",
        ReadVersionCommand,
        0
};

static const CLI_Command_Definition_t ResetCommandDefinition = {
        "reset",
        "\r\nreset:\r\n Resets MMC\r\n",
        ResetCommand,
        0
};

static const CLI_Command_Definition_t ClockSwitchCommandDefinition = {
        "clock_switch",
        "\r\nclock_switch <ch..> <in/out> <1..15> <term_on/term_off> <pol_p/pol_n>"
        "\r\n <term> and <pol> parameters are not mandatory\r\n",
        ClockSwitchCommand,
        -1
};

static const CLI_Command_Definition_t PtsCommandDefinition = {
        "pts",
        "\r\npts <command>:\r\n PTS\r\n",
        PtsCommand,
        1
};

/**
 * @brief Registers all the defined CLI commands.
 */
void RegisterCLICommands(void)
{
    // GPIO
    FreeRTOS_CLIRegisterCommand(&GpioReadCommandDefinition);

    FreeRTOS_CLIRegisterCommand(&ReadVersionCommandDefinition);
    FreeRTOS_CLIRegisterCommand(&ResetCommandDefinition);

    FreeRTOS_CLIRegisterCommand(&ClockSwitchCommandDefinition);

    FreeRTOS_CLIRegisterCommand(&PtsCommandDefinition);
}

/*
 *   openMMC -- Open Source modular IPM Controller firmware
 *
 *   Copyright (C) 2021  Krzysztof Macias <krzysztof.macias@creotech.pl>
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

/* Project Includes */
#include "port.h"
#include "cli.h"
#include "idt_8v54816.h"

uint8_t clock_switch_default_config() {
    uint8_t chan[16];

    chan[0] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[1] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[2] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[3] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[4] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[5] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[6] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[7] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[8] =  IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;
    chan[9] =  IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[10] = IDT_DIR_IN  | IDT_TERM_ON  | IDT_POL_P ;
    chan[11] = IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;
    chan[12] = IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;
    chan[13] = IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;
    chan[14] = IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;
    chan[15] = IDT_DIR_OUT | IDT_TERM_OFF | IDT_POL_P | IDT_SRC_CH5;

    return clock_switch_write_reg(chan);
}

void board_init() {
    /* I2C MUX Init */
    gpio_set_pin_state(PIN_PORT(GPIO_I2C_MUX_ADDR1), PIN_NUMBER(GPIO_I2C_MUX_ADDR1), GPIO_LEVEL_LOW);
    gpio_set_pin_state(PIN_PORT(GPIO_I2C_MUX_ADDR2), PIN_NUMBER(GPIO_I2C_MUX_ADDR2), GPIO_LEVEL_LOW);
    gpio_set_pin_state(PIN_PORT(GPIO_I2C_SW_RESETn), PIN_NUMBER(GPIO_I2C_SW_RESETn), GPIO_LEVEL_HIGH);
}

void board_config() {
//	cli_init();
//	clock_switch_default_config();
}

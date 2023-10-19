/*
 * tca9548.c
 *
 *  Created on: 21 lut 2017
 *      Author: kmacias
 */

#include "FreeRTOS.h"

#include "port.h"
#include "tca9548.h"
#include "i2c.h"
#include "i2c_mapping.h"

void tca9548_selPort( uint8_t chip_id, uint8_t num )
{
	uint8_t data;
	data=(1<<num);

    if( i2c_take_by_chipid( chip_id, &i2c_addr, &i2c_id, (TickType_t) 10) ) {
        xI2CMasterWrite(i2c_id, i2c_addr, &data, 1);
        i2c_give(i2c_id);
    }
}

void tca9548_clrPort( uint8_t chip_id )
{
	uint8_t data=0x00;

    if( i2c_take_by_chipid( chip_id, &i2c_addr, &i2c_id, (TickType_t) 10) ) {
        xI2CMasterWrite(i2c_id, i2c_addr, &data, 1);
        i2c_give(i2c_id);
    }
}

uint8_t tca9548_read( uint8_t chip_id )
{
	uint8_t data_rx=0;

    if( i2c_take_by_chipid( chip_id, &i2c_addr, &i2c_id, (TickType_t) 10) ) {
        xI2CMasterRead(i2c_id, i2c_addr, &data_rx, 1);
        i2c_give(i2c_id);
    }

	return data_rx;
}

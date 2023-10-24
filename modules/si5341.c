/*
 * si5341.c
 *
 *  Created on: 15 lut 2017
 *      Author: kmacias
 */

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "port.h"

/* Project includes */
#include "task_priorities.h"
#include "i2c.h"
#include "i2c_mapping.h"
#include "si5341.h"


void si5341_init ( void )
{

}


uint8_t si5341_write_reg ( uint8_t reg, uint8_t val )
{
  uint8_t i2c_interf, i2c_addr;
  uint8_t data[2] = {
    reg,
    val
  };

  if ( i2c_take_by_chipid( CHIP_ID_SI5341, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE ) 
  {
    xI2CMasterWrite( i2c_interf, i2c_addr, data, 2);
    i2c_give( i2c_interf );
    return 0;
  }

  return -1;
}


uint8_t si5341_read_reg ( uint8_t reg )
{
	uint8_t i2c_interf, i2c_addr;
	uint8_t val = 0xFF;

  if ( i2c_take_by_chipid( CHIP_ID_SI5341, &i2c_addr, &i2c_interf, portMAX_DELAY) == pdTRUE )
  {
    xI2CMasterWriteRead( i2c_interf, i2c_addr, reg, &val, 1 );
    i2c_give( i2c_interf );
  }

  return val;
}

/*
 * si5324.h
 *
 *  Created on: 15 lut 2017
 *      Author: kmacias
 */

#ifndef SI5324_H_
#define SI5324_H_

void si5324_init ( void );

uint8_t si5324_write_reg ( uint8_t reg, uint8_t val);
uint8_t si5324_read_reg ( uint8_t reg );

#endif /* MODULES_SI5324_H_ */

/*
 * si5341.h
 *
 *  Created on: 15 lut 2017
 *      Author: kmacias
 */

#ifndef SI5341_H_
#define SI5341_H_

void si5341_init ( void );

uint8_t si5341_write_reg ( uint8_t reg, uint8_t val);
uint8_t si5341_read_reg ( uint8_t reg );

#endif /* MODULES_SI5341_H_ */

/*
 * tca9548.h
 *
 *  Created on: 21 lut 2017
 *      Author: kmacias
 */

#ifndef MODULES_TCA9548_H_
#define MODULES_TCA9548_H_

#include "tca9548.h"

void tca9548_selPort( uint8_t chip_id, uint8_t num );
void tca9548_clrPort( uint8_t chip_id );
uint8_t tca9548_read( uint8_t chip_id );


#endif /* MODULES_TCA9548_H_ */

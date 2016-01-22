/*
 * utils.c
 *
 *   AFCIPMI  --
 *
 *   Copyright (C) 2015  Henrique Silva  <henrique.silva@lnls.br>
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
 */

/*! @file utils.c
  @brief General utilities functions
  @author Henrique Silva
*/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "utils.h"

/*! @brief Calculate the difference between 2 tick values
 * Since he tick counter can overflow, we need to check if the current value is higher than the start time before performing any calculations.
 * The Tick counter is expected to overflow at the portMAX_DELAY value
 * @param current_time Current tick count
 * @param start_time Start tick count
 *
 * @return Tick difference between arguments
 */
TickType_t getTickDifference(TickType_t current_time, TickType_t start_time)
{
    TickType_t result = 0;
    if (current_time < start_time) {
        result = start_time - current_time;
        result = portMAX_DELAY - result;
    } else {
        result = current_time - start_time;
    }
    return result;
}

/* Include chksum calculation function */


/*! @brief Calculate a n-byte message 2's complement checksum.
 * The checksum byte is calculated by perfoming a simple 8bit 2's complement of the sum of all previous bytes.
 * Since we're using a unsigned int to hold the checksum value, we only need to subtract all bytes from it.
 * @param buffer Pointer to the message bytes.
 * @param range How many bytes will be used in the calculation.
 *
 * @return Checksum of the specified bytes of the buffer.
 */
uint8_t calculate_chksum ( uint8_t * buffer, uint8_t range )
{
    configASSERT( buffer != NULL );
    uint8_t chksum = 0;
    uint8_t i;
    for ( i = 0; i < range; i++ ) {
        chksum -= buffer[i];
    }
    return chksum;
}

/* Compare two buffers' size and data
 * Returns 0 if equal, 0xFF if different */
uint8_t cmpBuffs( uint32_t *bufa, uint32_t len_a, uint32_t *bufb, uint32_t len_b )
{
    uint16_t i;
    if (len_a != len_b) {
        return 0xFF;
    }

    for( i = 0; i<len_a; i++ ) {
        if( *bufa != *bufb ) {
            return (0xFF);
        }
        bufa++;
        bufb++;
    }
    return (0);
}

uint8_t isPowerOfTwo( uint8_t x )
{
  return ((x != 0) && !(x & (x - 1)));
}

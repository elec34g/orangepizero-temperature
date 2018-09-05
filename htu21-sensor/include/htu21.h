/* Copyright (c) 2016 Max Asaulov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef INCLUDE_HTU21_H_
#define INCLUDE_HTU21_H_

#include <math.h>
#include <stdint.h>

#define HTU21_I2C_ADDR              0x40
#define	HTU21_TRIGGER_TEMP_HOLD     0xE3
#define HTU21_TRIGGER_HUM_HOLD      0xE5
#define	HTU21_TRIGGER_TEMP_NO_HOLD  0xF3
#define HTU21_TRIGGER_HUM_NO_HOLD   0xF5
#define HTU21_READ_USER_REGISTER    0xE7
#define HTU21_WRITE_USER_REGISTER   0xE6
#define HTU21_SOFT_RESET            0xFE
#define HTU21_COEFFTEMP             -0.15F

static inline uint8_t htu21_crc8(uint8_t const *data, size_t len)
{
	uint8_t crc = 0x00;

    while (len--)
    {
        crc ^= *data++;

        for (size_t i = 0; i < 8; i++) {
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
    }

    return crc;
}

static inline double htu21_get_hum(uint8_t msb, uint8_t lsb)
{
	const unsigned int hum_unprocessed = (msb << 8) | lsb;
	const double hum = -6 + 125 * ( (double) hum_unprocessed) / pow(2,16);
	return hum;
}

static inline double htu21_get_temp(uint8_t msb, uint8_t lsb)
{
	const unsigned int temp_unprocessed = (msb << 8) | lsb;
	const double temp = -46.85 + 175.72 * ( (double) temp_unprocessed) / pow(2,16);
	return temp;
}

static inline double htu21_hum_compensated(double temp, double hum)
{
	return (hum + (25 - temp)* HTU21_COEFFTEMP);
}

#endif /* INCLUDE_HTU21_H_ */

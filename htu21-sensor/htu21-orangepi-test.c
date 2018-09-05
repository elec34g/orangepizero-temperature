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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <htu21.h>

#define PATH_STR                   "/dev/i2c-0"

int main(int argc, char * argv[])
{
	//printf("Starting test...\n");
	int file = open(PATH_STR, O_RDWR);
	if (file < 0) {
		printf("Error opening a bus %s : %s\n", PATH_STR, strerror(errno));
		if (errno == EACCES) {
			printf("Try sudo\n");
		};
		exit(1);
	};
	if (ioctl(file, I2C_SLAVE, HTU21_I2C_ADDR) < 0) {
		printf("Error checing an address %u\n", HTU21_I2C_ADDR);
    		exit(1);
  	}
	uint8_t data = 0;
	uint8_t command = 0;
	command = HTU21_READ_USER_REGISTER;
	if (write(file, &command, sizeof(command)) < (int)sizeof(command)) {
		printf("Error sending a command %u : %s\n", command, strerror(errno));
		exit(1);
	};
	if (read(file,&data,sizeof(data)) < (int)sizeof(data)) {
		printf("Error reading data : %s\n", strerror(errno));
                exit(1);	
	};
//	printf("Get status 0x%02X\n", data);
	command = HTU21_TRIGGER_TEMP_HOLD;
        if (write(file, &command, sizeof(command)) < (int)sizeof(command)) {
                printf("Error sending a command %u : %s\n", command, strerror(errno));
                exit(1);
        };
	uint8_t data_ar[3] = {};
	if (read(file, &data_ar, sizeof(data_ar)) < (int)sizeof(data)) {
		printf("Error reading data : %s\n", strerror(errno));
                exit(1);
	}
//	printf("Get data 0x%2X 0x%2X 0x%2X\n", data_ar[0], data_ar[1], data_ar[2]);
	const double temp = htu21_get_temp( data_ar[0], data_ar[1]);
//	printf("Temp %.2F C\n", temp);
	command = HTU21_TRIGGER_HUM_HOLD;
        if (write(file, &command, sizeof(command)) < (int)sizeof(command)) {
                printf("Error sending a command %u : %s\n", command, strerror(errno));
                exit(1);
        };
        if (read(file, &data_ar, sizeof(data_ar)) < (int)sizeof(data)) {
                printf("Error reading data : %s\n", strerror(errno));
                exit(1);
        }
//	printf("Get data 0x%2X 0x%2X 0x%2X\n", data_ar[0], data_ar[1], data_ar[2]);
    const double hum = htu21_get_hum( data_ar[0], data_ar[1]);
    const double hum_compensated = htu21_hum_compensated(temp, hum);

	if(strcmp(argv[1], "t")==0) printf("%.2F", temp);
	else if(strcmp(argv[1],"h")==0) printf("%.2F%%\n",hum_compensated);
	else printf("Temp %.2F C;Humidity %.2F%%\n %s %s ", temp,hum_compensated ,argv[0],argv[1]);
	close(file);
	return 0;
}

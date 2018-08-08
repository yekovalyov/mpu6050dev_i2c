
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "oled96.h"
#include "mpu6050dev_i2c.h"


int16_t sensorData[7];

#define  OSTR_MAX  40
char ostr[8][OSTR_MAX+1];




int main(int argc, char *argv[])
{
	int  use_oled = 1;
	int  m = mpuInit(0, 0x68);
	if (m) {
		return 1;
	}

	int i=oledInit(1, 0x3c, 0, 0);
	if (i)
	{
		fprintf(stderr, "Warning: Oled initialization failed\n");
		use_oled = 0;
	}

	for(i=0; i<200; ++i){
		int temp10;

		mpuReadAll(&sensorData[0]);

		temp10 = (sensorData[6] + 521)/34 + 350;
		if (use_oled) {
			/*oledFill(0); // fill with black
			*/
			snprintf(&ostr[0][0], OSTR_MAX, "accel: x %04x   ", (int)(uint16_t)sensorData[0]);
			snprintf(&ostr[1][0], OSTR_MAX, "       y %04x   ", (int)(uint16_t)sensorData[1]);
			snprintf(&ostr[2][0], OSTR_MAX, "       z %04x   ", (int)(uint16_t)sensorData[2]);
			snprintf(&ostr[3][0], OSTR_MAX, "gyro:  x %04x   ", (int)(uint16_t)sensorData[3]);
			snprintf(&ostr[4][0], OSTR_MAX, "       y %04x   ", (int)(uint16_t)sensorData[4]);
			snprintf(&ostr[5][0], OSTR_MAX, "       z %04x   ", (int)(uint16_t)sensorData[5]);
			snprintf(&ostr[6][0], OSTR_MAX, "temp: %04x %5i ",
				(int)(uint16_t)sensorData[6], (int)sensorData[6]);
			snprintf(&ostr[7][0], OSTR_MAX, "%6u: %6i C", i, temp10);

			for (int k=0; k<8; ++k) {
				oledWriteString(0,k, &ostr[k][0], FONT_NORMAL);
			}
		}
		printf("%3d: %04x %04x %04x %04x %04x %04x %04x : ",
			i,
			(unsigned int)(uint16_t)sensorData[0],
			(unsigned int)(uint16_t)sensorData[1],
			(unsigned int)(uint16_t)sensorData[2],
			(unsigned int)(uint16_t)sensorData[3],
			(unsigned int)(uint16_t)sensorData[4],
			(unsigned int)(uint16_t)sensorData[5],
			(unsigned int)(uint16_t)sensorData[6]);
		printf("%4i %4i %4i %4i %4i %4i %4i %f\n",
			(int)sensorData[0],
			(int)sensorData[1],
			(int)sensorData[2],
			(int)sensorData[3]/131,
			(int)sensorData[4]/131,
			(int)sensorData[5]/131, /* LSB/degree*/
			(int)sensorData[6],
			temp10/10.0);
	}
	/*
	printf("Press ENTER to quit\n");
	getchar();

	oledShutdown();
	mpuShutdown();
	*/
	return 0;
}

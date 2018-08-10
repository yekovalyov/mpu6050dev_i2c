
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "oled96.h"
#include "mpu6050dev_i2c.h"


void print_stdout(struct mpu_data_t  *data, const int n) {
	int	temp10 = data->regs.temperature/34 + 350;
	printf("%3d: %04x %04x %04x %04x %04x %04x %04x : ",
	n,
		(unsigned int)data->umem[0],
		(unsigned int)data->umem[1],
		(unsigned int)data->umem[2],
		(unsigned int)data->umem[3],
		(unsigned int)data->umem[4],
		(unsigned int)data->umem[5],
		(unsigned int)data->umem[6]);
	printf("%4i %4i %4i %4i %4i %4i %4i %f\n",
		(int)data->mem[0],
		(int)data->mem[1],
		(int)data->mem[2],
		(int)data->mem[3],
		(int)data->mem[4]/131,
		(int)data->mem[5]/131, /* LSB/degree*/
		(int)data->mem[6]/131,
		temp10/10.0);
	fflush(stdout);
}

#define  OSTR_MAX  40
void print_oled96(struct mpu_data_t  *data, const int n) {
	char ostr[8][OSTR_MAX+1];

	int	temp10 = data->regs.temperature/34 + 350;

	/*oledFill(0); // fill with black
	*/
	snprintf(&ostr[0][0], OSTR_MAX, "accel: x %04x   ", (int)(uint16_t)data->regs.accel_x);
	snprintf(&ostr[1][0], OSTR_MAX, "       y %04x   ", (int)(uint16_t)data->regs.accel_y);
	snprintf(&ostr[2][0], OSTR_MAX, "       z %04x   ", (int)(uint16_t)data->regs.accel_z);
	snprintf(&ostr[3][0], OSTR_MAX, "gyro:  x %04x   ", (int)(uint16_t)data->regs.gyro_x);
	snprintf(&ostr[4][0], OSTR_MAX, "       y %04x   ", (int)(uint16_t)data->regs.gyro_y);
	snprintf(&ostr[5][0], OSTR_MAX, "       z %04x   ", (int)(uint16_t)data->regs.gyro_z);
	snprintf(&ostr[6][0], OSTR_MAX, "temp: %04x %5i ",
				(int)(uint16_t)data->regs.temperature, (int)data->regs.temperature);
	snprintf(&ostr[7][0], OSTR_MAX, "%6u: %6i C", n, temp10);
	

	for (int k=0; k<8; ++k) {
		oledWriteString(0,k, &ostr[k][0], FONT_NORMAL);
	}
}



int main(int argc, char *argv[])
{
	struct mpu_data_t  data;
	int  use_oled96 = 1;
	int  m = mpuInit(0, 0x68);
	if (m) {
		return 1;
	}

	int i=oledInit(1, 0x3c, 0, 0);
	if (i)
	{
		fprintf(stderr, "Warning: Oled initialization failed\n");
		use_oled96 = 0;
	}

	for(i=0; i<200; ++i)
	{
		mpuReadAll(&data);

		if (use_oled96) {
			print_oled96(&data, i);
		}
		print_stdout(&data, i);
	}
	/*
	printf("Press ENTER to quit\n");
	getchar();

	oledShutdown();
	mpuShutdown();
	*/
	return 0;
}

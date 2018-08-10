#ifndef MPU6050_USERSPACE_H
#define MPU6050_USERSPACE_H

#include <stdint.h>


struct mpu_data_t {
	union {
		int16_t mem[7];
		uint16_t umem[7];
		struct {
			int16_t  accel_x;
			int16_t  accel_y;
			int16_t  accel_z;
			int16_t  temperature;
			int16_t  gyro_x;
			int16_t  gyro_y;
			int16_t  gyro_z;
		} regs;
	};
};


int mpuInit(const int channel, const unsigned char i2c_address);

int16_t mpuReadWord(const unsigned char addr);

void mpuReadAll(struct mpu_data_t *data);

// Turns off sensor and closes the I2C handle
void mpuShutdown(void);


#endif
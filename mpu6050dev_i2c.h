#ifndef MPU6050_USERSPACE_H
#define MPU6050_USERSPACE_H

#include <stdint.h>

int mpuInit(int iChannel, unsigned char iAddress);

int16_t mpuReadWord(unsigned char addr);

void mpuReadAll(int16_t *sensors);

// Turns off sensor and closes the I2C handle
void mpuShutdown(void);


#endif
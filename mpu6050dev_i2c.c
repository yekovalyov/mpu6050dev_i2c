#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "mpu6050dev_i2c.h"
#include "mpu6050-regs.h"

static int file_i2c = 0;

int  mpuWriteByte(const unsigned char addr, const unsigned char byte);


//
// Opens a file system handle to the I2C device
// Initializes the mpu controller 
// Returns 0 for success, 1 for failure
//
int mpuInit(const int channel, const unsigned char i2c_addr)
{
	char filename[32];
	int rc;
	unsigned char uc[129];

	sprintf(filename, "/dev/i2c-%d", channel);
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		fprintf(stderr, "Failed to open the i2c bus\n");
		file_i2c = 0;
		return 1;
	}

	if (ioctl(file_i2c, I2C_SLAVE, i2c_addr) < 0)
	{
		fprintf(stderr, "Failed to acquire bus access or talk to slave\n");
		file_i2c = 0;
		return 1;
	}

	{
		uc[0] = REG_WHO_AM_I;
		rc = write(file_i2c, uc, 1);
		rc = read(file_i2c, uc, 1);
		if (rc != 1  ||  uc[0] != MPU6050_WHO_AM_I) {
			fprintf(stderr, "Wrong value in Who am I reg  %02x != %02x\n",
				uc[REG_WHO_AM_I], MPU6050_WHO_AM_I);
			//fprintf(stderr, "Diagnostic regs:  %02x %02x  %02x %02x \n",
			//	rc, uc[0], uc[1], uc[2]);
			return 1;
		}
	}

	if (mpuWriteByte(REG_PWR_MGMT_1, 0)) {
		return 1;
	}

	if (mpuWriteByte(REG_INT_PIN_CFG, 0xA0)) {
		return 1;
	}

	if (mpuWriteByte(REG_INT_ENABLE, 1)) {
		return 1;
	}

	return 0;
}

void mpuShutdown()
{
	if (file_i2c != 0)
	{
		close(file_i2c);
		file_i2c = 0;
	}
}

int16_t mpuReadWord(const unsigned char addr)
{
	unsigned char buf[2];
	int rc;
	unsigned int ret = 0xffff;

	buf[0] = addr;
	rc = write(file_i2c, buf, 1);
	if (rc) {
		rc = read(file_i2c, buf, 2);
		ret = buf[0] << 8 | buf[1];
	}
	return ret;
}

unsigned char  mpuReadByte(const unsigned char addr)
{
	unsigned char buf[1] = {addr};
        unsigned char ret = 0xff;
	int rc = write(file_i2c, buf, 1);
	if (rc) {
		rc = read(file_i2c, buf, 1);
		ret = buf[0];
	}
	return ret;
}

int  mpuWriteByte(const unsigned char addr, const unsigned char byte)
{
	unsigned char buf[2] = {addr, byte};
	int rc = write(file_i2c, buf, 2);
	if (rc < 2) {
		return 1;
	}
	return 0;
}

void mpuReadAll(struct mpu_data_t *data)
{
	unsigned char i;
	for(i=0; i<7; ++i) {
		data->mem[i] = mpuReadWord(REG_ACCEL_XOUT_H + i*2);
	}
}




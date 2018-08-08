#!/usr/bin/make -f

ARCH?=arm
CROSS_COMPILE?=/mnt/hd/yk/armbian/build/cache/toolchains/gcc-linaro-7.2.1-2017.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
KERNELDIR = /mnt/hd/yk/armbian/build/cache/sources/linux-mainline/linux-4.17.y
CC = ${CROSS_COMPILE}gcc
REMOTE_HOST?=orangesu

CFLAGS=-c -Wall -O2 -Iexternal/include
LIBS = -Lexternal/lib -loled96
# -lmpu6050 -lm -lpthread

all: libmpu6050dev_i2c.a sample_mpu6050dev_i2c

run: sample_mpu6050dev_i2c
	scp $< ${REMOTE_HOST}:
	ssh ${REMOTE_HOST} time ./$<

sample_mpu6050dev_i2c: sample_mpu6050dev_i2c.o libmpu6050dev_i2c.a external/lib/liboled96.a
	$(CC) $^ $(LIBS) -o $@

sample_mpu6050dev_i2c.o: sample_mpu6050dev_i2c.c mpu6050dev_i2c.h external/include/oled96.h
	$(CC) $(CFLAGS) $<

sample: sample.o
	$(CC) $^ $(LIBS) -o $@

sample.o: sample.c
	$(CC) $(CFLAGS) $<

mpu6050dev_i2c.o: mpu6050dev_i2c.h mpu6050dev_i2c.c


libmpu6050dev_i2c.a: mpu6050dev_i2c.o
	ar -rc $@ $<


external/lib/liboled96.a: build_libs
external/include/oled96.h: build_libs

build_libs:
	$(MAKE) CC=${CC} ARCH=${ARCH} -C external


clean:
	rm -rf *.o *.a *.gch sample_mpu6050dev_i2c
	$(MAKE) -C external clean

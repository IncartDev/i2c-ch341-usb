/*
 * Simple program for testing CH341A GPIO output using sysfs
 *
 * Copyright (c) 2017 Gunar Schorcht (gunar@schorcht.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2.
 *
 * Compile: gcc -o gpio_output gpio_output.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <cinttypes>

#include <sys/ioctl.h>
// #include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define I2C_SLAVE_ADDR   0x50

uint8_t i2c_id_cmd  [] = { 0x0f };  // get ID command 
uint8_t i2c_rd_cmd  [] = { 0xa8 };  // read data command

uint8_t fm24_write[] = {0x0, 0xf1, 0x0a};
uint8_t fm24_write2[] = {0x0, 0xf0, 0x0b};
uint8_t fm24_read[] = {0x0, 0xf1};

uint8_t i2c_id_data [1];            // ID is one byte
uint8_t i2c_rd_data [6];            // data are 6 bytes

// struct i2c_msg i2c_messages[] = 
// {
//     {
//         .addr  = I2C_SLAVE_ADDR,
//         .flags = 0,
//         .buf   = i2c_id_cmd,
//         .len   = sizeof(i2c_id_cmd)        
//     },
//     {
//         .addr  = I2C_SLAVE_ADDR,
//         .flags = I2C_M_RD,
//         .buf   = i2c_id_data,
//         .len   = sizeof(i2c_id_data)
//     },
//     {
//         .addr  = I2C_SLAVE_ADDR,
//         .flags = 0,
//         .buf   = i2c_rd_cmd,
//         .len   = sizeof(i2c_rd_cmd)
//     },
//     {
//         .addr  = I2C_SLAVE_ADDR,
//         .flags = I2C_M_RD,
//         .buf   = i2c_rd_data,
//         .len   = sizeof(i2c_rd_data)
//     }
// };

int main(int argc, char *argv[]) 
{
	// if (argc != 2)
	// {
    // 	fprintf(stderr, "usage: sudo %s <file_name>\n\n", argv[0]);
    // 	fprintf(stderr, "    where <file_name> is the name of a sysfs gpio value file.\n");
    // 	fprintf(stderr, "    example: %s /sys/class/gpio/gpio4/value\n\n", argv[0]);
    // 	return -1;
    // }
    
	int i2c_bus;
	
	if ((i2c_bus = open("/dev/i2c-7", O_RDWR)) == -1) 
	{
		perror("open");
		return -1;
	}

	if (ioctl(i2c_bus, I2C_SLAVE_FORCE, I2C_SLAVE_ADDR) < 0)
	{
		perror("Could not set i2c slave addr");
		return -1;
	}

	if (write(i2c_bus, fm24_write, sizeof(fm24_write)) != sizeof(fm24_write))
	{
		perror("Could not write id command to i2c slave");
		return -1;
	}

	if (write(i2c_bus, fm24_read, sizeof(fm24_read)) != sizeof(fm24_read))
	{
		perror("Could not write id command to i2c slave");
		return -1;
	}

	if (read (i2c_bus, i2c_id_data, sizeof(i2c_id_data)) != sizeof(i2c_id_data))
	{
		perror("Could not write read id from i2c slave");
		return -1;
	}
	printf("Done: %x\n", i2c_id_data[0]);

	// while (1)
	// {
	// 	for (int i = 0; i < 32; i++)
	// 		if (write(i2c_bus, i2c_id_cmd, sizeof(i2c_id_cmd)) != sizeof(i2c_id_cmd))
	// 		{
	// 			perror("Could not write id command to i2c slave");
	// 			return -1;
	// 		}
	// 	for (int i = 0; i < 32; i++)
	// 		if (read (i2c_bus, i2c_id_data, sizeof(i2c_id_data)) != sizeof(i2c_id_data))
	// 		{
	// 			perror("Could not write read id from i2c slave");
	// 			return -1;
	// 		}
	// 	printf("Done: %x\n", i2c_id_data[0]);
	// 	sleep(1);
	// }

	// if (read (i2c_bus, i2c_id_data, sizeof(i2c_id_data)) != sizeof(i2c_id_data))
	// {
	// 	perror("Could not write read id from i2c slave");
	// 	return -1;
	// }

	// if (write(i2c_bus, i2c_rd_cmd, sizeof(i2c_rd_cmd)) != sizeof(i2c_rd_cmd))
	// {
	// 	perror("Could not write read data command to i2c slave");
	// 	return -1;
	// }

	// if (read (i2c_bus, i2c_rd_data, sizeof(i2c_rd_data)) != sizeof(i2c_rd_data))
	// {
	// 	perror("Could not write read data from i2c slave");
	// 	return -1;
	// }

    // int count = 0;
    
	// while (1) 
	// {
	// 	if (write(fd, count % 2 ? "1" : "0", 1) == -1) 
	// 	{
	// 		perror("write");
	// 		return -1;
	// 	}
	// 	count++;
	// 	sleep(1);
	// }

	return 0;
}

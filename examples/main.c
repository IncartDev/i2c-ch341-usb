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

#define I2C_SLAVE_ADDR0   0x3c
#define I2C_SLAVE_ADDR1   0x3d
#define I2C_SLAVE_ADDR2   0x3e
#define I2C_SLAVE_ADDR3   0x3f

uint8_t i2c_id_cmd  [] = { 0x0f };  // get ID command 
uint8_t i2c_rd_cmd  [] = { 0xa8 };  // read data command


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

void send_cmd(int fd, uint8_t cmd, uint8_t data)
{
    uint8_t fm24_write[] = {cmd, data};
    if (write(fd, fm24_write, sizeof(fm24_write)) != sizeof(fm24_write))
    {
        perror("Could not write id command to i2c slave");
        return;
    }
}

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

    int  fd;

    if ((fd = open("/sys/class/gpio/gpio5/value", O_RDWR)) == -1) 
    {
        perror("open");
        return -1;
    }

    if (write(fd, "0", 1) == -1) 
    {
        perror ("write");
        return -1;
    }

    sleep(1);

    if (write(fd, "1", 1) == -1) 
    {
        perror ("write");
        return -1;
    }

	if (ioctl(i2c_bus, I2C_SLAVE_FORCE, I2C_SLAVE_ADDR0) < 0)
	{
		perror("Could not set i2c slave addr");
		return -1;
	}

    while (true)
    {
        uint8_t byte[] = {0x0};
        uint8_t read_data[] = {0xff};
        if (write(i2c_bus, byte, sizeof(byte)) != sizeof(byte))
        {
            perror("Could not write id command to i2c slave");
            return -1;
        }
        if (read (i2c_bus, read_data, sizeof(read_data)) != sizeof(read_data))
        {
            perror("Could not write read id from i2c slave");
            return -1;
        }
        sleep(1);
    }

    // printf("Done: %x\n", read_data[0]);

    send_cmd(i2c_bus, 0x00, 0x21);
    send_cmd(i2c_bus, 0x00, 0x16);
    send_cmd(i2c_bus, 0x00, 0x0c);
    send_cmd(i2c_bus, 0x00, 0x81);
    send_cmd(i2c_bus, 0x00, 0x20);
    send_cmd(i2c_bus, 0x00, 0x04);
    send_cmd(i2c_bus, 0x00, 0x09);

    send_cmd(i2c_bus, 0x40, 0xff);
    send_cmd(i2c_bus, 0x40, 0xff);
    send_cmd(i2c_bus, 0x40, 0xff);

	// if (write(i2c_bus, fm24_read, sizeof(fm24_read)) != sizeof(fm24_read))
	// {
	// 	perror("Could not write id command to i2c slave");
	// 	return -1;
	// }

    //while (true)
    {
        // send_cmd(i2c_bus, 0x0, 0x21);
        
        sleep(1);
    }
	// printf("Done: %x %x\n", read_data[0], read_data[1]);
	return 0;
}

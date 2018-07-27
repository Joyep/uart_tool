/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//#include <hardware/hardware.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#include "log.h"
#define  LOG_TAG  "uart_api.c"
static int log_level = LOG_LEVEL_DEBUG;


#define msleep(x) usleep(x*1000)


int uart_open(const char *dev)
{
	int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		LOG_E("open %s failed", dev);
		return fd;
	}
	return fd;
}


int uart_close(int fd)
{
	if(fd != -1) {
		fd = -1;
		return close(fd);
	}
	LOG_E("uart port not open, skip!");
	return 0;
}


int uart_write(int fd, void *buf, int len)
{
	int res = 0;
	log_buf(__func__, buf, len);
	res = write(fd, buf, len);
	if(res < 0) {
		LOG_E("write data failed(%d)", res);
		return res;
	}
	return res;
}

int uart_read(int fd, void *buf, int len)
{
	int res = 0;
	res = read(fd, buf, len);
	if(res < 0) {
		LOG_E("read data failed(%d)", res);
		if(errno == EAGAIN) {
			return 0;
		}
		return res;
	}
	log_buf(__func__, buf, res);
	return res;
}

speed_t get_baudrate(int baudrate)
{
	switch (baudrate) {
		case 0:
			return B0;
		case 50:
			return B50;
		case 75:
			return B75;
		case 110:
			return B110;
		case 134:
			return B134;
		case 150:
			return B150;
		case 200:
			return B200;
		case 300:
			return B300;
		case 600:
			return B600;
		case 1200:
			return B1200;
		case 1800:
			return B1800;
		case 2400:
			return B2400;
		case 4800:
			return B4800;
		case 9600:
			return B9600;
		case 19200:
			return B19200;
		case 38400:
			return B38400;
		case 57600:
			return B57600;
		case 115200:
			return B115200;
		case 230400:
			return B230400;
		case 460800:
			return B460800;
		case 500000:
			return B500000;
		case 576000:
			return B576000;
		case 921600:
			return B921600;
		case 1000000:
			return B1000000;
		case 1152000:
			return B1152000;
		case 1500000:
			return B1500000;
		case 2000000:
			return B2000000;
		case 2500000:
			return B2500000;
		case 3000000:
			return B3000000;
		case 3500000:
			return B3500000;
		case 4000000:
			return B4000000;
		default:
			return -1;
	}
}


int uart_config(int fd, int databits, int stopbits, int parity, int baudrate)
{
	speed_t speed;
	struct termios Opt;

	if (tcgetattr(fd, &Opt) != 0) {
		LOG_E("uart_set tcgetattr init fail.");
		return -1;
	}

	Opt.c_cflag |= (CLOCAL | CREAD);

	switch (databits) {
		case 7:
			Opt.c_cflag &= ~CSIZE;
			Opt.c_cflag |= CS7;
			break;
		case 8:
			Opt.c_cflag &= ~CSIZE;
			Opt.c_cflag |= CS8;
			break;
		default:
			LOG_E("uart_set Unsupported databits.");
			return -1;
	}

	switch (parity) {
		case 'n':
		case 'N':
			Opt.c_cflag &= ~PARENB;
			Opt.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			Opt.c_cflag |= PARENB;
			Opt.c_cflag |= PARODD;
			Opt.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			Opt.c_cflag |= PARENB;
			Opt.c_cflag &= ~PARODD;
			Opt.c_iflag |= INPCK;
			break;
		case 's':
		case 'S':
			Opt.c_cflag &= ~PARENB;
			Opt.c_cflag &= ~CSTOPB;
			Opt.c_iflag |= INPCK;
			break;
		default:
			LOG_E("uart_set Unsupported parity.");
			return -1;
	}

	switch (stopbits) {
		case 1:
			Opt.c_cflag &= ~CSTOPB;
			break;
		case 2:
			Opt.c_cflag |= CSTOPB;
			break;
		default:
			LOG_E("uart_set Unsupported stopbits.");
			return -1;
	}

	speed = get_baudrate(baudrate);
	if ((int) speed == -1) {
		/* TODO: throw an exception */
		LOG_E("uart_set Invalid baudrate.");
		return -1;
	}
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&Opt, speed);
	cfsetospeed(&Opt, speed);

	Opt.c_cflag |= (CLOCAL | CREAD);

	Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	Opt.c_oflag &= ~OPOST;
	Opt.c_oflag &= ~(ONLCR | OCRNL);

	Opt.c_iflag &= ~(ICRNL | INLCR);
	Opt.c_iflag &= ~(IXON | IXOFF | IXANY);

	tcflush(fd, TCIFLUSH);

	Opt.c_cc[VTIME] = 0;
	Opt.c_cc[VMIN] = 0;

	if (tcsetattr(fd, TCSANOW, &Opt) != 0) {
		LOG_E("uart_set tcgetattr TCSANOW fail.");
		return -1;
	}

	return 0;

}


int uart_write_read(int fd, unsigned char *data, int data_len, unsigned char *read, int read_len)
{
	int ret;
	int mtimes = 50;
	int len;

	//log_buf("write data1", data, data_len);
	ret = uart_write(fd, data, data_len);
	while(mtimes--) {
		msleep(20);
		len = uart_read(fd, read, read_len);
		if(len <= 0) {
			continue;
		}
		return len;
	}

	LOG_E("timeout");
	return -1;
}




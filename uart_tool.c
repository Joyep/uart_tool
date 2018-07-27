#include <stdio.h>

#include "uart_api.h"

#include "log.h"
#define LOG_TAG "uart_test.c"
static int  log_level = LOG_LEVEL_DEBUG;

int main(void)
{
	int fd = uart_open("/dev/ttyS1");

	char _Getcardstatus[12]  = {11,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65};
	char buf2[50];
	int len;

	log_buf("write org", _Getcardstatus+1, _Getcardstatus[0]);

	len = uart_write_read(fd, _Getcardstatus+1, _Getcardstatus[0], buf2, 50);

	log_buf("get card status", buf2, len);

	uart_close(fd);
}

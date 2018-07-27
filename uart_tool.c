#include <stdio.h>

#include "uart_device.h"

#include "log.h"
#define LOG_TAG "uart_test.c"
static int  log_level = LOG_LEVEL_DEBUG;

int main(void)
{

	char _Getcardstatus[12]  = {11,0x65,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x65};
	char buf2[50];
	int len;

	uart_t* uart = uart_open("/dev/ttyS1");
    if(!uart) {
        LOG_E("uart open failed!!");
        return -1;
    }

	log_buf("test log buf", NULL, NULL);
	log_buf("write org", _Getcardstatus+1, _Getcardstatus[0]);

	len = uart->write_read(uart, _Getcardstatus+1, _Getcardstatus[0], buf2, 50);

	log_buf("get card status", buf2, len);

	uart->close(uart);
}

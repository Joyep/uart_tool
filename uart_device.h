#ifndef __UART_API_H__
#define __UART_API_H__

#include <linux/types.h>

typedef struct uart_device uart_t;
struct uart_device {
    int fd;
    int (*close)(uart_t* uart);
    int (*config)(uart_t* uart, int databits, int stopbits, int parity, int baudrate);
    int (*write)(uart_t* uart, void *buf, int len);
    int (*read)(uart_t* uart, void *buf, int len);
    int (*write_read)(uart_t* uart, unsigned char *data, int data_len, unsigned char *read, int read_len);
};

extern uart_t* uart_open(const char *dev);

#endif

#ifndef __UART_API_H__
#define __UART_API_H__

extern int uart_open(const char *id);
extern int uart_close(int fd);
extern int uart_write(int fd, void *buf, int len);
extern int uart_read(int fd, void *buf, int len);
extern int uart_config(int fd, int databits, int stopbits, int parity, int baudrate);
extern int uart_write_read(int fd, unsigned char *data, int data_len, unsigned char *read, int read_len);

#endif

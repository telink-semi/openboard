#ifndef	__SERIAL_PORT_H
#define	__SERIAL_PORT_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define	 BAUD_RATE		115200

typedef void(*UartReciveCallback)(char*,uint32_t);

void serial_port_init(void);
void uart_send_array(char *array,uint32_t length);
void uart_rec_handle(UartReciveCallback cb);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

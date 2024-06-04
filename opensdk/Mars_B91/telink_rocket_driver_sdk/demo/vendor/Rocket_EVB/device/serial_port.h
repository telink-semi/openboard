#ifndef	__SERIAL_PORT_H
#define	__SERIAL_PORT_H
#include "../app_config.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define	 BAUD_RATE			115200
#define  USART_REC_LEN  	(1024*4)

extern volatile unsigned char UART_RX_BUF[USART_REC_LEN];

typedef void (*UartReciveCallback)(unsigned char*,unsigned int);

void serial_port_init(unsigned int baud);
void uart_send_str(unsigned char *array);
void uart_send_array(unsigned char *array,unsigned int length);
void uart_rec_handle(UartReciveCallback cb);
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif



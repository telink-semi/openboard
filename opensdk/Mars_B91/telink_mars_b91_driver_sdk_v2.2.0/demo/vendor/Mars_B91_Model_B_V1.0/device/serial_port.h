#ifndef	__SERIAL_PORT_H
#define	__SERIAL_PORT_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define	 BAUD_RATE		115200
#define 	USART_REC_LEN  				(32 + 1024*4)

extern volatile unsigned char UART_RX_BUF[USART_REC_LEN];

typedef void (*UartReciveCallback)(char*,uint32_t);

void serial_port_init(void);
void uart_send_str(char *array);
void uart_send_array(char *array,uint32_t length);
void uart_rec_handle(UartReciveCallback cb);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

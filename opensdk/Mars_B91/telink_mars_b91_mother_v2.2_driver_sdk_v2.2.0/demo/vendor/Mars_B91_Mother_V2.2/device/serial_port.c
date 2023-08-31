#include "../app_config.h"
#include "serial_port.h"

#define 	USART_REC_LEN  				1024*4

volatile unsigned char UART_RX_BUF[USART_REC_LEN];
volatile unsigned int  g_uart_rec_time = 0;
volatile unsigned int  g_uart_length = 0;
volatile unsigned int  g_uart_last_length = 0;

void serial_port_init()
{
	uint16_t div;
	uint8_t bwpc;
    uart_reset(UART0);
	uart_set_pin(UART0_TX_PIN, UART0_RX_PIN);
	uart_rtx_en(UART0);
	uart_cal_div_and_bwpc(BAUD_RATE, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	//uart_tx_irq_trig_level(UART0, 0);
	uart_rx_irq_trig_level(UART0, 1);
	uart_set_irq_mask(UART0, UART_RX_IRQ_MASK|UART_ERR_IRQ_MASK);
	plic_interrupt_enable(IRQ19_UART0);
}

void uart_send_array(char *array,uint32_t length)
{
	for(uint32_t i = 0; i < length; i++){
		uart_send_byte(UART0, array[i]);
		uart_rtx_pin_tx_trig(UART0);
	}
}

void uart_rec_handle(UartReciveCallback cb){
	if(clock_time_exceed(g_uart_rec_time,10*1000)){ // 10ms
		if(g_uart_length && g_uart_last_length == g_uart_length){ // received
			cb((char*)UART_RX_BUF,g_uart_length);
			g_uart_last_length = 0;
			g_uart_length = 0;
		}
		else g_uart_last_length = g_uart_length;
		g_uart_rec_time = stimer_get_tick();
	}
}

_attribute_ram_code_sec_ void uart0_irq_handler(void)
{
	if(uart_get_irq_status(UART0, UART_RX_ERR)){
		uart_clr_irq_status(UART0,UART_CLR_RX);// it will clear rx_fifo and rx_err_irq ,rx_buff_irq,so it won't enter rx_buff_irq interrupt.
		uart_reset(UART0); 		  //clear hardware pointer
		uart_clr_rx_index(UART0); //clear software pointer
	}
	if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS)){
		if(g_uart_length < USART_REC_LEN){
			UART_RX_BUF[g_uart_length++] = uart_read_byte(UART0);
		}
		else{
			uart_read_byte(UART0);
		}
	}
}

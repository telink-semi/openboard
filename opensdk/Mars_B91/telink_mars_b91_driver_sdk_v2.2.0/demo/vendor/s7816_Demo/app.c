/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "app_config.h"

volatile unsigned int  s7816_irq_cnt=0;

volatile __attribute__((aligned(4))) unsigned char s7816_rx_buff_byte[S7816_RX_BUFF_LEN] ={0};
volatile __attribute__((aligned(4))) unsigned char command[5]={0x00,0x84,0x00,0x00,0x04};//the command serves to get random number.

void user_init()
{
	s7816_set_pin(S7816_RST_PIN,S7816_VCC_PIN,S7816_CLK_PIN,S7816_TRX_PIN);
	s7816_init(S7816_UART_CHN,S7816_4MHZ,F,D);

	core_interrupt_enable();
#if( S7816_UART_CHN == S7816_UART0)
	plic_interrupt_enable(IRQ19_UART0);
#elif(S7816_UART_CHN == S7816_UART1)
	plic_interrupt_enable(IRQ18_UART1);
#endif
	uart_tx_irq_trig_level(S7816_UART_CHN, 0);
	uart_rx_irq_trig_level(S7816_UART_CHN, 1);
	uart_set_irq_mask(S7816_UART_CHN, UART_RX_IRQ_MASK);
	s7816_en(S7816_UART_CHN);//enable the 7816 module
}
void main_loop (void)
{
	/*********************activate and coldReset and set trx pin***************/
	s7816_coldreset();// the coldreset accompanied by IC-CARD activate.
	delay_ms(30);//wait for the return atr.the time is decided by the clock and the atr numbers.
	//s7816_warmreset(); //the warmreset is required after the IC-CARD activate.
   /*******************************TX*****************************/
    for(int i=0;i<5;i++)
    {
    	s7816_send_byte(S7816_UART_CHN,command[i]);
    	//delay_ms(0.5);//extra protect time
    }
    /******************************RX****************************/
}

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
#if(S7816_UART_CHN == S7816_UART0)
void uart0_irq_handler(void)
{
	if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS))
	{
		if(s7816_irq_cnt<=S7816_RX_BUFF_LEN)//if the rx buff is full,it won't receive data.
	    {
			s7816_rx_buff_byte[s7816_irq_cnt] = uart_read_byte(UART0);
			s7816_irq_cnt++;
		}
	}
}


#elif(S7816_UART_CHN == S7816_UART1)
void uart1_irq_handler(void)
{
	if(uart_get_irq_status(UART1, UART_RXBUF_IRQ_STATUS))
	{
		if(s7816_irq_cnt<=S7816_RX_BUFF_LEN)
	    {
			s7816_rx_buff_byte[s7816_irq_cnt] = uart_read_byte(UART1);
			s7816_irq_cnt++;
		}
	}
}
#endif



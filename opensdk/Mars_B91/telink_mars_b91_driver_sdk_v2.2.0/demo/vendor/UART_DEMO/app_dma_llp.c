/********************************************************************************************************
 * @file    app_dma_llp.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/**
   @verbatim
   ===============================================================================
                        ##### how to test demo #####
   ===============================================================================
   (+) DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN
       Hardware connection: the tx of the board<->the rx of serial port tool, the rx of the board<->the tx of serial port tool,the ground <-> the ground;
       Data stream: the serial port sends data to the rx. rx_done is used to check whether the data is received. Then the receive length hardware automatically writes back to the first four bytes of rec_buff and send the data to the serial port;
   (+) DMA_LLP_MODE == DMA_LLP_PINGPONG
       Hardware connection: the tx of the board<->the rx of serial port tool, the rx of the board<->the tx of serial port tool,the ground <-> the ground;
       Data stream: the serial port sends data to the rx. rx_done is used to check whether the data is received. rec_buff/rec_buff1 alternately receive,Then the receive length hardware automatically writes back to the first four bytes of rec_buff/rec_buff1 and send the data to the serial port;
   @endverbatim
 */


#include "app_config.h"
//only the slave can support llp mode, the master cannot.
#if (UART_MODE==UART_DMA_LLP)

#define UART_DMA_CHANNEL_RX  DMA2
#define UART_DMA_CHANNEL_TX  DMA3

#define BUFF_DATA_LEN     256

#define DMA_LLP_SINGLE_CHAIN      1       //b91 and b92 are optional
#define DMA_LLP_PINGPONG          2       //only b92 can choose
#define DMA_LLP_MODE              2


#define DMA_REV_LEN          0xFFFFFC


volatile unsigned char rec_buff[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
volatile unsigned char rec_buff1[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
volatile unsigned char uart_dma_send_flag = 0;
volatile unsigned char rev_data_len=0;
volatile unsigned char pingpong_flag=0;
dma_chain_config_t rx_dma_list[2];
void user_init()
{
	unsigned short div;
	unsigned char bwpc;
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_function_en(LED3);
	gpio_function_en(LED4);


	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_output_en(LED3);
	gpio_output_en(LED4);


	uart_reset(UART0);
	uart_set_pin(UART0,UART0_TX_PIN,UART0_RX_PIN);// second parameter is tx_pin,third parameter is rx_pin.
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_rx_timeout(UART0, bwpc, 12, UART_BW_MUL2);//uart use rx_timeout to decide the end of a packet,UART_BW_MUL2 means rx_timeout is 2-byte.
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	uart_set_irq_mask(UART0, UART_RXDONE_MASK);
	uart_set_irq_mask(UART0, UART_TXDONE_MASK);
	plic_interrupt_enable(IRQ19_UART0);
	core_interrupt_enable();
	uart_set_tx_dma_config(UART0, UART_DMA_CHANNEL_TX);
/*
	  Application scenario:  write_num is available, the DMA receive length must be set to 0xfffffc byte,
							 the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation.
	  Possible problems:     Because the dma receive length is set to 0xfffffc byte, it is possible that the dma receive length is larger than the rec_buff set length,
							 which is out of the rec_buff's address range.
*/
#if(DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN)
	uart_rx_dma_chain_init(UART0,UART_DMA_CHANNEL_RX,(unsigned char *)(rec_buff+4),DMA_REV_LEN);
#elif(DMA_LLP_MODE == DMA_LLP_PINGPONG)

	uart_set_dma_chain_llp(UART0,UART_DMA_CHANNEL_RX,(unsigned char*)(rec_buff+4),DMA_REV_LEN,&rx_dma_list[0]);
	uart_rx_dma_add_list_element(UART0,UART_DMA_CHANNEL_RX,&rx_dma_list[0],&rx_dma_list[1],(unsigned char*)(rec_buff1+4),DMA_REV_LEN);
	uart_rx_dma_add_list_element(UART0,UART_DMA_CHANNEL_RX,&rx_dma_list[1],&rx_dma_list[0],(unsigned char*)(rec_buff+4),DMA_REV_LEN);
	dma_chn_en(UART_DMA_CHANNEL_RX);
#endif

}
void main_loop (void)
{
	gpio_toggle(LED1);
	delay_ms(500);
}
#if( (FLOW_CTR == NORMAL)||(FLOW_CTR ==  USE_CTS))
_attribute_ram_code_sec_ void uart0_irq_handler(void)
{
	if(uart_get_irq_status(UART0,UART_TXDONE_IRQ_STATUS))
    {
    	gpio_toggle(LED2);
    	uart_clr_irq_status(UART0,UART_TXDONE_IRQ_STATUS);
    }
    if(uart_get_irq_status(UART0,UART_RXDONE_IRQ_STATUS))
    {
    	gpio_toggle(LED3);
        if((uart_get_irq_status(UART0,UART_RX_ERR)))
        {
        	uart_clr_irq_status(UART0,UART_RXBUF_IRQ_STATUS);
        }

        uart_clr_irq_status(UART0,UART_RXDONE_IRQ_STATUS);
#if(DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN)
        rev_data_len = *(unsigned int*)rec_buff;
        uart_send_dma(UART0, (unsigned char*)(rec_buff+4), rev_data_len);
#elif(DMA_LLP_MODE == DMA_LLP_PINGPONG)
        if(pingpong_flag==0){
           rev_data_len = *(unsigned int*)rec_buff;
           uart_send_dma(UART0, (unsigned char*)(rec_buff+4), rev_data_len);
           pingpong_flag=1;
        }
        else if(pingpong_flag==1){
           rev_data_len = *(unsigned int*)rec_buff1;
           uart_send_dma(UART0, (unsigned char*)(rec_buff1+4), rev_data_len);
           pingpong_flag=0;
        }
#endif

    }
}
#endif

#endif












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

#define UART0_MODULE   0 //UART0
#define UART1_MODULE   1 //UART1

#if(MCU_CORE_B93)
#define UART2_MODULE   2 //UART2
#define UART3_MODULE   3 //UART3
#elif (MCU_CORE_B95)
#define UART2_MODULE   2 //UART2
#endif

#define UART_MODULE_SEL   UART0_MODULE

#define UART_DMA_CHANNEL_RX  DMA2
#define UART_DMA_CHANNEL_TX  DMA3

#define BUFF_DATA_LEN     256

#define DMA_LLP_SINGLE_CHAIN      1
#define DMA_LLP_PINGPONG          2
#define DMA_LLP_MODE              2
#if(MCU_CORE_B92)
#define DMA_REV_LEN          0xFFFFFC
#elif(MCU_CORE_B93 || MCU_CORE_B95)
#define DMA_REV_LEN          BUFF_DATA_LEN
#endif

unsigned char rec_buff[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
unsigned char rec_buff1[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
volatile unsigned int rev_data_len=0;
volatile unsigned int rev_data_len1=0;
volatile unsigned char pingpong_flag=0;
dma_chain_config_t rx_dma_list[2];
volatile unsigned char rx_done_flag=0;
void user_init(void)
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


	uart_reset(UART_MODULE_SEL);
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_set_pin(UART_MODULE_SEL,UART0_TX_PIN,UART0_RX_PIN);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_set_pin(UART_MODULE_SEL,UART1_TX_PIN,UART1_RX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	uart_set_pin(UART_MODULE_SEL,UART2_TX_PIN,UART2_RX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	uart_set_pin(UART_MODULE_SEL,UART3_TX_PIN,UART3_RX_PIN);
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
    uart_set_pin(UART_MODULE_SEL, UART2_TX_PIN, UART2_RX_PIN);
#endif
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
#if(MCU_CORE_B92)
	uart_set_rx_timeout(UART_MODULE_SEL, bwpc, 12, UART_BW_MUL2);
#elif(MCU_CORE_B93 || MCU_CORE_B95)
	uart_set_rx_timeout(UART_MODULE_SEL, bwpc, 12, UART_BW_MUL2,0);
#endif
	uart_init(UART_MODULE_SEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	dma_set_llp_int_mode(UART_DMA_CHANNEL_RX,DMA_INTERRUPT_MODE);
	dma_set_irq_mask(UART_DMA_CHANNEL_RX, TC_MASK);
	plic_interrupt_enable(IRQ_DMA);
	core_interrupt_enable();
	uart_set_tx_dma_config(UART_MODULE_SEL, UART_DMA_CHANNEL_TX);
/*
	  Application scenario(B92):  write_num is available, the DMA receive length must be set to 0xfffffc byte,
							      the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation.
	  Possible problems(B92):     Because the dma receive length is set to 0xfffffc byte, it is possible that the dma receive length is larger than the rec_buff set length,
							      which is out of the rec_buff's address range.
      B93: B93 does not have the problem of b92, dma can be configured to any length, hardware will have the write back length,
           chain jump conditions: one is uart rx timeout, one is when the receiving length reaches the set length, and will produce tc_mask interrupt, so can play no packet loss function.
*/
#if(DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN)
	uart_rx_dma_chain_init(UART_MODULE_SEL,UART_DMA_CHANNEL_RX,(unsigned char *)(rec_buff+4),DMA_REV_LEN);
#elif(DMA_LLP_MODE == DMA_LLP_PINGPONG)
	uart_set_dma_chain_llp(UART_MODULE_SEL,UART_DMA_CHANNEL_RX,(unsigned char*)(rec_buff+4),DMA_REV_LEN,&rx_dma_list[0]);
	uart_rx_dma_add_list_element(UART_MODULE_SEL,UART_DMA_CHANNEL_RX,&rx_dma_list[0],&rx_dma_list[1],(unsigned char*)(rec_buff1+4),DMA_REV_LEN);
	uart_rx_dma_add_list_element(UART_MODULE_SEL,UART_DMA_CHANNEL_RX,&rx_dma_list[1],&rx_dma_list[0],(unsigned char*)(rec_buff+4),DMA_REV_LEN);
	dma_chn_en(UART_DMA_CHANNEL_RX);
#endif

}
void main_loop (void)
{
	gpio_toggle(LED1);
	if(rx_done_flag ==1){
	rx_done_flag =0;
#if(DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN)
	uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4), rev_data_len);
	while(uart_tx_is_busy(UART_MODULE_SEL));
#elif(DMA_LLP_MODE == DMA_LLP_PINGPONG)
	if(pingpong_flag ==1){
		 uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4), rev_data_len);
		 while(uart_tx_is_busy(UART_MODULE_SEL));
	}else if(pingpong_flag ==0){
		 uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff1+4), rev_data_len1);
		 while(uart_tx_is_busy(UART_MODULE_SEL));
	}
#endif
	}
}
void dma_irq_handler(void)
{
    if(dma_get_tc_irq_status(BIT(UART_DMA_CHANNEL_RX)))
    {
    	dma_clr_tc_irq_status(BIT(UART_DMA_CHANNEL_RX));
			gpio_toggle(LED3);
#if(DMA_LLP_MODE == DMA_LLP_SINGLE_CHAIN)
			rev_data_len = *(unsigned int*)rec_buff;
#elif(DMA_LLP_MODE == DMA_LLP_PINGPONG)
			if(pingpong_flag==0){
			   rev_data_len = *(unsigned int*)rec_buff;
			   pingpong_flag=1;
			}
			else if(pingpong_flag==1){
			   rev_data_len1 = *(unsigned int*)rec_buff1;
			   pingpong_flag=0;
			}
#endif
			rx_done_flag=1;
   }

}

PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)

#endif












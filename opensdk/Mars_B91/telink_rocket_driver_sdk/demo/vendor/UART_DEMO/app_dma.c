/********************************************************************************************************
 * @file    app_dma.c
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
/**
   @verbatim
   ===============================================================================
                        ##### how to test demo #####
   ===============================================================================
   the nodma is divided into the following test items, which are described below:
   (+) FLOW_CTR == NORMAL(B91/B92)
       (++) UART_DEVICE ==  UART_MASTER_DEVICE
       Hardware connection: the tx <->the rx of serial port tool,the ground <-> the ground;
       Data stream: tx sends data to the serial port in dma mode, and sends the next data through the tx_done flag in the main_loop;
       (++) UART_DEVICE ==  UART_SLAVE_DEVICE
       Hardware connection: the tx of the board<->the rx of serial port tool, the rx of the board<->the tx of serial port tool,the ground <-> the ground;
         (+++)B91:
            Data stream: the serial port sends data to the rx. rx_done is used to check whether the data is received. Then uart_get_dma_rev_data_len is used to calculate the length and send the data to the serial port;
         (+++)B92:
            when the received length configured for rx_dma is less than 0xfffffc:
            Data stream: the serial port sends data to the rx. rx_done is used to check whether the data is received. Then uart_get_dma_rev_data_len is used to calculate the length and send the data to the serial port;
            when the received length configured for rx_dma is 0xfffffc:
            Data stream: the serial port sends data to the rx. rx_done is used to check whether the data is received. Then the receive length hardware automatically writes back to the first four bytes of rxbuff and send the data to the serial port;
   (+) FLOW_CTR==USE_CTS(B91/B92)
       Hardware connection: the tx of the board <-> rx of serial port tool, the ground <-> the ground;
       Data stream: tx sends data to the serial port in dma mode, and sends the next data through the tx_done flag in the main_loop continually,After the external sends a high level to the cts pin, the tx is not sending data;
   (+) FLOW_CTR == USE_RTS(B91/B92)
       Hardware connection: the rx of the board <-> tx of serial port tool, the ground <-> the ground;
       Data stream: The serial port sends a length of more than 5 bytes to the rx. Using the logic analyzer to grab the level of the rts pin, a change from low to high is found;

   In addition to the above test items, the demo gives an example of the conversion of an rtx pin to tx function
   and an example of what you need to do to enter suspend.
   @endverbatim
 */

#if (UART_MODE==UART_DMA)

#define UART0_MODULE   0 //UART0
#define UART1_MODULE   1 //UART1

#if(MCU_CORE_B93)
#define UART2_MODULE   2 //UART2
#define UART3_MODULE   3 //UART3
#elif (MCU_CORE_B95)
#define UART2_MODULE   2 //UART2
#endif

#define UART_MODULE_SEL   UART0_MODULE

#if( FLOW_CTR==USE_CTS)
	#define STOP_VOLT   	1			//0 :Low level stops TX.  1 :High level stops TX.
#endif

#if (FLOW_CTR==USE_RTS)
	#define RTS_MODE		UART_RTS_MODE_AUTO 		    //It can be UART_RTS_MODE_AUTO/UART_RTS_MODE_MANUAL.
	#define RTS_THRESH		5			//UART_RTS_MODE_AUTO need.It indicates RTS trigger threshold.
	#define RTS_INVERT		1			//UART_RTS_MODE_AUTO need.1 indicates RTS_pin will change from low to high.
	#define RTS_POLARITY	0			//UART_RTS_MODE_MANUAL need. It indicates RTS_POLARITY .
#endif

#define UART_MASTER_DEVICE   1
#if(FLOW_CTR == NORMAL)
#define UART_SLAVE_DEVICE    2
#endif

#define UART_DEVICE        UART_SLAVE_DEVICE

#define UART_DMA_CHANNEL_RX  DMA2
#define UART_DMA_CHANNEL_TX  DMA3

#define BUFF_DATA_LEN     256

#define DMA_REV_LEN_LESS_MAX     1       //b91/b92/b93/b95 are optional(dma can be configured to any length (as long as it is inside 0xfffffc))
#define DMA_REV_LEN_MAX          2       //only b92 can choose(dma can only be configured as 0xfffffc)
#define DMA_REV_LEN_TYPE         1

#if(DMA_REV_LEN_TYPE == DMA_REV_LEN_LESS_MAX)
#define DMA_REV_LEN           BUFF_DATA_LEN
#elif(MCU_CORE_B92 && (DMA_REV_LEN_TYPE == DMA_REV_LEN_MAX))
#define DMA_REV_LEN           0xFFFFFC
#endif


unsigned char tx_byte_buff[16] __attribute__((aligned(4))) ={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
#if(MCU_CORE_B91||(MCU_CORE_B92&&(DMA_REV_LEN_TYPE == DMA_REV_LEN_LESS_MAX)))
unsigned char rec_buff[BUFF_DATA_LEN] __attribute__((aligned(4))) = {0};
#elif(MCU_CORE_B95 || MCU_CORE_B93 || (MCU_CORE_B92&&(DMA_REV_LEN_TYPE == DMA_REV_LEN_MAX)))
unsigned char rec_buff[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
#endif
#if(MCU_CORE_B93 || MCU_CORE_B95)
unsigned char rec_buff1[BUFF_DATA_LEN+4] __attribute__((aligned(4))) = {0};
#endif
volatile unsigned char uart_dma_send_flag = 1;

volatile unsigned int rev_data_len=0;
volatile unsigned int rev_data_len1=0;

volatile unsigned char buff_rx_index=0;
volatile unsigned char buff_tx_index=0;

volatile unsigned char dma_rx_done_flag=0;
void user_init(void)
{
	unsigned short div;
	unsigned char bwpc;
	gpio_function_en(LED1);
	gpio_output_en(LED1); 		//enable output
	gpio_input_dis(LED1);		//disable input
	gpio_function_en(LED2);
	gpio_output_en(LED2); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_function_en(LED3);
	gpio_output_en(LED3); 		//enable output
	gpio_input_dis(LED3);		//disable input
	gpio_function_en(LED4);
	gpio_output_en(LED4); 		//enable output
	gpio_input_dis(LED4);		//disable input
	uart_reset(UART_MODULE_SEL);
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
#if(MCU_CORE_B91)
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_set_rtx_pin(UART0_RTX_PIN);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_set_rtx_pin(UART1_RTX_PIN);
#endif
#elif(MCU_CORE_B92||MCU_CORE_B93)
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_set_rtx_pin(UART_MODULE_SEL,UART0_RTX_PIN);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_set_rtx_pin(UART_MODULE_SEL,UART1_RTX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	uart_set_rtx_pin(UART_MODULE_SEL,UART2_RTX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	uart_set_rtx_pin(UART_MODULE_SEL,UART3_RTX_PIN);
#endif
#elif(MCU_CORE_B95)
#if(UART_MODULE_SEL == UART0_MODULE)
    uart_set_rtx_pin(UART_MODULE_SEL, UART0_RTX_PIN);
#elif(UART_MODULE_SEL == UART1_MODULE)
    uart_set_rtx_pin(UART_MODULE_SEL, UART1_RTX_PIN);
#elif(UART_MODULE_SEL == UART2_MODULE)
    uart_set_rtx_pin(UART_MODULE_SEL, UART2_RTX_PIN);
#endif
#endif
	uart_rtx_en(UART_MODULE_SEL);
#elif(UART_WIRE_MODE == UART_2WIRE_MODE)
#if(MCU_CORE_B91)
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_set_pin(UART0_TX_PIN,UART0_RX_PIN);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_set_pin(UART1_TX_PIN,UART1_RX_PIN);
#endif
#elif(MCU_CORE_B92||MCU_CORE_B93)
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_set_pin(UART_MODULE_SEL,UART0_TX_PIN,UART0_RX_PIN);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_set_pin(UART_MODULE_SEL,UART1_TX_PIN,UART1_RX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	uart_set_pin(UART_MODULE_SEL,UART2_TX_PIN,UART2_RX_PIN);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	uart_set_pin(UART_MODULE_SEL,UART3_TX_PIN,UART3_RX_PIN);
#endif
#elif (MCU_CORE_B95)
#if(UART_MODULE_SEL == UART0_MODULE)
    uart_set_pin(UART_MODULE_SEL, UART0_TX_PIN, UART0_RX_PIN);
#elif(UART_MODULE_SEL == UART1_MODULE)
    uart_set_pin(UART_MODULE_SEL, UART1_TX_PIN, UART1_RX_PIN);
#elif(UART_MODULE_SEL == UART2_MODULE)
    uart_set_pin(UART_MODULE_SEL, UART2_TX_PIN, UART2_RX_PIN);
#endif
#endif
#endif
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
#if(MCU_CORE_B92||MCU_CORE_B91)
	uart_set_rx_timeout(UART_MODULE_SEL, bwpc, 12, UART_BW_MUL2);
#elif(MCU_CORE_B93||MCU_CORE_B95)
	uart_set_rx_timeout(UART_MODULE_SEL, bwpc, 12, UART_BW_MUL2,0);
#endif
	uart_init(UART_MODULE_SEL, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	uart_set_tx_dma_config(UART_MODULE_SEL, UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART_MODULE_SEL, UART_DMA_CHANNEL_RX);

#if( FLOW_CTR == NORMAL)
#if(MCU_CORE_B91)
	uart_clr_tx_done(UART_MODULE_SEL);
#endif
	uart_set_irq_mask(UART_MODULE_SEL, UART_TXDONE_MASK);
#if(UART_MODULE_SEL == UART0_MODULE)
	plic_interrupt_enable(IRQ_UART0 );
#elif(UART_MODULE_SEL==UART1_MODULE)
	plic_interrupt_enable(IRQ_UART1);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	plic_interrupt_enable(IRQ_UART2);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	plic_interrupt_enable(IRQ_UART3);
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
	plic_interrupt_enable(IRQ_UART2);
#endif
	core_interrupt_enable();

#if(UART_DEVICE==UART_SLAVE_DEVICE)
#if((MCU_CORE_B91 || MCU_CORE_B92)&&(DMA_REV_LEN  ==  BUFF_DATA_LEN))
	uart_set_irq_mask(UART_MODULE_SEL, UART_RXDONE_MASK);

#elif(MCU_CORE_B93 || MCU_CORE_B95)
	dma_set_irq_mask(UART_DMA_CHANNEL_RX, TC_MASK);
	dma_set_wbufov_mask( BIT(UART_DMA_CHANNEL_RX));
	plic_interrupt_enable(IRQ_DMA);
#endif
#if(DMA_REV_LEN  ==  BUFF_DATA_LEN)
/*
	Application scenario(B91/B92) : the dma receive length is set to the rec_buff length to avoid receiving data that is too long and out of the rec_buff address range,
						            but the data receive length needs to be calculated by calling uart_get_dMA_rev_data_len.
	Possible problems(B91/B92):    1. the receive length cannot be greater than the DMA set length,otherwise excess data will be discarded.
	                      2. In an application, there may be a high priority interrupt source that interrupts the uart rx_done interrupt processing sequence,
						  in interruption period,if the next data is received to the fifo, when the re-entry of the UART RX_DONE interrupt,resulting in the error in computing the received length of the current data.
						  3.The two packets of data are very close to each other, but the rx_done signal of the previous data has also been generated. Before the rx_done interrupt flag and rx_fifo software are cleared,
						    the next data has been transferred, which leads to the error of clearing.
	B93:there is no problem with b91/b92, dma can be configured with any length, the hardware will generate the write back length, and it is also designed to not lose packets.

 */
#if(MCU_CORE_B91||MCU_CORE_B92)
	uart_receive_dma(UART_MODULE_SEL, (unsigned char*)rec_buff,DMA_REV_LEN);
#elif(MCU_CORE_B93 || MCU_CORE_B95)
	uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4),DMA_REV_LEN);
	buff_rx_index=1;
#endif

#elif((DMA_REV_LEN  ==  0xFFFFFC&&MCU_CORE_B92))
/*
	  Application scenario(B92):  write_num is available, the DMA receive length must be set to 0xfffffc byte,
	                         the dma automatically writes back the data receive length to the first four bytes of the rec_buff without manual calculation.
	  Possible problems:     Because the dma receive length is set to 0xfffffc byte, it is possible that the dma receive length is larger than the rec_buff set length,
	                         which is out of the rec_buff's address range.
*/
	dma_set_irq_mask(UART_DMA_CHANNEL_RX, TC_MASK);
	plic_interrupt_enable(IRQ5_DMA);
	uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4),DMA_REV_LEN);
#endif
#endif


#elif( FLOW_CTR ==  USE_CTS )
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_cts_config(UART_MODULE_SEL,UART0_CTS_PIN,STOP_VOLT);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_cts_config(UART_MODULE_SEL,UART1_CTS_PIN,STOP_VOLT);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	uart_cts_config(UART_MODULE_SEL,UART2_CTS_PIN,STOP_VOLT);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	uart_cts_config(UART_MODULE_SEL,UART3_CTS_PIN,STOP_VOLT);
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
	uart_cts_config(UART_MODULE_SEL,UART2_CTS_PIN,STOP_VOLT);
#endif
	uart_set_cts_en(UART_MODULE_SEL);
#if(MCU_CORE_B91)
	uart_clr_tx_done(UART_MODULE_SEL);
#endif
	uart_set_irq_mask(UART_MODULE_SEL, UART_TXDONE_MASK);
#if(UART_MODULE_SEL == UART0_MODULE)
	plic_interrupt_enable(IRQ_UART0);
#elif(UART_MODULE_SEL==UART1_MODULE)
	plic_interrupt_enable(IRQ_UART1);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	plic_interrupt_enable(IRQ_UART2);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	plic_interrupt_enable(IRQ_UART3);
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
	plic_interrupt_enable(IRQ_UART2);
#endif
	core_interrupt_enable();
#elif( FLOW_CTR ==   USE_RTS )
	uart_set_rts_en(UART_MODULE_SEL);
#if(UART_MODULE_SEL == UART0_MODULE)
	uart_rts_config(UART_MODULE_SEL,UART0_RTS_PIN,RTS_INVERT,RTS_MODE);
#elif(UART_MODULE_SEL==UART1_MODULE)
	uart_rts_config(UART_MODULE_SEL,UART1_RTS_PIN,RTS_INVERT,RTS_MODE);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
	uart_rts_config(UART_MODULE_SEL,UART2_RTS_PIN,RTS_INVERT,RTS_MODE);
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
	uart_rts_config(UART_MODULE_SEL,UART3_RTS_PIN,RTS_INVERT,RTS_MODE);
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
	uart_rts_config(UART_MODULE_SEL,UART2_RTS_PIN,RTS_INVERT,RTS_MODE);
#endif
	uart_rts_trig_level_auto_mode(UART_MODULE_SEL, RTS_THRESH);
#endif
#if(UART_DEMO_COMBINED_WITH_PM_FUNC)
	pm_set_gpio_wakeup(WAKEUP_PAD, WAKEUP_LEVEL_HIGH, 1);
	gpio_set_up_down_res(WAKEUP_PAD, GPIO_PIN_PULLDOWN_100K);
#endif

}
void main_loop (void)
{

#if( (FLOW_CTR == NORMAL)||(FLOW_CTR ==  USE_CTS))
#if(UART_DEVICE==UART_MASTER_DEVICE)
	gpio_toggle(LED1);
	delay_ms(500);
	if(uart_dma_send_flag==1)
	{
		uart_send_dma(UART_MODULE_SEL, (unsigned char*)tx_byte_buff, 16);
		uart_dma_send_flag=0;
#if( UART_WIRE_MODE == UART_1WIRE_MODE)
		uart_rtx_pin_tx_trig(UART_MODULE_SEL);
#endif
	}
#elif(UART_DEVICE==UART_SLAVE_DEVICE)
#if(MCU_CORE_B91||MCU_CORE_B92)
	gpio_toggle(LED1);
	delay_ms(500);
#elif(MCU_CORE_B93 || MCU_CORE_B95)
   if(dma_rx_done_flag ==1){
	   dma_rx_done_flag=0;
       while(!uart_dma_send_flag);
       uart_dma_send_flag=0;
	   if(buff_tx_index==0){
		  uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4), rev_data_len);
		  *(unsigned int*)rec_buff =0x00;
		  buff_tx_index=1;
	  }
	  else if(buff_tx_index==1){
		  uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff1+4), rev_data_len1);
		  *(unsigned int*)rec_buff1 =0x00;
		  buff_tx_index=0;
	  }
   }
#endif

#if(UART_DEMO_COMBINED_WITH_PM_FUNC)
#if(MCU_CORE_B91 || MCU_CORE_B92)
	if(uart_dma_send_flag == 1){
		uart_dma_send_flag = 0;
#elif(MCU_CORE_B93 || MCU_CORE_B95)
	if(uart_dma_send_flag == 1){
		uart_dma_send_flag = 0;
		tc_flag=0;
#endif
	  if(gpio_get_level(WAKEUP_PAD)==0){
	     pm_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, PM_TICK_STIMER_16M, 0);
		 uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff),BUFF_DATA_LEN);
		}
	}
#endif
#endif
#elif(FLOW_CTR ==  USE_RTS)
    while(1);
#endif
}
#if( (FLOW_CTR == NORMAL)||(FLOW_CTR ==  USE_CTS))

#if(UART_MODULE_SEL == UART0_MODULE)
_attribute_ram_code_sec_ void uart0_irq_handler(void)
#elif(UART_MODULE_SEL==UART1_MODULE)
_attribute_ram_code_sec_ void uart1_irq_handler(void)
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
_attribute_ram_code_sec_ void uart2_irq_handler(void)
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
_attribute_ram_code_sec_ void uart3_irq_handler(void)
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
_attribute_ram_code_sec_ void uart2_irq_handler(void)
#endif
{
#if(MCU_CORE_B91)
    if(uart_get_irq_status(UART_MODULE_SEL,UART_TXDONE))
#elif(MCU_CORE_B92||MCU_CORE_B93||MCU_CORE_B95)
   if(uart_get_irq_status(UART_MODULE_SEL,UART_TXDONE_IRQ_STATUS))
#endif
    {
    	gpio_toggle(LED2);
    	uart_dma_send_flag=1;
#if(MCU_CORE_B91)
    	uart_clr_tx_done(UART_MODULE_SEL);
#elif(MCU_CORE_B92 || MCU_CORE_B93 || MCU_CORE_B95)
    	uart_clr_irq_status(UART_MODULE_SEL,UART_TXDONE_IRQ_STATUS);
#endif
    }
#if(MCU_CORE_B91 || MCU_CORE_B92)
#if(MCU_CORE_B91)
    if(uart_get_irq_status(UART_MODULE_SEL,UART_RXDONE))
#elif(MCU_CORE_B92)
    if(uart_get_irq_status(UART_MODULE_SEL,UART_RXDONE_IRQ_STATUS))
#endif
    {
        gpio_toggle(LED3);
        if((uart_get_irq_status(UART_MODULE_SEL,UART_RX_ERR)))
        {
#if(MCU_CORE_B91)
        	uart_clr_irq_status(UART_MODULE_SEL,UART_CLR_RX);
#elif(MCU_CORE_B92)
        	uart_clr_irq_status(UART_MODULE_SEL,UART_RXBUF_IRQ_STATUS);
#endif
        }
        /************************get the length of receive data****************************/
        rev_data_len = uart_get_dma_rev_data_len(UART_MODULE_SEL,UART_DMA_CHANNEL_RX);
        /************************clr rx_irq****************************/
        //In b91m,clearing the interrupt status flag needs to be placed after calculating the length of the DMA received data,
        //otherwise the DMA received data length is calculated incorrectly.
#if(MCU_CORE_B91)
        uart_reset(UART_MODULE_SEL);
#elif(MCU_CORE_B92)
        uart_clr_irq_status(UART_MODULE_SEL,UART_RXDONE_IRQ_STATUS);
#endif
        //DMA access memory needs to be aligned according to word.
    	uart_receive_dma(UART_MODULE_SEL, (unsigned char*)rec_buff,DMA_REV_LEN);
    	uart_send_dma(UART_MODULE_SEL, (unsigned char*)rec_buff, rev_data_len);
    }
#endif
}
#if(UART_MODULE_SEL == UART0_MODULE)
    PLIC_ISR_REGISTER(uart0_irq_handler, IRQ_UART0 )
#elif(UART_MODULE_SEL == UART1_MODULE)
    PLIC_ISR_REGISTER(uart1_irq_handler, IRQ_UART1)
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART2_MODULE))
    PLIC_ISR_REGISTER(uart2_irq_handler, IRQ_UART2)
#elif(MCU_CORE_B93&&(UART_MODULE_SEL == UART3_MODULE))
    PLIC_ISR_REGISTER(uart3_irq_handler, IRQ_UART3)
#elif(MCU_CORE_B95&&(UART_MODULE_SEL == UART2_MODULE))
    PLIC_ISR_REGISTER(uart2_irq_handler, IRQ_UART2)
#endif



#if(UART_DEVICE==UART_SLAVE_DEVICE)
#if((DMA_REV_LEN  ==  0xFFFFFC)&&MCU_CORE_B92)
void dma_irq_handler(void)
{
	 if(dma_get_tc_irq_status( BIT(UART_DMA_CHANNEL_RX)))
	 {
		 dma_clr_tc_irq_status( BIT(UART_DMA_CHANNEL_RX));
        /************************get the length of receive data****************************/
        rev_data_len = *(unsigned int*)rec_buff;
        /************************clr rx_irq****************************/
        uart_clr_irq_status(UART_MODULE_SEL,UART_RXDONE_IRQ_STATUS);
        uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4),DMA_REV_LEN);
        uart_send_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4), rev_data_len);
	 }
}
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
#elif(MCU_CORE_B93 || MCU_CORE_B95)
void dma_irq_uart_rx_process(void){
	 if(buff_rx_index ==1){
		  rev_data_len = *(unsigned int*)rec_buff;
          //When the receive length is equal to the dma configured length, wbufov interrupt is generated,
          //when the wbufov interrupt is generated, there is no return length, and the receive length is the dma configured length.
		  if(rev_data_len ==0){
			  rev_data_len =DMA_REV_LEN;
		  }
		  uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff1+4),DMA_REV_LEN);
		  buff_rx_index=0;
	  }
	  else if(buff_rx_index==0){
		  rev_data_len1 = *(unsigned int*)rec_buff1;
		  //When the receive length is equal to the dma configured length, wbufov interrupt is generated,
		  //when the wbufov interrupt is generated, there is no return length, and the receive length is the dma configured length.
		  if(rev_data_len1 ==0){
			  rev_data_len1 =DMA_REV_LEN;
		  }
		  uart_receive_dma(UART_MODULE_SEL, (unsigned char*)(rec_buff+4),DMA_REV_LEN);
		  buff_rx_index=1;
	  }
	 dma_rx_done_flag=1;
}

/*
 * Scenario analysis, there are the following three situations:
 * 1. If the send length is smaller than the receive length, only tc interruption is generated
 * 2. The send length is equal to the receive length: tc/wbufov interrupt is generated(wbufov interrupts are faster than tc mask interrupts)
 * 3. The sending length is greater than the receiving length: When the receiving length reaches the configured length, the wbufov interrupt is generated, the dma is reconfigured, and the receiving continues until the tc interrupt is generated.
 * Through the above analysis, tc interrupt and wbufov interrupt are used for processing.
 * When the send length is equal to the receive length,wbufov interrupt is used; when the send length is less than the receive length, tc interrupt is used,
 * Interrupt processing: the rec_buff/rec_buff1 is alternately received through the buff_rx_index flag, and the received data is alternately sent through the buff_tx_index flag in main_loop.
 */
void dma_irq_handler(void)
{
	if(dma_get_tc_irq_status( BIT(UART_DMA_CHANNEL_RX))){
		 if((uart_get_irq_status(UART_MODULE_SEL,UART_RX_ERR)))
		 {
			uart_clr_irq_status(UART_MODULE_SEL,UART_RXBUF_IRQ_STATUS);
		 }
		 dma_clr_tc_irq_status( BIT(UART_DMA_CHANNEL_RX));
//When the send length is equal to the dma receive length, wbufov is interrupted, because the interrupt is interrupted, the tc interrupt is also up,
//when processing the interrupt, the tc interrupt is processed first, the wbufov interrupt will be cleared in the tc interrupt, once wbufov is interrupted,
//the dma write back length is 0, then the receive length is the dma configuration length.
		 dma_clr_wbufov_status( BIT(UART_DMA_CHANNEL_RX));
		 dma_irq_uart_rx_process();
	}
	else if(dma_get_wbufov_status( BIT(UART_DMA_CHANNEL_RX))){
		 if((uart_get_irq_status(UART_MODULE_SEL,UART_RX_ERR)))
		 {
			uart_clr_irq_status(UART_MODULE_SEL,UART_RXBUF_IRQ_STATUS);
		 }

		 dma_clr_wbufov_status( BIT(UART_DMA_CHANNEL_RX));
//When the send length is greater than the dma configuration length, after the dma length is reached, the current dma will not move data into the buff, but will continue to work, but will discard the excess data,
//before configuring the next dma, need to disable the last dma first (disable dma after,the current dma will no longer generate tc interruption), and then configure dma_set_wbufov_ctrl to stop data loss and continue to be received by the next dma.
		 dma_chn_dis(UART_DMA_CHANNEL_RX);
		 dma_set_wbufov_ctrl( BIT(UART_DMA_CHANNEL_RX));
		 dma_irq_uart_rx_process();
	 }
}
PLIC_ISR_REGISTER(dma_irq_handler, IRQ_DMA)
#endif
#endif

#endif

#endif











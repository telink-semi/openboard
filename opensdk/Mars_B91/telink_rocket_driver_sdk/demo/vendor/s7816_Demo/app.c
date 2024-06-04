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
/**
   @verbatim
   ===============================================================================
                        ##### ISO-7816 ATR protocol explain#####
   ===============================================================================
   ATR stands for "Answer to Reset" , which is the first instruction the card sends after reset. 
    - There are two protocols, T0 and T1, which the IC card must support, but not simultaneously. 
    - T=0 communication protocol is an asynchronous half-duplex character transmission protocol.
    - T=1 communication protocol is an asynchronous half-duplex block transmission protocol. 
    - If there is no TD1 in the reset response information, it means that the communication will use the T=0 protocol.
   
   ATR protocol format:
                        |     data element    |        explain       |
                        |:--------------------|:---------------------|
                        |         TS          |     start symbol     |
                        |         T0          |    format symbol     |
                        | TA1,TB1,TC1,TD1,... |   interface symbol   |
                        |  T1,T2,......,TK    |    history symbol    |
                        |         TCK         |  verification symbol |

    - TS is a mandatory part of ATR and must always be sent. This byte only allows two encodings : 
      - 3B for positive agreement.
      - 3F for negative agreement.

    - T0 contains a set of bits to indicate which interface character will be transmitted next and also indicates the number of subsequent historical characters. Like TS, this byte must be present in each ATR. 
      - The high half byte (b5-b8) indicates whether TA1 to TD1 exist in the subsequent characters. (b5 corresponds to TA1, and b8 corresponds to TD1).
      - The low half byte (b1-b4) indicates the number of optional historical characters (0 to 15).

    - TA1 communicates the values of FI and DI :
      - FI (high 4 bits) is used to determine the value of F, which is the clock frequency conversion factor, used to modify the clock frequency provided by the terminal after the reset. 
      - DI (low 4 bits) is used to determine the value of D, called the bit rate adjustment factor, used to adjust the bit duration after the reset response.

    - TD1 character is crucial :
      - The high 4 bits of TD1 determine whether TA2/TB2/TC2/TD2 exist. 
      - In the same way, the high 4 bits of TD2 determine whether TA3/TB3/TC3/TD3 exist, all the way to TDx not existing.

    - T1, T2,......,TK They indicate some general information :
      - Such as the manufacturer of the card, the chip embedded in the card, and the status of the files in the card.

    - TCK has a value that allows the integrity of the data sent during the reset response period to be checked :
      - The value of TCK should be such that the result of the XOR operation between T0 and including TCK is zero. When there is no TD1, T=0, TCK does not exist.
   @endverbatim
 */

/**
   @verbatim
    =============================================================================
                        ##### how to test demo #####
    =============================================================================
    hardware connection : rst <-> rst,vcc <-> vcc,clk <-> clk,trx <-> trx,gnd <-> gnd;
    The demo phenomenon is as follows:
    =========================================
    Take an initialized SMARTCOS-PSAM card as an example. The demo first performs a cold reset and then takes random numbers. Take random command is: 0x00,0x84,0x00,0x00,0x04 (take 4 bytes of random number)
    - Cold reset to obtain initial ATR
    - Taking random numbers is as follows:
        - Where 0x00,0x84,0x00,0x00,0x04 are the commands sent by the terminal to the IC card to take random numbers.
        - The random numbers obtained are a total of 4 bytes, and the random numbers taken will be different each time.
        - Data ending with ox9000 indicates successful execution of the command.
    - For some manufacturers' IC cards, custom characters may be added after the standard ATR reply, and the random number instruction may not be replied. All this depends on the settings of IC card manufacturers.
  @endverbatim
 */





#include "app_config.h"

volatile unsigned int  s7816_irq_cnt=0;

volatile __attribute__((aligned(4))) unsigned char s7816_rx_buff_byte[S7816_RX_BUFF_LEN] ={0};
volatile __attribute__((aligned(4))) unsigned char command[5]={0x00,0x84,0x00,0x00,0x04};//the command serves to get random number.

void user_init(void)
{
	s7816_set_pin(S7816_RST_PIN,S7816_VCC_PIN,S7816_CLK_PIN,S7816_TRX_PIN);
	s7816_init(S7816_UART_CHN,S7816_4MHZ,F,D);

	core_interrupt_enable();
#if( S7816_UART_CHN == S7816_UART0)
	plic_interrupt_enable(IRQ_UART0);
#elif(S7816_UART_CHN == S7816_UART1)
	plic_interrupt_enable(IRQ_UART1);
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
PLIC_ISR_REGISTER(uart0_irq_handler, IRQ_UART0)

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
PLIC_ISR_REGISTER(uart1_irq_handler, IRQ_UART1)
#endif



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
                        ##### how to test demo #####
   ===============================================================================
   master and slave end hardware connection:scl <-> scl,sda <-> sda,gnd <-> gnd,during the test, power on the slave and then the master.
   (+) I2C_DEVICE == I2C_MASTER_DEVICE(B91/B92)
       the bin is burned into the master, and the data flow situation:
       the master writes the data to the slave end, and then reads back to determine whether the written data is consistent with the read data;
   (+) I2C_DEVICE == I2C_SLAVE_DEVICE(B91/B92)
      (+) I2C_STRETCH_MODE == I2C_STRETCH_DIS(B91/B92)
           the bin is burned into the slave. data flow condition:
           the slave receives data from the master and then writes the received data back to the master.
      (+) I2C_STRETCH_MODE == I2C_STRETCH_EN(B92)
          (+)Another mechanism for communicating with the master,the bin is burned into the slave. data flow condition:
             the slave receives data from the master and then writes the received data back to the master.
   @endverbatim
 */
#include "app_config.h"

#if(I2C_MASTER_WRITE_READ_MODE == I2C_MASTER_WRITE_READ_NO_DMA)

/************************************************  i2c_mode  **********************************/
#define  	I2C_MASTER_DEVICE        	1   //i2c master demo
#define     I2C_SLAVE_DEVICE			2   //i2c slave demo

#define     I2C_DEVICE					I2C_MASTER_DEVICE

#if(MCU_CORE_B92)
#define I2C_STRETCH_EN       0
#define I2C_STRETCH_DIS      1
#define I2C_STRETCH_MODE     I2C_STRETCH_EN
#endif


#define     I2C_CLK_SPEED				400000 //i2c clock 400K.




#define     SLAVE_RX_IRQ_TRIG_LEVEL     4

volatile unsigned char i2c_read_flag =0;

#define     BUFF_DATA_LEN_NO_DMA	 		 16
unsigned char i2c_rx_buff[BUFF_DATA_LEN_NO_DMA] __attribute__((aligned(4)));


#define     BUFF_ADDR_DATA_LEN_NO_DMA	 		16
unsigned char i2c_tx_buff[BUFF_ADDR_DATA_LEN_NO_DMA] __attribute__((aligned(4)))= {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

volatile unsigned char  slave_rx_end_flag=0;

volatile unsigned char  slave_rx_done_end_flag=0;

void user_init(void)
{
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
    i2c_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);

#if(I2C_DEVICE == I2C_MASTER_DEVICE)
    i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));
#elif(I2C_DEVICE == I2C_SLAVE_DEVICE)
    i2c_slave_init(0x5a);
	i2c_rx_irq_trig_cnt(SLAVE_RX_IRQ_TRIG_LEVEL);
#if(MCU_CORE_B91)
    i2c_set_irq_mask(I2C_RX_BUF_MASK|I2C_RX_DONE_MASK);
#elif(MCU_CORE_B92||MCU_CORE_B93)
    i2c_set_irq_mask(I2C_RX_BUF_MASK|I2C_RX_END_MASK);
#if(I2C_STRETCH_MODE == I2C_STRETCH_EN)
    i2c_slave_stretch_en();
#endif
#endif
    plic_interrupt_enable(IRQ_I2C);
    core_interrupt_enable();
#endif
}

void main_loop(void)
{
#if(I2C_DEVICE == I2C_MASTER_DEVICE)

	delay_ms(10);
	i2c_master_write(0x5a,(unsigned char *)i2c_tx_buff, BUFF_ADDR_DATA_LEN_NO_DMA);
	delay_ms(10);
	i2c_master_read(0x5a,(unsigned char *)i2c_rx_buff, BUFF_DATA_LEN_NO_DMA);

	for(volatile unsigned int i=0;i<BUFF_DATA_LEN_NO_DMA;i++)
	{
		if( i2c_rx_buff[i] != i2c_tx_buff[i] )
		{
			gpio_set_high_level(LED2);
		}
	}
	i2c_tx_buff[0]++;
	gpio_toggle(LED1);

#endif


#if(I2C_DEVICE == I2C_SLAVE_DEVICE)
#if(MCU_CORE_B91||(MCU_CORE_B92 &&(I2C_STRETCH_MODE == I2C_STRETCH_DIS)))
	if( (slave_rx_done_end_flag==1)|| (slave_rx_end_flag==1))
	{
		i2c_slave_write((unsigned char*)i2c_rx_buff,BUFF_DATA_LEN_NO_DMA);
		slave_rx_done_end_flag=0;
		slave_rx_end_flag=0;
		gpio_toggle(LED4);
	}
#elif((MCU_CORE_B92||MCU_CORE_B93)||(I2C_STRETCH_MODE == I2C_STRETCH_EN))
	//parsing to the read and write command sent by the master, the interrupt state set 1,
	//judge whether it is a read command or not,the slave pull the clock line up,and fill in the data.
	if(i2c_get_irq_status(I2C_SLAVE_WR_STATUS))
	{
		i2c_clr_irq_status(I2C_SLAVE_WR_STATUS);
		if(I2C_SLAVE_WRITE == i2c_slave_get_cmd())
		{
		  i2c_slave_write((unsigned char*)i2c_rx_buff,BUFF_DATA_LEN_NO_DMA);
		  gpio_toggle(LED4);
		}
	}
#endif

#endif
}

/**
 * @brief		This function serves to handle the interrupt of i2c
 * @param[in] 	none
 * @return 		none
 */
//When the slave parsing ID does not match, the slave will return nack, but it will still receive the data sent from the master,
//enter the rx_buff and rx_done interrupt normally,, and reply nack.
_attribute_ram_code_sec_noinline_ void i2c_irq_handler(void)
{

#if(I2C_DEVICE==I2C_SLAVE_DEVICE)
#if(MCU_CORE_B91)
	if(i2c_get_irq_status(I2C_RX_BUF_STATUS))
	{
		i2c_slave_read((unsigned char*)(i2c_rx_buff+i2c_read_flag),SLAVE_RX_IRQ_TRIG_LEVEL);

		 i2c_read_flag+=SLAVE_RX_IRQ_TRIG_LEVEL;
		 if(i2c_read_flag>BUFF_DATA_LEN_NO_DMA-SLAVE_RX_IRQ_TRIG_LEVEL)
		 {
			 i2c_read_flag=0;
		    if(BUFF_DATA_LEN_NO_DMA % SLAVE_RX_IRQ_TRIG_LEVEL ==0 )
			 {
				 slave_rx_end_flag=1;
			 }
		 }
	}

	if((i2c_get_irq_status(I2C_RXDONE_STATUS)))//when BUFF_DATA_LEN_NO_DMA % SLAVE_RX_IRQ_TRIG_LEVEL =0,it can not enter rx_done irq.
	{
       if((BUFF_DATA_LEN_NO_DMA%SLAVE_RX_IRQ_TRIG_LEVEL !=0))
       {
    	   i2c_slave_read((unsigned char*)(i2c_rx_buff+BUFF_DATA_LEN_NO_DMA-BUFF_DATA_LEN_NO_DMA%SLAVE_RX_IRQ_TRIG_LEVEL),BUFF_DATA_LEN_NO_DMA%SLAVE_RX_IRQ_TRIG_LEVEL);
       }
	    i2c_clr_fifo(I2C_RX_BUFF_CLR);
		slave_rx_done_end_flag=1;
	}
#elif(MCU_CORE_B92||MCU_CORE_B93)
	if(i2c_get_irq_status(I2C_RX_BUF_STATUS))
	{
		i2c_slave_read((unsigned char*)(i2c_rx_buff+i2c_read_flag),SLAVE_RX_IRQ_TRIG_LEVEL);
		i2c_read_flag+=SLAVE_RX_IRQ_TRIG_LEVEL;
	}
	if((i2c_get_irq_status(I2C_RX_END_STATUS)))
	{
		 i2c_clr_irq_status(I2C_RX_END_STATUS);
		if(i2c_get_rx_buf_cnt()>0)
		{
			i2c_slave_read((unsigned char*)(i2c_rx_buff+i2c_read_flag),i2c_get_rx_buf_cnt());
		}
	   i2c_read_flag=0;
	   slave_rx_done_end_flag=1;
	}

#endif
	gpio_toggle(LED3);
#endif
}
PLIC_ISR_REGISTER(i2c_irq_handler, IRQ_I2C)
#endif




















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


#if(I2C_MASTER_WRITE_READ_MODE == I2C_MASTER_WRITE_READ_DMA)

/************************************************  i2c_mode  **********************************/
#define  	I2C_MASTER_DEVICE        	1   //i2c master demo
#define     I2C_SLAVE_DEVICE			2   //i2c slave demo
#define     I2C_DEVICE					I2C_MASTER_DEVICE

#if(MCU_CORE_B92)
/*
 * If the slave supports stretch when tx_fifo is 0, and the master supports stretch, then the slave can configure tx dma by referring to the demo with stretch enabled,
 * otherwise, refer to the demo of stretch dis. when the master read, the slave needs to fill data in advance
 */
#define     I2C_STRETCH_EN             1
#define     I2C_STRETCH_DIS            2
#define     I2C_STRETCH_MODE           I2C_STRETCH_EN
#endif


#define     I2C_CLK_SPEED				200000 //i2c clock 200K.

#define  I2C_TX_DMA_CHN  DMA1
#define  I2C_RX_DMA_CHN  DMA0
#define  I2C_RX_DMA_STATUS  BIT(I2C_RX_DMA_CHN)


#define     BUFF_DATA_LEN_DMA	 	     	     16
volatile unsigned char i2c_tx_buff[BUFF_DATA_LEN_DMA] __attribute__((aligned(4))) = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
#define     RX_BUFF_DATA_LEN_DMA	 	     	 16

#if(MCU_CORE_B91)
#define  DMA_REV_LEN    RX_BUFF_DATA_LEN_DMA
#elif(MCU_CORE_B92)
//In DMA mode, only one demo is provided where DMA_REV_LEN is 0xFFFFFC (maximum),If  want to set the length to less than 0XFFFFFC, can ask the driver to confirm how to use it.
#define  DMA_REV_LEN    0xFFFFFC
#endif


//Note:In B91,the length of data received by the slave must be the same as the length of data sent by the master.
//In B92,the length of data received by slave should not be greater than the length of i2c_rx_buff, otherwise it will be out of the i2c_rx_buff's address range.
#if(MCU_CORE_B91)
volatile unsigned char i2c_rx_buff[RX_BUFF_DATA_LEN_DMA] __attribute__((aligned(4)));
#elif(MCU_CORE_B92)
volatile unsigned char i2c_rx_buff[RX_BUFF_DATA_LEN_DMA+4] __attribute__((aligned(4)));
#endif
volatile unsigned char i2c_master_read_nack_cnt=0;
volatile unsigned char i2c_master_write_nack_cnt=0;


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
	i2c_set_rx_dma_config(I2C_RX_DMA_CHN);
	i2c_set_tx_dma_config(I2C_TX_DMA_CHN);

#if(I2C_DEVICE == I2C_MASTER_DEVICE)
	//For eagle the slave ID is 0x5a, for kite the slave ID is 0x5c.
	i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));//set i2c frequency 200K.
#if(MCU_CORE_B92)
#if(I2C_STRETCH_MODE == I2C_STRETCH_EN)
	i2c_master_stretch_en();
#endif
	i2c_master_detect_nack_en();
	i2c_set_irq_mask(I2C_MASTER_NAK_MASK);
	plic_interrupt_enable(IRQ21_I2C);
	core_interrupt_enable();
#endif
#elif(I2C_DEVICE == I2C_SLAVE_DEVICE)
#if(MCU_CORE_B92 &&I2C_STRETCH_MODE == I2C_STRETCH_EN)
	i2c_slave_stretch_en();
#endif
	i2c_slave_init(0x5a);
#if(MCU_CORE_B91)
	plic_interrupt_enable(IRQ5_DMA);
	dma_set_irq_mask(I2C_RX_DMA_CHN, TC_MASK);
    i2c_clr_irq_status(I2C_TX_DONE_CLR);
	i2c_set_irq_mask(I2C_TX_DONE_MASK);
#elif(MCU_CORE_B92)
	i2c_set_irq_mask(I2C_RX_END_MASK|I2C_SLAVE_WR_MASK);
#if(I2C_STRETCH_MODE == I2C_STRETCH_DIS)
	dma_set_irq_mask(I2C_RX_DMA_CHN, TC_MASK);
	plic_interrupt_enable(IRQ5_DMA);
#endif
#endif
	core_interrupt_enable();
	plic_interrupt_enable(IRQ21_I2C);

/*
 * B91 and B92 demo different:
 * B91: the DMA receive length must be set to be the same as the length of data sent by the master;
 * B92: 1.If the receiving length of DMA is set to max value:0xfffffc byte, and write_num is turned on,
 *        then The length of the data received by dma will be written to the first four bytes of addr;
 *	    2.If the receiving length information of DMA is set to less than max value:0xfffffc byte, even if write_num is turned on,
 *	       the length of data received by DMA will not be written to the first four bytes of addr;
 *	    in b92 demo,it choose that the receiving length of DMA be set to the maximum value of the scenario.
 */
#if(MCU_CORE_B91)
    i2c_slave_set_rx_dma((unsigned char*)(i2c_rx_buff),DMA_REV_LEN);
#elif(MCU_CORE_B92)
    i2c_slave_set_rx_dma((unsigned char*)(i2c_rx_buff+4),DMA_REV_LEN);
#endif
#endif
}

void main_loop(void)
{
#if(I2C_DEVICE == I2C_MASTER_DEVICE)

	delay_ms(10);
	i2c_master_write_dma(0x5a,(unsigned char *)i2c_tx_buff,BUFF_DATA_LEN_DMA);
	while(i2c_master_busy());
	delay_ms(10);
    i2c_master_read_dma(0x5a,(unsigned char *)i2c_rx_buff,RX_BUFF_DATA_LEN_DMA);
	while(i2c_master_busy());

	for(volatile unsigned int i=0;i<RX_BUFF_DATA_LEN_DMA;i++)
	{
		if( i2c_rx_buff[i] != i2c_tx_buff[i] )
		{
			gpio_toggle(LED2);
			delay_ms(50);
		}
	}
	i2c_tx_buff[0]++;
	gpio_toggle(LED1);
#endif
}

//Open the i2c tc_mask dma interrupt, when moving configuration data length of the dma, tc_dma interrupt will rise.
//when enabling dma transfers i2c receiving data, the internal working principle: when the i2c fifo each reach 4,
//will send the dma requests, to move data from the fifo, if finally fifo data less than four,
//through the i2c stop signal sends a request to the dma,move the fifo data away.
#if(MCU_CORE_B91||(MCU_CORE_B92 &&(I2C_STRETCH_MODE == I2C_STRETCH_DIS)))
void dma_irq_handler(void)
{
#if(I2C_DEVICE == I2C_SLAVE_DEVICE)

    if(dma_get_tc_irq_status(I2C_RX_DMA_STATUS))
    {
    	dma_clr_tc_irq_status(I2C_RX_DMA_STATUS);
#if(MCU_CORE_B91)
    	i2c_slave_set_tx_dma((unsigned char*)(i2c_rx_buff),RX_BUFF_DATA_LEN_DMA);
#elif(MCU_CORE_B92 &&(I2C_STRETCH_MODE == I2C_STRETCH_DIS))
    	i2c_slave_set_tx_dma((unsigned char*)(i2c_rx_buff+4),RX_BUFF_DATA_LEN_DMA);
#endif
        gpio_toggle(LED1);
	}

	gpio_toggle(LED3);


#endif

}
#endif

//When the slave parsing ID does not match, the slave will return nack, but it will still receive the data sent from the master,
//enter the rx_buff and rx_done interrupt normally,, and reply nack.
 void i2c_irq_handler(void)
{
#if(MCU_CORE_B92)
#if(I2C_DEVICE==I2C_MASTER_DEVICE)
	if(i2c_get_irq_status(I2C_MASTER_NAK_STATUS)){
      	i2c_clr_irq_status(I2C_MASTER_NAK_CLR);
		reg_i2c_sct1 = (FLD_I2C_LS_STOP);
		while(i2c_master_busy());//wait for the STOP signal to finish sending.
		dma_chn_dis(I2C_TX_DMA_CHN);
		//In B92, when sending data in the master, whether in the ID stage or data stage, if NACK signal is received,
		//the processing in the interrupt is disable TX_DMA, clear FIFO, stop the transmission of data, record the number of exceptions,
		//after interrupt, and process the following operations.
		if(I2C_MASTER_WRITE == i2c_get_master_wr_status())
		{
            i2c_clr_fifo(I2C_TX_BUFF_CLR);
			i2c_master_write_nack_cnt++;
		}
		//When receiving data in the master, nack can be received only in the id stage,
		//After receiving nack, the rx_dma shall be disable to stop the data receiving and record the abnormal count,
		//If it is in the data stage the slave is abnormal, the master will not reply nack and detect abnormal problems,
		//so it continues to receive the data 0xFF.When an exception occurs, the data sent in the main_loop does not match the data received.
		if(I2C_MASTER_READ == i2c_get_master_wr_status())
		{
			i2c_master_read_nack_cnt++;
		}
	 }

#endif
#endif
#if(I2C_DEVICE == I2C_SLAVE_DEVICE)

#if(MCU_CORE_B91)
	if(i2c_get_irq_status(I2C_TXDONE_STATUS))
		{
			i2c_clr_irq_status(I2C_TX_DONE_CLR);
			i2c_slave_set_rx_dma((unsigned char*)(i2c_rx_buff),DMA_REV_LEN);
			gpio_toggle(LED2);
		}
#elif(MCU_CORE_B92)
#if(I2C_STRETCH_MODE == I2C_STRETCH_DIS)
	if(i2c_get_irq_status(I2C_TX_DONE_STATUS))
	{
		i2c_clr_irq_status(I2C_TX_DONE_CLR);
		i2c_slave_set_rx_dma((unsigned char*)(i2c_rx_buff+4),DMA_REV_LEN);
		gpio_toggle(LED2);
	}

#elif(I2C_STRETCH_MODE == I2C_STRETCH_EN)
    if(i2c_get_irq_status(I2C_SLAVE_WR_STATUS))
    {
    	i2c_clr_irq_status(I2C_SLAVE_WR_CLR);
    	if(I2C_SLAVE_WRITE == i2c_slave_get_cmd())
        {
		 i2c_slave_set_tx_dma((unsigned char*)(i2c_rx_buff+4),*(unsigned int *)i2c_rx_buff);
		 gpio_toggle(LED4);
	    }
    }
	if((i2c_get_irq_status(I2C_RX_END_STATUS)))
	{
		i2c_clr_irq_status(I2C_RX_END_CLR);
		i2c_slave_set_rx_dma((unsigned char*)(i2c_rx_buff+4),DMA_REV_LEN);
	}
#endif
#endif
#endif


}

#endif















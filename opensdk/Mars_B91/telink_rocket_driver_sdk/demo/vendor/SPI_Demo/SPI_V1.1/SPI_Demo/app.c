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

#if (SPI_MODE == SPI_NDMA_MODE)
/**********************************************************************************************************************
 *                                         SPI device  selection                                                	 *
 *********************************************************************************************************************/
#define SPI_MASTER_DEVICE		       		1
#define SPI_SLAVE_DEVICE		      		2

#define SPI_DEVICE				   	   SPI_MASTER_DEVICE

/**********************************************************************************************************************
 *                                         SPI protocol demo selection                                                *
 *********************************************************************************************************************/
#define B85M_SLAVE_PROTOCOL		   			1 // B85m as slave
#define B91M_SLAVE_PROTOCOL		   			2 // B91m hspi/pspi/gspi/lspi slave mode as slave
#define B91M_SPI_SLAVE_PROTOCOL		       	3 // B91m spi slave as slave

#define SPI_PROTOCOL			       B91M_SLAVE_PROTOCOL

/**********************************************************************************************************************
 *                                         SPI module selection                                               	 	  *
 *********************************************************************************************************************/
#define LSPI_MODULE	    			             0
#define GSPI_MODULE	    			             1

#define SPI_MODULE_SEL	    			        GSPI_MODULE
/**********************************************************************************************************************
 *                                         SPI multiple slave                                                                                    	 	  *
 *********************************************************************************************************************/
#define ONE_SLAVE	       						 0
#define MULTI_SLAVE		   						 1

#define SPI_SLAVE_NUM	    				 ONE_SLAVE
/**********************************************************************************************************************
 *                                         3line SPI  enable                                              	 	  *
 *********************************************************************************************************************/
#define SPI_NORMAL_SLAVE	       					0
#define SPI_3LINE_SLAVE		   						1

#define SPI_SLAVE_MODE	    				   SPI_NORMAL_SLAVE
/**********************************************************************************************************************
 *                                          SPI slave ready test
 *When the slave_ready bit of the slave is 1, the master sends read status cmd, and the slave will reply 0x5a.
 *When the slave_ready bit of the slave is 0, the master sends read status cmd, and the slave will reply 0x00.                                	 	  *
 *********************************************************************************************************************/
#define SPI_SLAVE_READY_TEST	       					0

/**********************************************************************************************************************
*                                         SPI clock set
*********************************************************************************************************************/
#define SPI_CLK	    					  	1000000
/**********************************************************************************************************************
*                                         SPI pin set                                            	 	  *
*********************************************************************************************************************/
#if (SPI_MODULE_SEL == GSPI_MODULE)
gspi_pin_config_t gspi_pin_config = {
	.spi_csn_pin 		= GPIO_FC_PA0,
	.spi_clk_pin		= GPIO_FC_PA3,
	.spi_mosi_io0_pin   = GPIO_FC_PB3,
	.spi_miso_io1_pin   = GPIO_FC_PB2,//3line mode set 0
	.spi_io2_pin     	= GPIO_FC_PB1,//set quad mode otherwise set 0
	.spi_io3_pin   		= GPIO_FC_PB0,//set quad mode otherwise set 0
};
#if (SPI_SLAVE_NUM == MULTI_SLAVE)
//39 GPIOs can be multiplexed as PIN for GSPI_CSN0_IO function.Only a part of IO is listed here.
#define SLAVE_CSN_PIN_NUM    8
gpio_pin_e slave_csn_pin[SLAVE_CSN_PIN_NUM] = {GPIO_FC_PA0, /*GPIO_FC_PA1, GPIO_FC_PA2, GPIO_FC_PA3, */GPIO_FC_PA4,
 		/*GPIO_FC_PB0, GPIO_FC_PB1, */GPIO_FC_PB2, GPIO_FC_PB3, GPIO_FC_PB4, GPIO_FC_PB5, GPIO_FC_PB6, GPIO_FC_PB7};
#endif
#elif (SPI_MODULE_SEL == LSPI_MODULE)
lspi_pin_config_t lspi_pin_config = {
	.spi_csn_pin 		= LSPI_CSN_PE0_PIN,
	.spi_clk_pin		= LSPI_CLK_PE1_PIN,
	.spi_mosi_io0_pin   = LSPI_MOSI_IO0_PE2_PIN,
	.spi_miso_io1_pin   = LSPI_MISO_IO1_PE3_PIN,//3line mode set 0
	.spi_io2_pin    	= LSPI_IO2_PE4_PIN,//set quad mode otherwise set 0
	.spi_io3_pin  		= LSPI_IO3_PE5_PIN,//set quad mode otherwise set 0
};
#endif

#if (SPI_DEVICE == SPI_MASTER_DEVICE)
/**********************************************************************************************************************
*                                         SPI master setting 				                                	 	*
*********************************************************************************************************************/
#define 	DATA_BYTE_LEN     16
unsigned char spi_rx_buff[DATA_BYTE_LEN] __attribute__((aligned(4))) = {0x00};
#if (SPI_PROTOCOL == B85M_SLAVE_PROTOCOL)
#define SPI_B85M_READ_CMD			0x80
#define SPI_B85M_WRITE_CMD			0x00
typedef struct {
	unsigned char address[3];
	unsigned char cmd;
	unsigned char data[DATA_BYTE_LEN];
} spi_b85m_slave_protocol_t;
spi_b85m_slave_protocol_t __attribute__((aligned(4))) spi_tx_buff = {
	.address = {0x04, 0x40, 0x00},
	.cmd = SPI_B85M_WRITE_CMD,
	.data = {0xAA, 0x10, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xBB},
};

#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
#if(SPI_SLAVE_READY_TEST == 1)
volatile unsigned char spi_b91m_slave_io_mode;
#endif
spi_wr_rd_config_t spi_b91m_slave_protocol_config = {
	.spi_io_mode		= SPI_SINGLE_MODE,/*IO mode set to SPI_3_LINE_MODE when SPI_3LINE_SLAVE.*/
	.spi_dummy_cnt  	= 32,//B92 supports up to 32-byte dummy, and B93 supports up to 256-byte dummy.
	.spi_cmd_en 		= 1,
	.spi_addr_en 		= 0,
	.spi_addr_len 		= 4,//when spi_addr_en = 0,invalid set.
	.spi_cmd_fmt_en  	= 0,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 0,//when spi_addr_en = 0,invalid set.
};
typedef struct
{
	unsigned char address[4];
	unsigned char data_len;
	unsigned char data[DATA_BYTE_LEN + 3];
}  spi_b91m_slave_protocol_t;
spi_b91m_slave_protocol_t __attribute__((aligned(4))) spi_tx_buff =
{
	.address = {0xc0, 0x20, 0x04, 0x00},/*.address = {0xc0, 0x00, 0x00, 0x00},*/
	.data_len = DATA_BYTE_LEN + 3,
	.data = {0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
};

#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
#define spi_slave_address	0xc0200400
spi_wr_rd_config_t spi_b91m_spi_slave_protocol_config = {
	.spi_io_mode 		= SPI_SINGLE_MODE,
	.spi_dummy_cnt 		= 8,
	.spi_cmd_en 		= 1,
	.spi_addr_en 		= 1,
	.spi_addr_len 		= 4,//when hspi_addr_en = 0,invalid set.
	.spi_cmd_fmt_en 	= 0,//when hspi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 0,//when hspi_addr_en=0,invalid set.
};
typedef struct
	{
		unsigned char data[DATA_BYTE_LEN];
	}  spi_b91m_spi_slave_protocol_t;
	spi_b91m_spi_slave_protocol_t __attribute__((aligned(4)))  spi_tx_buff =
	{
		.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
	};
#endif

void user_init(void)
{
	delay_ms(2000);
	gpio_output_en(LED1); 		//enable output
	gpio_input_dis(LED1);		//disable input
	gpio_set_low_level(LED1);
	gpio_function_en(LED1);
	gpio_output_en(LED2); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_set_low_level(LED2);      //LED On
	gpio_function_en(LED2);

	spi_master_init(SPI_MODULE_SEL, sys_clk.pll_clk * 1000000/SPI_CLK, SPI_MODE0);
#if (SPI_MODULE_SEL == GSPI_MODULE)
	gspi_set_pin(&gspi_pin_config);
#elif(SPI_MODULE_SEL == LSPI_MODULE)
	lspi_set_pin(&lspi_pin_config);
#endif

#if (SPI_PROTOCOL == B85M_SLAVE_PROTOCOL)
	spi_master_config(SPI_MODULE_SEL, SPI_NOMAL);

#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_master_config_plus(SPI_MODULE_SEL, &spi_b91m_slave_protocol_config);
#if(SPI_SLAVE_READY_TEST == 1)
	spi_b91m_slave_io_mode = spi_b91m_slave_protocol_config.spi_io_mode;
#endif
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
	spi_master_config_plus(SPI_MODULE_SEL, &spi_b91m_spi_slave_protocol_config);
#endif

}
#if(SPI_SLAVE_READY_TEST == 1)
/**
 * @brief 		This function servers to get lspi/gspi slave ready status. When slave is ready, slave ready reply a byte data:0x5a.
 * slave reply a byte data: 0x00.indicating that slave is not ready for data transmission.
 * @param[in] 	spi_sel 	- the spi module.
 * @param[in] 	mode	 	- the spi master io mode.
 * @return 		1:Indicates that the slave is ready. 0:Indicates that slave is not ready.
 */
_Bool spi_master_get_slave_ready(spi_sel_e spi_sel,spi_io_mode_e mode)
{
	unsigned char slave_ready_flag[1] = {0};
	switch (mode)
	{
		case SPI_SINGLE_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 1, SPI_MODE_RD_DUMMY_READ);
			break;
		case SPI_DUAL_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_DUAL_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 1, SPI_MODE_RD_DUMMY_READ);
			break;
		case SPI_QUAD_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_QUAD_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 1, SPI_MODE_RD_DUMMY_READ);
			break;
		case SPI_3_LINE_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 1, SPI_MODE_RD_DUMMY_READ);
			break;
	}
	while (spi_is_busy(spi_sel));
	if (slave_ready_flag[0] == 0x5a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
#endif
void main_loop (void)
{
	delay_ms(200);
	gpio_toggle(LED1);
#if (SPI_PROTOCOL == B85M_SLAVE_PROTOCOL)
	spi_tx_buff.cmd = SPI_B85M_WRITE_CMD;
	spi_master_write(SPI_MODULE_SEL, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff));
	spi_tx_buff.cmd = SPI_B85M_READ_CMD;
	spi_master_write_read(SPI_MODULE_SEL, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff.address)+ sizeof(spi_tx_buff.cmd),(unsigned char*)spi_rx_buff, DATA_BYTE_LEN);
	for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[i] != (spi_rx_buff[i]))
		{
			gpio_set_high_level(LED2);//LED on indicate data error
		}
	}
	spi_tx_buff.data[0]++;
	
#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
#if(SPI_SLAVE_READY_TEST == 1)
	while(1)
	{
		if(!spi_master_get_slave_ready(SPI_MODULE_SEL,spi_b91m_slave_io_mode))
		{
			gpio_set_high_level(LED2);//If the LED is always on, the slave ready function is abnormal.
		}
		else
		{
			gpio_toggle(LED2);//The flashing LED indicates that the slave ready function is normal.
			break;
		}
	}
#else
	spi_master_write_plus(SPI_MODULE_SEL, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_DUMMY_WRITE);
	spi_master_read_plus(SPI_MODULE_SEL, SPI_READ_DATA_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
	for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[3 + i] != (spi_rx_buff[i]))
		{
			gpio_set_high_level(LED2);//LED on indicate data error
			while(1);
		}
	}
	spi_tx_buff.data[3]++;
#endif
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
	spi_master_write_plus(SPI_MODULE_SEL, SPI_SLAVE_WRITE_DATA_CMD, spi_slave_address, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_WRITE_ONLY);
	spi_master_read_plus(SPI_MODULE_SEL, SPI_SLAVE_READ_DATA_CMD, spi_slave_address, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
	for (unsigned char i = 0; i < DATA_BYTE_LEN; i++)
	{
		if(spi_tx_buff.data[i] != (spi_rx_buff[i]))
		{
			gpio_set_high_level(LED2);//LED on indicate data error
		}
	}
	spi_tx_buff.data[0]++;
#endif
#if (SPI_SLAVE_NUM == MULTI_SLAVE)
#if (SPI_MODULE_SEL == GSPI_MODULE)
	for(unsigned char i = 0;i < SLAVE_CSN_PIN_NUM-1; i++)
	{
		if (gspi_pin_config.spi_csn_pin == slave_csn_pin[i])
			{
				gspi_change_csn_pin(slave_csn_pin[i],slave_csn_pin[i+1]);
				gspi_pin_config.spi_csn_pin = slave_csn_pin[i+1];
			}
	}
#endif
#endif
}

#elif(SPI_DEVICE == SPI_SLAVE_DEVICE)
   /**********************************************************************************************************************
	*                                         SPI slave setting 				                                	 	  *
	*********************************************************************************************************************/
#if(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
#define SPI_RX_BUFF_LEN     24
#define DATA_BYTE_LEN    	16
#define DATA_BYTE_OFFSET    8 //must be a multiple 4
unsigned char spi_slave_rx_buff[SPI_RX_BUFF_LEN+4] __attribute__((aligned(4))) = {0x00};
volatile unsigned int spi_irq_rx_cnt = 0;
volatile unsigned int spi_tx_cntn = 0;
volatile unsigned int spi_tx_num = 0;
#endif

void user_init(void)
{
	delay_ms(2000);
	gpio_output_en(LED1); 		//enable output
	gpio_input_dis(LED1);			//disable input
	gpio_function_en(LED1);
	gpio_set_low_level(LED1);              	//LED On

	gpio_output_en(LED2); 		//enable output
	gpio_input_dis(LED2);
	gpio_function_en(LED2);
	gpio_set_low_level(LED2);
	
	
#if(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_slave_init(SPI_MODULE_SEL, SPI_MODE0);
	spi_tx_irq_trig_cnt(SPI_MODULE_SEL,4);
	spi_clr_irq_status(SPI_MODULE_SEL, SPI_SLV_CMD_INT|SPI_RXF_INT|SPI_END_INT);
	spi_set_irq_mask(SPI_MODULE_SEL, SPI_SLV_CMD_EN |SPI_RXFIFO_INT_EN |SPI_END_INT_EN );//endint_en txfifoint_en rxfifoint_en
	//B92 supports up to 32-byte dummy.
	//When B92 and B91 communicate with SPI, B91 as slave supports up to 8byte dummy. When B91 is master, invalid data can be added before tx_buff to act as dummy.
	spi_set_dummy_cnt(SPI_MODULE_SEL, 32);
#if(SPI_SLAVE_MODE == SPI_3LINE_SLAVE)
	spi_set_3line_mode(SPI_MODULE_SEL);
#endif

#if (SPI_MODULE_SEL == GSPI_MODULE)
	gspi_set_pin(&gspi_pin_config);
	plic_interrupt_enable(IRQ_GSPI);
#elif(SPI_MODULE_SEL == LSPI_MODULE)
	lspi_set_pin(&lspi_pin_config);
	plic_interrupt_enable(IRQ_LSPI);
#endif
	spi_slave_ready_en(SPI_MODULE_SEL);
	core_interrupt_enable();
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
	spi_slave_set_pin();//spi slave only need set pin.
#endif
}

void main_loop(void)
{
	delay_ms(200);
#if(SPI_SLAVE_READY_TEST == 1)
	spi_slave_ready_en(SPI_MODULE_SEL);
#endif
	gpio_toggle(LED1);
}


#if(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
/*  handle cmd  interrupt when received read cmd enable txfifo interrupt */
_attribute_ram_code_sec_noinline_ void spi_irq_slv_cmd_process(void)
{
	switch (spi_slave_get_cmd(SPI_MODULE_SEL))
	{
		case SPI_READ_DATA_SINGLE_CMD:
		case SPI_READ_DATA_DUAL_CMD:
		case SPI_READ_DATA_QUAD_CMD:
			spi_set_irq_mask(SPI_MODULE_SEL,SPI_TXFIFO_INT_EN);//enable txfifo_int

			break;
	}
}
 /*  handle txfifo  interrupt to return data that received from data master */
_attribute_ram_code_sec_noinline_ void spi_irq_tx_fifo_process(void)
{
	spi_tx_num = spi_slave_rx_buff[4] - 3;// return data number to master.
	if ((spi_tx_num - spi_tx_cntn) < 4)
	{
		spi_write(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff  + DATA_BYTE_OFFSET  + spi_tx_cntn), spi_tx_num - spi_tx_cntn);
		spi_clr_irq_mask(SPI_MODULE_SEL,SPI_TXFIFO_INT_EN);//disable txfifo_int
		spi_tx_cntn = 0;//reset
		spi_tx_num = 0;
	}

	else
	{
		spi_write(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + DATA_BYTE_OFFSET  + spi_tx_cntn), 4);
		spi_clr_irq_status(SPI_MODULE_SEL, SPI_TXF_INT);//clr
		spi_tx_cntn += 4;
	}
}
 /*  handle rxfifo  interrupt to  received data from  master */
_attribute_ram_code_sec_noinline_ void spi_irq_rx_fifo_process(void)
{
	if(spi_irq_rx_cnt < (SPI_RX_BUFF_LEN-(SPI_RX_BUFF_LEN % 4)))
	{
		spi_read(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + spi_irq_rx_cnt), 4);
		spi_irq_rx_cnt += 4;
		}
	else
	{
		spi_read(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + spi_irq_rx_cnt), SPI_RX_BUFF_LEN % 4);
		spi_irq_rx_cnt += SPI_RX_BUFF_LEN % 4;
		spi_rx_fifo_clr(SPI_MODULE_SEL);
	}
}

 /*  handle end  interrupt to  received remains data  from master */
_attribute_ram_code_sec_noinline_ void spi_irq_end_process(void)
{
	if(((spi_get_rxfifo_num(SPI_MODULE_SEL)) != 0) && (spi_irq_rx_cnt < SPI_RX_BUFF_LEN))
	{
		spi_read(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + spi_irq_rx_cnt),  spi_get_rxfifo_num(SPI_MODULE_SEL));
	}
	spi_tx_fifo_clr(SPI_MODULE_SEL);
	spi_rx_fifo_clr(SPI_MODULE_SEL);
	spi_irq_rx_cnt = 0;
}
#if(SPI_MODULE_SEL == GSPI_MODULE)
_attribute_ram_code_sec_noinline_ void gspi_irq_handler(void)
#elif(SPI_MODULE_SEL == LSPI_MODULE)
_attribute_ram_code_sec_noinline_ void lspi_irq_handler(void)
#endif
{
	if (spi_get_irq_status(SPI_MODULE_SEL,SPI_SLV_CMD_INT))
	{
		spi_irq_slv_cmd_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_SLV_CMD_INT);//clr
	}

	if (spi_get_irq_status(SPI_MODULE_SEL,SPI_RXF_INT))
	{
		spi_irq_rx_fifo_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_RXF_INT);//clr
	}

	if(spi_get_irq_status(SPI_MODULE_SEL,SPI_END_INT))
	{
		spi_irq_end_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_END_INT);//clr
	}

	if (spi_get_irq_mask(SPI_MODULE_SEL,SPI_TXFIFO_INT_EN)&&spi_get_irq_status(SPI_MODULE_SEL,SPI_TXF_INT))
	{
		spi_irq_tx_fifo_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_TXF_INT);//clr
	}
}

#if(SPI_MODULE_SEL == GSPI_MODULE)
PLIC_ISR_REGISTER(gspi_irq_handler, IRQ_GSPI)
#elif(SPI_MODULE_SEL == LSPI_MODULE)
PLIC_ISR_REGISTER(lspi_irq_handler, IRQ_LSPI)
#endif

#endif
#endif
#endif



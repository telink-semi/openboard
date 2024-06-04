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
#define SPI_MASTER_DEVICE		       		  1
#define SPI_SLAVE_DEVICE		      		  2

#define SPI_DEVICE				   		    SPI_MASTER_DEVICE

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
#define PSPI_MODULE		       					 0
#define HSPI_MODULE		   						 1

#define SPI_MODULE_SEL	    					HSPI_MODULE

/**********************************************************************************************************************
 *                                         SPI multiple slave                                         	 	  *
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
* * When cclk pclk and hclk are set to 24MHz, the SPI_CLK[no use DMA] range are as follows
* B85M_SLAVE_PROTOCOL    	PSPI_MODULE 			max:depend on spi slave  min:48000
* 									HSPI_MODULE 			max:depend on spi slave  min:48000
* B91M_SLAVE_PROTOCOL 			PSPI_MODULE single io	max:500000    min:48000
* 												dual io		max:500000    min:48000
* 									HSPI_MODULE single io	max:500000    min:48000
* 												dual io		max:500000    min:48000
* 												quad io		max:400000    min:48000
* B91M_SPI_SLAVE_PROTOCOL 		 		PSPI_MODULE single io	max:12000000  min:48000
* 												dual io		max:4000000   min:48000
* 									HSPI_MODULE single io	max:12000000  min:48000
* 												dual io		max:6000000   min:48000
*
*********************************************************************************************************************/
#define SPI_CLK	    					  	400000
/**********************************************************************************************************************
*                                         SPI pin set
*                          lection  just set for  hspi/pspi  master/slave mode  not include spi slave                                            	 	  *
*********************************************************************************************************************/
#if (SPI_MODULE_SEL == PSPI_MODULE)
pspi_pin_config_t pspi_pin_config = {
	.pspi_clk_pin 		= PSPI_CLK_PC5,
	.pspi_csn_pin   	= PSPI_CSN_PC4,
	.pspi_mosi_io0_pin  = PSPI_MOSI_IO0_PC7,
	.pspi_miso_io1_pin  = PSPI_MISO_IO1_PC6,
};
#if (SPI_SLAVE_NUM == MULTI_SLAVE)
#define SLAVE_CSN_PIN_NUM    3
pspi_csn_pin_def_e slave_csn_pin[SLAVE_CSN_PIN_NUM] = {GPIO_PC0, GPIO_PC4, GPIO_PD0};
#endif
#elif (SPI_MODULE_SEL == HSPI_MODULE)
hspi_pin_config_t hspi_pin_config = {
	.hspi_clk_pin		= HSPI_CLK_PB4,
	.hspi_csn_pin 		= HSPI_CSN_PB6,
	.hspi_mosi_io0_pin  = HSPI_MOSI_IO0_PB3,
	.hspi_miso_io1_pin  = HSPI_MISO_IO1_PB2,//3line mode set none
	.hspi_wp_io2_pin    = HSPI_WP_IO2_PB1,//set quad mode otherwise set none
	.hspi_hold_io3_pin  = HSPI_HOLD_IO3_PB0,//set quad mode otherwise set none
};
#if (SPI_SLAVE_NUM == MULTI_SLAVE)
#define SLAVE_CSN_PIN_NUM    2
hspi_csn_pin_def_e slave_csn_pin[SLAVE_CSN_PIN_NUM] = {GPIO_PB6, GPIO_PA1};
#endif
#endif

#if (SPI_DEVICE == SPI_MASTER_DEVICE)
	/**********************************************************************************************************************
	*                                         SPI master setting 				                                	 	  *
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
#if(SPI_MODULE_SEL == PSPI_MODULE)
	pspi_config_t pspi_slave_protocol_config = {
		.pspi_io_mode 	= PSPI_SINGLE,/*Io mode set to PSPI_3LINE when SPI_3LINE_SLAVE.*/
		.pspi_dummy_cnt = 8,
		.pspi_cmd_en 	= 1,
	};
#elif(SPI_MODULE_SEL == HSPI_MODULE)
	hspi_config_t hspi_slave_protocol_config = {
		.hspi_io_mode 		= HSPI_SINGLE,/*Io mode set to HSPI_3LINE when SPI_3LINE_SLAVE.*/
		.hspi_dummy_cnt 	= 8,
		.hspi_cmd_en 		= 1,
		.hspi_addr_en 		= 0,//must
		.hspi_addr_len 		= 4,//when hspi_addr_en = false, invalid set.
		.hspi_cmd_fmt_en 	= 0,//when hspi_cmd_en = false, invalid set.
		.hspi_addr_fmt_en 	= 0,//when hspi_addr_en = false, invalid set.
	};
#endif
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
#if(SPI_MODULE_SEL == PSPI_MODULE)
	pspi_config_t spi_slave_protocol_config = {
		.pspi_io_mode 	= PSPI_SINGLE,
		.pspi_dummy_cnt = 8,
		.pspi_cmd_en 	= 1,
	};

#elif(SPI_MODULE_SEL == HSPI_MODULE)
#define spi_slave_address	0xc0000200
	hspi_config_t spi_slave_protocol_config = {
		.hspi_io_mode 		= HSPI_SINGLE,
		.hspi_dummy_cnt 	= 8,
		.hspi_cmd_en 		= 1,
		.hspi_addr_en 		= 1,
		.hspi_addr_len 		= 4,//when hspi_addr_en = false, invalid set.
		.hspi_cmd_fmt_en 	= 0,//when hspi_cmd_en = false, invalid set.
		.hspi_addr_fmt_en 	= 1,//when hspi_addr_en = false, invalid set.
	};
#endif
	typedef struct
	{
#if(SPI_MODULE_SEL == PSPI_MODULE)
		unsigned char address[4];
#endif
		unsigned char data[DATA_BYTE_LEN];
	}  spi_b91m_spi_slave_protocol_t;
	spi_b91m_spi_slave_protocol_t __attribute__((aligned(4)))  spi_tx_buff =
	{
#if(SPI_MODULE_SEL == PSPI_MODULE)
		.address = {0xc0, 0x00, 0x02, 0x00},
#endif
		.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
	};


#endif

void user_init(void)
{
	delay_ms(2000);
	gpio_output_en(LED2); 		//enable output
	gpio_output_en(LED4); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_input_dis(LED4);		//disable input
	gpio_set_high_level(LED4);              	     //LED4  On
	gpio_set_low_level(LED2);
	gpio_function_en(LED2);
	gpio_function_en(LED4);
	
#if(SPI_MODULE_SEL == PSPI_MODULE)
	spi_master_init(SPI_MODULE_SEL, sys_clk.pclk * 1000000 / (2 * SPI_CLK) - 1, SPI_MODE0);
	pspi_set_pin(&pspi_pin_config);
#elif(SPI_MODULE_SEL == HSPI_MODULE)
	spi_master_init(SPI_MODULE_SEL, sys_clk.hclk * 1000000 / (2 * SPI_CLK) - 1, SPI_MODE0);
	hspi_set_pin(&hspi_pin_config);
#endif

#if (SPI_PROTOCOL == B85M_SLAVE_PROTOCOL)
	spi_master_config(SPI_MODULE_SEL, SPI_NOMAL);
#elif(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
#if(SPI_MODULE_SEL == PSPI_MODULE)
	pspi_master_config_plus(&pspi_slave_protocol_config);
#if(SPI_SLAVE_READY_TEST == 1)
	spi_b91m_slave_io_mode = pspi_slave_protocol_config.pspi_io_mode;
#endif
#elif(SPI_MODULE_SEL == HSPI_MODULE)
	hspi_master_config_plus(&hspi_slave_protocol_config);
#if(SPI_SLAVE_READY_TEST == 1)
	spi_b91m_slave_io_mode = hspi_slave_protocol_config.hspi_io_mode;
#endif
#endif
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
#if(SPI_MODULE_SEL == PSPI_MODULE)
	pspi_master_config_plus(&spi_slave_protocol_config);
#elif(SPI_MODULE_SEL == HSPI_MODULE)
	hspi_master_config_plus(&spi_slave_protocol_config);
#endif
#endif
}
#if(SPI_SLAVE_READY_TEST == 1)
/**
 * @brief 		This function servers to get hspi/pspi slave ready status.
 * @param[in] 	spi_sel 	- the spi module.
 * @param[in] 	mode	 	- the spi master io mode.
 * @return 		1:Indicates that the slave is ready. 0:Indicates that slave is not ready.
 */
_Bool spi_master_get_slave_ready(spi_sel_e spi_sel,spi_io_mode_e mode)
{
	unsigned char slave_ready_flag[17] = {0};
	switch (mode)
	{
		case SPI_SINGLE_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 5, SPI_MODE_RD_READ_ONLY);
			if (slave_ready_flag[4] == 0x5a)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			break;
		case SPI_DUAL_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_DUAL_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 9, SPI_MODE_RD_READ_ONLY);
			if (slave_ready_flag[8] == 0x5a)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			break;
		case HSPI_QUAD_MODE:
			spi_master_read_plus(spi_sel, HSPI_READ_STATUS_QUAD_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 17, SPI_MODE_RD_READ_ONLY);
			if (slave_ready_flag[16] == 0x5a)
			{
				return 1;
			}
			else
			{
				return 0;
			}
			break;
		case SPI_3_LINE_MODE:
			spi_master_read_plus(spi_sel, SPI_READ_STATUS_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)slave_ready_flag, 5, SPI_MODE_RD_READ_ONLY);
			if (slave_ready_flag[4] == 0x5a)
			{
				return 1;
			}
			else
			{
				return 0;
			}

			break;
	}
	return 0;
}
#endif
void main_loop (void)
{
	delay_ms(200);
	gpio_toggle(LED4);
#if (SPI_PROTOCOL == B85M_SLAVE_PROTOCOL)
	spi_tx_buff.cmd = SPI_B85M_WRITE_CMD;
	spi_master_write(SPI_MODULE_SEL, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff));
	spi_tx_buff.cmd = SPI_B85M_READ_CMD;
	//DATA_BYTE_LEN shouldn't set to 0. Write address and command first, then read.
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
		}
	}
	spi_tx_buff.data[3]++;
#endif
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
#if(SPI_MODULE_SEL == HSPI_MODULE)
	spi_master_write_plus(SPI_MODULE_SEL, SPI_SLAVE_WRITE_DATA_CMD, spi_slave_address, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_WRITE_ONLY);
	spi_master_read_plus(SPI_MODULE_SEL, SPI_SLAVE_READ_DATA_CMD, spi_slave_address, (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);
#elif(SPI_MODULE_SEL == PSPI_MODULE)
	spi_master_write_plus(SPI_MODULE_SEL, SPI_SLAVE_WRITE_DATA_CMD, 0x00, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff), SPI_MODE_WR_WRITE_ONLY);
	spi_master_write_read_plus(SPI_MODULE_SEL, SPI_SLAVE_READ_DATA_CMD, (unsigned char*)&spi_tx_buff, sizeof(spi_tx_buff.address), (unsigned char*)spi_rx_buff, DATA_BYTE_LEN, SPI_MODE_WR_DUMMY_RD);
#endif
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
#if (SPI_MODULE_SEL == HSPI_MODULE)
for(unsigned char i = 0;i < SLAVE_CSN_PIN_NUM-1; i++)
{
	if (hspi_pin_config.hspi_csn_pin == slave_csn_pin[i])
		{
			hspi_change_csn_pin(slave_csn_pin[i],slave_csn_pin[i+1]);
			hspi_pin_config.hspi_csn_pin = slave_csn_pin[i+1];
			break;
		}
}
#elif (SPI_MODULE_SEL == PSPI_MODULE)
for(unsigned char i = 0;i < SLAVE_CSN_PIN_NUM-1; i++)
{
	if (pspi_pin_config.pspi_csn_pin == slave_csn_pin[i])
		{
			pspi_change_csn_pin(slave_csn_pin[i],slave_csn_pin[i+1]);
			pspi_pin_config.pspi_csn_pin = slave_csn_pin[i+1];
			break;
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
volatile unsigned char spi_slave_rx_buff[SPI_RX_BUFF_LEN] __attribute__((aligned(4))) = {0x00};
volatile unsigned int spi_irq_rx_cnt = 0;
volatile unsigned int spi_tx_cntn = 0;
volatile unsigned int spi_tx_num = 0;
#endif
void user_init(void)
{
	delay_ms(2000);
	gpio_output_en(LED2); 		//enable output
	gpio_output_en(LED4); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_input_dis(LED4);		//disable input
	gpio_set_high_level(LED4);              	     //LED4  On
	gpio_set_low_level(LED2);
	gpio_function_en(LED2);
	gpio_function_en(LED4);
	
#if(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)
	spi_slave_init(SPI_MODULE_SEL, SPI_MODE0);
	spi_set_irq_mask(SPI_MODULE_SEL, SPI_SLV_CMD_EN |SPI_RXFIFO_INT_EN |SPI_END_INT_EN );//endint_en txfifoint_en rxfifoint_en
#if(SPI_MODULE_SEL == PSPI_MODULE)
	pspi_set_pin(&pspi_pin_config);
	plic_interrupt_enable(IRQ_SPI_APB);
#elif(SPI_MODULE_SEL == HSPI_MODULE)
	hspi_set_pin(&hspi_pin_config);
	plic_interrupt_enable(IRQ_SPI_AHB);
#endif
#if(SPI_SLAVE_MODE == SPI_3LINE_SLAVE)
	spi_set_3line_mode(SPI_MODULE_SEL);
#endif
	core_interrupt_enable();
#elif(SPI_PROTOCOL == B91M_SPI_SLAVE_PROTOCOL)
	spi_slave_set_pin();//spi slave only need set pin.
#endif
}

void main_loop(void)
{
	delay_ms(200);
	gpio_toggle(LED4);
#if(SPI_SLAVE_READY_TEST == 1)
	spi_slave_ready_en(SPI_MODULE_SEL);
#endif
}


#if(SPI_PROTOCOL == B91M_SLAVE_PROTOCOL)

/*  handle cmd  interrupt when received read cmd enable txfifo interrupt */
_attribute_ram_code_sec_ void spi_irq_slv_cmd_process(void)
{
	switch (spi_slave_get_cmd(SPI_MODULE_SEL))
	{
		case SPI_READ_DATA_SINGLE_CMD:
		case SPI_READ_DATA_DUAL_CMD:
		case HSPI_READ_DATA_QUAD_CMD:
			spi_set_irq_mask(SPI_MODULE_SEL,SPI_TXFIFO_INT_EN);//enable txfifo_int
			break;
	}
}
 /*  handle txfifo  interrupt to return data that received from data master */
_attribute_ram_code_sec_ void spi_irq_tx_fifo_process(void)
{
	spi_tx_num = spi_slave_rx_buff[4] - 3;// return data number to master.
	if ((spi_tx_num - spi_tx_cntn) < 4)
	{
		spi_write(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + DATA_BYTE_OFFSET +  spi_tx_cntn), spi_tx_num - spi_tx_cntn);
		spi_clr_irq_mask(SPI_MODULE_SEL,SPI_TXFIFO_INT_EN);//disable txfifo_int
		spi_tx_cntn = 0;//reset
		spi_tx_num = 0;
	}

	else
	{
		spi_write(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + DATA_BYTE_OFFSET +  spi_tx_cntn), 4);
		spi_clr_irq_status(SPI_MODULE_SEL, SPI_TXF_INT);//clr
		spi_tx_cntn += 4;
	}

}
 /*  handle rxfifo  interrupt to  received data from  master */
_attribute_ram_code_sec_ void spi_irq_rx_fifo_process(void)
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
_attribute_ram_code_sec_ void spi_irq_end_process(void)
{
	if(((spi_get_rxfifo_num(SPI_MODULE_SEL)) != 0) && (spi_irq_rx_cnt < SPI_RX_BUFF_LEN))
	{
		spi_read(SPI_MODULE_SEL, (unsigned char *)(spi_slave_rx_buff + spi_irq_rx_cnt),  spi_get_rxfifo_num(SPI_MODULE_SEL));
	}
	spi_tx_fifo_clr(SPI_MODULE_SEL);
	spi_rx_fifo_clr(SPI_MODULE_SEL);
	spi_irq_rx_cnt = 0;
}

#if(SPI_MODULE_SEL == PSPI_MODULE)
_attribute_ram_code_sec_ void pspi_irq_handler(void)
#elif(SPI_MODULE_SEL == HSPI_MODULE)
_attribute_ram_code_sec_ void hspi_irq_handler(void)
#endif
{
	unsigned char spi_irq = spi_get_irq_status(SPI_MODULE_SEL,0xff);
	if (spi_irq & SPI_SLV_CMD_INT)
	{
		spi_irq_slv_cmd_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_SLV_CMD_INT);//clr
	}

	if (spi_irq & SPI_RXF_INT)
	{
		spi_irq_rx_fifo_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_RXF_INT);//clr
	}

	if(spi_irq & SPI_END_INT)
	{
		spi_irq_end_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_END_INT);//clr
	}

	if (spi_irq & SPI_TXF_INT)
	{
		spi_irq_tx_fifo_process();
		spi_clr_irq_status(SPI_MODULE_SEL,SPI_TXF_INT);//clr
	}

}
#if(SPI_MODULE_SEL == PSPI_MODULE)
PLIC_ISR_REGISTER(pspi_irq_handler, IRQ_SPI_APB)
#elif(SPI_MODULE_SEL == HSPI_MODULE)
PLIC_ISR_REGISTER(hspi_irq_handler, IRQ_SPI_AHB)
#endif

#endif
#endif
#endif



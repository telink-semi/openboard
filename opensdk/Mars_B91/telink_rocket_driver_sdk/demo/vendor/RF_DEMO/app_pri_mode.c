/********************************************************************************************************
 * @file    app_pri_mode.c
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
#if (RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M || RF_MODE==RF_PRIVATE_500K || RF_MODE==RF_PRIVATE_250K)


unsigned char  rx_packet[128*4]  __attribute__ ((aligned (4)));
unsigned char  Private_TPLL_tx_packet[48] __attribute__ ((aligned (4))) ={3,0,0,0,0,10,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xc,0xf};
unsigned char  Private_SB_tx_packet[48] __attribute__ ((aligned (4))) ={3,0,0,0,0,20,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

#define TX					1
#define RX					2
#define RF_TRX_MODE			TX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		MANUAL

#define TPLL_MODE  			1
#define SB_MODE   			2
#define PRI_MODE			SB_MODE


#define RF_RX_IRQ_EN				0

#define TX_PKT_PAYLOAD		32

#define RX_FIFO_NUM			4
#define RX_FIFO_DEP			128

#define RF_FREQ				17
#define ACCESS_CODE        0x29417671// 0xd6be898e//
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;

#if(RF_RX_IRQ_EN)
_attribute_ram_code_sec_ void rf_irq_handler(void)
{


	if(rf_get_irq_status(FLD_RF_IRQ_RX ))
	{
#if(RF_AUTO_MODE == AUTO)
		unsigned char* raw_pkt = rf_get_rx_packet_addr(RX_FIFO_NUM,RX_FIFO_DEP,rx_packet);
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
		if(rf_pri_tpll_packet_crc_ok(raw_pkt))
		{
			rx_cnt++;
			gpio_toggle(LED2);
		}
		rf_start_srx(stimer_get_tick());
#elif(PRI_MODE==SB_MODE)
		if(rf_pri_sb_packet_crc_ok(raw_pkt))
		{
			rx_cnt++;
		  gpio_toggle(LED2);
		}
		rf_start_srx(stimer_get_tick());
#endif
#else
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
		if(rf_pri_tpll_packet_crc_ok(rx_packet))
		{
			rx_cnt++;
			gpio_toggle(LED2);
		}
#elif(PRI_MODE==SB_MODE)
		if(rf_pri_sb_packet_crc_ok(rx_packet))
		{
			rx_cnt++;
			gpio_toggle(LED2);
		}

#endif
#endif
			rf_clr_irq_status(FLD_RF_IRQ_RX);

	}
	else
	{
		rf_clr_irq_status(0xffff);
	}


}
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)
#endif

#if(RF_AUTO_MODE == AUTO)
void user_init(void)
{
	rf_set_power_level(RF_POWER);
	rf_set_chn(RF_FREQ);
	rf_access_code_comm(ACCESS_CODE);
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))

#elif(PRI_MODE==SB_MODE)
	rf_private_sb_en();
	rf_set_private_sb_len(TX_PKT_PAYLOAD);
#endif

#if(RF_TRX_MODE==TX)
	rf_set_tx_dma(2,128);
#elif(RF_TRX_MODE==RX)
	rf_set_rx_dma(rx_packet,RX_FIFO_NUM-1,RX_FIFO_DEP);
#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX );
	rf_start_srx(stimer_get_tick());
#endif
#endif

	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);

}


void main_loop(void)
{


#if(RF_TRX_MODE==TX)
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
	unsigned char rf_data_len = TX_PKT_PAYLOAD+1;
	Private_TPLL_tx_packet[4]=TX_PKT_PAYLOAD;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	Private_TPLL_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	Private_TPLL_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	Private_TPLL_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	Private_TPLL_tx_packet[0] = rf_tx_dma_len&0xff;
#elif(PRI_MODE==SB_MODE)
	unsigned char rf_data_len = TX_PKT_PAYLOAD;
//	Private_SB_tx_packet[4]=0x20;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	Private_SB_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	Private_SB_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	Private_SB_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	Private_SB_tx_packet[0] = rf_tx_dma_len&0xff;
#endif

#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
	rf_start_stx(Private_TPLL_tx_packet, stimer_get_tick());
#elif(PRI_MODE== SB_MODE)
	rf_start_stx(Private_SB_tx_packet, stimer_get_tick());
#endif
	while(1)
	{

		delay_ms(1);
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX )));
		rf_clr_irq_status(FLD_RF_IRQ_TX );
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
		rf_start_stx(Private_TPLL_tx_packet, stimer_get_tick());
#elif(PRI_MODE== SB_MODE)
		rf_start_stx(Private_SB_tx_packet, stimer_get_tick());
#endif
		gpio_toggle(LED4);
		//delay_ms(100);
		tx_cnt++;
	}


#elif(RF_TRX_MODE==RX)
#if(!RF_RX_IRQ_EN)

	rf_start_srx(stimer_get_tick());
	while(1)
	{
		if(rf_get_irq_status(FLD_RF_IRQ_RX ))
		{
			unsigned char* raw_pkt = rf_get_rx_packet_addr(RX_FIFO_NUM,RX_FIFO_DEP,rx_packet);
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
			if(rf_pri_tpll_packet_crc_ok(raw_pkt))
#elif(PRI_MODE==SB_MODE)
			if(rf_pri_sb_packet_crc_ok(raw_pkt))
#endif
			{
				gpio_toggle(LED4);
				rx_cnt++;
//				delay_ms(10);

			}
				rf_clr_irq_status(FLD_RF_IRQ_RX );
				rf_start_srx(stimer_get_tick());


		}

	}
#endif
#endif
	gpio_toggle(LED4);
	delay_ms(100);
}

#elif(RF_AUTO_MODE == MANUAL)

void user_init(void)
{
	rf_set_power_level(RF_POWER);
	rf_set_chn(RF_FREQ);
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))

#elif(PRI_MODE==SB_MODE)
	rf_private_sb_en();
	rf_set_private_sb_len(TX_PKT_PAYLOAD);
#endif

	rf_access_code_comm(ACCESS_CODE);

#if(RF_TRX_MODE==TX)
	rf_set_tx_dma(2,128);
#elif(RF_TRX_MODE==RX)
	rf_set_rx_dma(rx_packet,RX_FIFO_NUM-1,RX_FIFO_DEP);

#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX );
	rf_set_rxmode();
	delay_us(85);//Wait for calibration to stabilize

#endif
#endif

	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);

}

void main_loop(void)
{
#if(RF_TRX_MODE==TX)
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
	unsigned char rf_data_len = TX_PKT_PAYLOAD+1;
	Private_TPLL_tx_packet[4]=TX_PKT_PAYLOAD;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	Private_TPLL_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	Private_TPLL_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	Private_TPLL_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	Private_TPLL_tx_packet[0] = rf_tx_dma_len&0xff;
#elif(PRI_MODE==SB_MODE)
	unsigned char rf_data_len = TX_PKT_PAYLOAD;
//	Private_TPLL_tx_packet[4]=0x20;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	Private_SB_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	Private_SB_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	Private_SB_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	Private_SB_tx_packet[0] = rf_tx_dma_len&0xff;
#endif

	rf_set_txmode();
	delay_us(113);//Wait for calibration to stabilize

	while(1)
	{
		delay_ms(1);
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
		rf_tx_pkt(Private_TPLL_tx_packet);
#elif(PRI_MODE== SB_MODE)
		rf_tx_pkt(Private_SB_tx_packet);
#endif
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX )));
		rf_clr_irq_status(FLD_RF_IRQ_TX );
		gpio_toggle(LED4);
		//delay_ms(100);
		tx_cnt++;
	}


#elif(RF_TRX_MODE==RX)
#if(!RF_RX_IRQ_EN)


	rf_set_rxmode();
	delay_us(85);//Wait for calibration to stabilize
	while(1)
	{
		if(rf_get_irq_status(FLD_RF_IRQ_RX ))
		{
#if(PRI_MODE==TPLL_MODE&&(RF_MODE == RF_PRIVATE_1M || RF_MODE==RF_PRIVATE_2M))
			if(rf_pri_tpll_packet_crc_ok(rx_packet))
#elif(PRI_MODE==SB_MODE)
			if(rf_pri_sb_packet_crc_ok(rx_packet))
#endif

			{
				gpio_toggle(LED4);
				rx_cnt++;
//				delay_ms(100);
			}
			rf_clr_irq_status(FLD_RF_IRQ_RX );

		}

	}
#endif
#endif

	gpio_toggle(LED4);
	delay_ms(100);
}

#endif

#endif




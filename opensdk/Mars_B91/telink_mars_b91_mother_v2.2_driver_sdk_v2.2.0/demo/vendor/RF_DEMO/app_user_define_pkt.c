/********************************************************************************************************
 * @file    app_user_define_pkt.c
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
#if(MCU_CORE_B92)
#if RF_MODE == RF_USER_DEFINE_PKT
/**
 * @brief	Mainly introduce the package format of the custom package.
 * ble_tx_packet_format={DMA length(4byte),preamble(1~2byte),access code(4byte),Payload(2~258 byte),CRC(3byte:crc0,crc1,crc2)};
 * about crc calculate:
 * crc_init_val = reverse_32bit(0x555555);
 * crc_init_true =( crc_init_val >> 8)& 0xffffff;
 * crc_value = crc24(unsigned char * p,len,crc_init_true);//p:the address of Payload;len = n (payload length:2~258);crc_init_true
 * crc0 = crc_value & 0xff;
 * crc1 = (crc_value>>8) & 0xff;
 * crc2 = (crc_value>>16) & 0xff;
 */


unsigned char  rx_packet[128*4]  __attribute__ ((aligned (4)));
/*
 * @brief The preamble length in this custom package is 2 Bytes.
 */
unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) ={3,0,0,0,0xaa,0xaa,0x8e,0x89,0xbe,0xd6,0x00,0x0a,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0xb0,0x39,0x93};

/*
 * @brief The preamble length in this custom package is 1 Byte.
 */
unsigned char  ble_tx_packet_1_preamble[48] __attribute__ ((aligned (4))) ={3,0,0,0,0xaa,0x8e,0x89,0xbe,0xd6,0x00,0x0a,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0xb0,0x39,0x93};

/*
 * @brief Custom single carrier package structure.
 */
//unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define TX					1
#define RX					2
#define RF_TRX_MODE			RX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		MANUAL

#define RF_RX_IRQ_EN		0

#define TX_PKT_PAYLOAD		12
#define TX_PREAMBLE_LEN		2
#define	ACCESS_CODE_LEN		4
#define TX_CRC_LEN			3
#define TX_DMA_BYTE_LEN		4

#define TX_CRC_OFFSET		(TX_PKT_PAYLOAD+TX_PREAMBLE_LEN+ACCESS_CODE_LEN+TX_DMA_BYTE_LEN)

#define RX_FIFO_NUM			4
#define RX_FIFO_DEP			128

#define RF_FREQ				17
#define ACCESS_CODE         0x8e89bed6//0x29417671//
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
volatile unsigned int crc_value;
volatile unsigned int crc_init_val;
volatile unsigned int crc_init_true;

#if(RF_RX_IRQ_EN)
_attribute_ram_code_sec_ void rf_irq_handler(void)
{


	if(rf_get_irq_status(FLD_RF_IRQ_RX))
	{
	#if(RF_AUTO_MODE == AUTO)
		unsigned char* raw_pkt = rf_get_rx_packet_addr(RX_FIFO_NUM,RX_FIFO_DEP,rx_packet);
		if(rf_ble_packet_crc_ok(raw_pkt))
		{
			rx_cnt++;
		    gpio_toggle(LED2);
		}
		rf_start_srx(stimer_get_tick());

	#else
	    if(rf_ble_packet_crc_ok(rx_packet))
	    {
	    	rx_cnt++;
	    	gpio_toggle(LED2);
	    }
	#endif
			rf_clr_irq_status(FLD_RF_IRQ_RX);



	}
	else
	{
		rf_clr_irq_status(0xffff);
	}

}
#endif

/**
* @brief       The function of this function is to flip the high and low bits of a 32bit number.
* @param[in]   x		TNumber to be flipped.
* @return      Flipped number;You can perform shift operations on the flipped numbers according to the number of bits you use.
*/
unsigned int reverse_32bit(volatile unsigned int x)
{
	x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
	x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
	x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));

	return((x >> 16) | (x << 16));
}

/**
* @brief       This function is used to calculate CRC24.
* @param[in]   p		The input data block for computation.
* @param[in]   len		The size of the input data block in bytes.
* @param[in]   crc_init	The CRC initial value needs to be reversed.
* @return      The updated crc32 value, based on the input supplied.
*/
unsigned int crc24 (unsigned char *p, unsigned int len, unsigned int crc_init)
{
	unsigned int crc = crc_init;

	for (unsigned int j=0; j<len; j++){
		unsigned char ds = p[j];
		for (unsigned int i=0; i<8; i++){

			int cond = (crc ^ ds ) & 1;
			crc >>= 1;
			if (cond)
			{
			 crc ^= 0xda6000;
			}

			ds = ds >> 1;
		}
	}
	 return crc;
}

#if(RF_AUTO_MODE == AUTO)
void user_init(void)
{
	rf_set_power_level(RF_POWER);
	rf_set_chn(RF_FREQ);
	crc_init_val = reverse_32bit(0x555555);
	crc_init_true =( crc_init_val >> 8)& 0xffffff;
	crc_value = crc24(&ble_tx_packet[10],12,crc_init_true);
#if(RF_TRX_MODE==TX)
	rf_ble1m_no_pn_tx_customer_mode_en();
	rf_set_tx_dma(2,128);
#elif(RF_TRX_MODE==RX)
	rf_access_code_comm(ACCESS_CODE);
	rf_set_rx_dma(rx_packet,RX_FIFO_NUM-1,RX_FIFO_DEP);
#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ15_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX);
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
	unsigned char rf_data_len = TX_PKT_PAYLOAD + TX_PREAMBLE_LEN + ACCESS_CODE_LEN + TX_CRC_LEN;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[11]= TX_PKT_PAYLOAD;
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;

	ble_tx_packet[TX_CRC_OFFSET] = crc_value & 0xff;
	ble_tx_packet[TX_CRC_OFFSET+1] = (crc_value>>8) & 0xff;
	ble_tx_packet[TX_CRC_OFFSET+2] = (crc_value>>16) & 0xff;
	rf_start_stx(ble_tx_packet,stimer_get_tick());

	while(1)
	{

		delay_ms(1);
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX)));
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		rf_start_stx(ble_tx_packet,stimer_get_tick());
		gpio_toggle(LED1);
		delay_ms(100);
		tx_cnt++;
	}


#elif(RF_TRX_MODE==RX)
#if(!RF_RX_IRQ_EN)

	rf_start_srx(stimer_get_tick());
	while(1)
	{
		if(rf_get_irq_status(FLD_RF_IRQ_RX))
		{
			unsigned char* raw_pkt = rf_get_rx_packet_addr(RX_FIFO_NUM,RX_FIFO_DEP,rx_packet);
			if(rf_ble_packet_crc_ok(raw_pkt))
			{
				gpio_toggle(LED1);
				rx_cnt++;
			}
				rf_clr_irq_status(FLD_RF_IRQ_RX);
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
	crc_init_val = reverse_32bit(0x555555);
	crc_init_true =( crc_init_val >> 8)& 0xffffff;
	crc_value = crc24(&ble_tx_packet[10],12,crc_init_true);

#if(RF_TRX_MODE==TX)
	rf_ble1m_no_pn_tx_customer_mode_en();
	rf_tx_hw_crc_dis();
	rf_set_tx_dma(2,128);
#elif(RF_TRX_MODE==RX)
	rf_access_code_comm(ACCESS_CODE);
	rf_set_rx_dma(rx_packet,RX_FIFO_NUM-1,RX_FIFO_DEP);

#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ15_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX);
	rf_set_rxmode();
	delay_us(85);  //Wait for calibration to stabilize

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

	unsigned char rf_data_len = TX_PKT_PAYLOAD + TX_PREAMBLE_LEN + ACCESS_CODE_LEN + TX_CRC_LEN;
	ble_tx_packet[10]=TX_PKT_PAYLOAD;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;

	ble_tx_packet[TX_CRC_OFFSET] = crc_value & 0xff;
	ble_tx_packet[TX_CRC_OFFSET+1] = (crc_value>>8) & 0xff;
	ble_tx_packet[TX_CRC_OFFSET+2] = (crc_value>>16) & 0xff;

	rf_set_txmode();
	delay_us(113);  //Wait for calibration to stabilize
	while(1)
	{
		delay_ms(1);
		rf_tx_pkt(ble_tx_packet);
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX)));
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		gpio_toggle(LED1);
		delay_ms(100);
		tx_cnt++;
	}


#elif(RF_TRX_MODE==RX)
#if(!RF_RX_IRQ_EN)


	rf_set_rxmode();
	delay_us(85);  //Wait for calibration to stabilize
	while(1)
	{
		if(rf_get_irq_status(FLD_RF_IRQ_RX))
		{
			if(rf_ble_packet_crc_ok(rx_packet))
			{
				gpio_toggle(LED1);
				rx_cnt++;
			}
			rf_clr_irq_status(FLD_RF_IRQ_RX);

		}

	}
#endif
#endif

	gpio_toggle(LED4);
	delay_ms(100);
}

#endif

#endif
#endif

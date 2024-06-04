/********************************************************************************************************
 * @file    app_ble_1m_hyper_length.c
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
#if RF_MODE == RF_BLE_1M_HYPER_LENGTH

unsigned char  rx_packet[512]  __attribute__ ((aligned (4)));
//unsigned char  rx_packet[1024*32]  __attribute__ ((aligned (4)));
unsigned char  ble_tx_packet[1024*32] __attribute__ ((aligned (4))) ={3,0,0,0,0,0x09,0x00,0x08,0x08,0x02,0x04,0x02,0x42,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xc,0xf};//for adv2


#define TX					1
#define RX					2
#define RF_TRX_MODE			TX

#define AUTO  				1
#define MANUAL				2
#define RF_AUTO_MODE 		AUTO

#define	ACL_HYPER_LENGTH 			1
#define ACL_AOA_HYPER_LRNGTH 		2
#define BIS_HYPER_LRNGTH 			3
#define CIS_HYPER_LRNGTH 			4
#define ADV1_HYPER_LENGTH			5
#define ADV1_AOA_HYPER_LENGTH		6
#define ADV2_HYPER_LENGTH			7
#define ADV2_AOA_HYPER_LENGTH		8
#define ADV2_AOA					9

#define PKT_TYPE					ACL_HYPER_LENGTH



#define PDU_TYPE					1
#define CTE_TYPE					0

#define RF_RX_IRQ_EN				0

#define TX_PKT_PAYLOAD_H	0xff
#define TX_PKT_PAYLOAD		0x0f

#define RX_FIFO_NUM			4
#define RX_FIFO_DEP			1024


#define RF_FREQ				17
#define ACCESS_CODE        0xf8118ac9//0Xfcaab2c1//0x29417671//
volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;

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
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)
#endif

#if(RF_AUTO_MODE == AUTO)
void user_init(void)
{
	rf_set_power_level(RF_POWER);
	rf_set_chn(RF_FREQ);
	rf_access_code_comm(ACCESS_CODE);
	rf_aoa_aod_sample_point_adjust(0x65);
//	rf_aoa_rx_ant_switch_point_adjust(0x86);
#if(PKT_TYPE == ACL_HYPER_LENGTH || PKT_TYPE == ACL_AOA_HYPER_LRNGTH || PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)
	rf_set_tx_pkt_len_mode(RF_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_HYPER_LENGTH_PACKET);
#elif(PKT_TYPE == ADV1_HYPER_LENGTH || PKT_TYPE == ADV1_AOA_HYPER_LENGTH)
	rf_set_tx_pkt_len_mode(RF_ADV1_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_ADV1_HYPER_LENGTH_PACKET);
#elif(PKT_TYPE == ADV2_HYPER_LENGTH|| PKT_TYPE == ADV2_AOA_HYPER_LENGTH || ADV2_AOA)
	rf_set_tx_pkt_len_mode(RF_ADV2_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_ADV2_HYPER_LENGTH_PACKET);
#endif


#if(PKT_TYPE == ACL_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ACL_AOA_HYPER_LRNGTH)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#elif(PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)//This mode does not support AOA function temporarily.
	rf_set_bis_cis_en();
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV1_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV1_AOA_HYPER_LENGTH)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#elif(PKT_TYPE == ADV2_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV2_AOA_HYPER_LENGTH || PKT_TYPE == ADV2_AOA)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#endif

#if(RF_TRX_MODE==TX)
	rf_set_tx_dma(0,128);
#elif(RF_TRX_MODE==RX)
	rf_set_rx_dma(rx_packet,0,RX_FIFO_DEP);
//	rf_set_baseband_trans_unit(RF_DWORLD_WIDTH);
#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_ZB_RT);
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

//rf_data_len = payload_len + header_len + Extended_header_len
void main_loop(void)
{
#if(RF_TRX_MODE==TX)

#if(PKT_TYPE == ACL_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;
	ble_tx_packet[4]=0x80;//HE == 1;Expanded 1byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#elif(PKT_TYPE == ACL_AOA_HYPER_LRNGTH)// HE ==1 and CP == 1;
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;
	ble_tx_packet[4]=0xa0;//HE == 1 and CP == 1;Expanded 2byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#if (CTE_TYPE == 0)
	ble_tx_packet[7]=0x02;//Contents of CTEifo(AOA CTE)
#elif(CTE_TYPE == 1)
	ble_tx_packet[7]=0x42;//Contents of CTEifo(AOD CTE with 1us slot)
#elif(CTE_TYPE == 2)
	ble_tx_packet[7]=0x82;//Contents of CTEifo(AOD CTE with us slot)
#endif

#elif(PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)
#if((TX_PKT_PAYLOAD_H & 0x80) != 0x80)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;
	ble_tx_packet[4]=0x80;//HE == 1;Expanded 1byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;

#elif((TX_PKT_PAYLOAD_H & 0x80)== 0x80)//lble==1
	unsigned int rf_data_len = ((TX_PKT_PAYLOAD_H&0x7f)<< 8 | TX_PKT_PAYLOAD)+2+3;
	ble_tx_packet[4]=0x80;//HE == 1 and lble == 1;Expanded 3byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;

#endif
#elif(PKT_TYPE == ADV1_HYPER_LENGTH)

	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;//HE == 1
	ble_tx_packet[4]=0x10;//HE == 1;Expanded 1byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;

#elif(PKT_TYPE == ADV1_AOA_HYPER_LENGTH)

	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<<8| TX_PKT_PAYLOAD)+2+1;
#if(PDU_TYPE == 0)
	ble_tx_packet[4]=0x17;//PDU_Type == 0x07;Expanded 1byte header
#elif(PDU_TYPE == 1 )
	ble_tx_packet[4]=0x18;//PDU_Type == 0x07;Expanded 1byte header
#endif
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
	ble_tx_packet[7] = 0x02;//Expand the length of the header according to the protocol.
	ble_tx_packet[8] = 0x04;//The header of the first byte of the extension determines that the content of the extension is CTEifo.
#if (CTE_TYPE == 0)
	ble_tx_packet[9]=0x02;//Contents of CTEifo
#elif(CTE_TYPE == 1)
	ble_tx_packet[9]=0x42;
#elif(CTE_TYPE == 2)
	ble_tx_packet[9]=0x82;
#endif
#elif(PKT_TYPE == ADV2_HYPER_LENGTH)//adv2 aoa or aod hyper length
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;

	ble_tx_packet[4]= reg_rf_aux_hl_adv;//PDU_Type == aux_hl_adv;Expanded 2byte header
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
	ble_tx_packet[7]=0x00;
#elif(PKT_TYPE == ADV2_AOA_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;
	ble_tx_packet[4]=reg_rf_aux_hl_adv;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#if(PDU_TYPE == 0)
	ble_tx_packet[7]=0x70;//PDU_Type == 0x07;
#elif(PDU_TYPE == 1 )
	ble_tx_packet[7]=0x80;////PDU_Type == 0x08;
#endif
	ble_tx_packet[8]=0x02;//Expand the length of the header according to the protocol.
	ble_tx_packet[9]=0x04;//The header of the first byte of the extension determines that the content of the extension is CTEifo.
#if (CTE_TYPE == 0)
	ble_tx_packet[10]=0x02;//Contents of CTEifo(AOA CTE)
#elif(CTE_TYPE == 1)
	ble_tx_packet[10]=0x42;//Contents of CTEifo(AOD CTE with 1us slot)
#elif(CTE_TYPE == 2)
	ble_tx_packet[10]=0x82;//Contents of CTEifo(AOD CTE with us slot)
#endif

#elif(PKT_TYPE == ADV2_AOA)
	unsigned int rf_data_len = TX_PKT_PAYLOAD + 2;
	ble_tx_packet[4]=0x07;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=0x02;
	ble_tx_packet[7]=0x04;
	ble_tx_packet[8]=0x02;

#endif

	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;

	rf_start_stx(ble_tx_packet,stimer_get_tick());

	while(1)
	{

		delay_ms(1);
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX)));
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		rf_start_stx(ble_tx_packet,stimer_get_tick());
		gpio_toggle(LED1);
		gpio_toggle(LED2);
		gpio_toggle(LED3);
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
			if(!rf_get_crc_err())
			{
				gpio_toggle(LED1);
				gpio_toggle(LED3);
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
	rf_access_code_comm(ACCESS_CODE);
	rf_aoa_aod_sample_point_adjust(0x65);
	rf_aoa_rx_ant_switch_point_adjust(0x86);
#if(PKT_TYPE == ACL_HYPER_LENGTH || PKT_TYPE == ACL_AOA_HYPER_LRNGTH || PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)
	rf_set_tx_pkt_len_mode(RF_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_HYPER_LENGTH_PACKET);
#elif(PKT_TYPE == ADV1_HYPER_LENGTH || PKT_TYPE == ADV1_AOA_HYPER_LENGTH)
	rf_set_tx_pkt_len_mode(RF_ADV1_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_ADV1_HYPER_LENGTH_PACKET);
#elif(PKT_TYPE == ADV2_HYPER_LENGTH|| PKT_TYPE == ADV2_AOA_HYPER_LENGTH || ADV2_AOA)
	rf_set_tx_pkt_len_mode(RF_ADV2_HYPER_LENGTH_PACKET);
	rf_set_rx_pkt_len_mode(RF_ADV2_HYPER_LENGTH_PACKET);
#endif


#if(PKT_TYPE == ACL_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ACL_AOA_HYPER_LRNGTH)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#elif(PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)//This mode does not support AOA function temporarily.
	rf_set_bis_cis_en();
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV1_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV1_AOA_HYPER_LENGTH)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#elif(PKT_TYPE == ADV2_HYPER_LENGTH)
	rf_aoa_aod_set_trx_mode(RF_AOA_AOD_OFF);
#elif(PKT_TYPE == ADV2_AOA_HYPER_LENGTH || PKT_TYPE == ADV2_AOA)
	rf_aoa_aod_sample_interval_time(SAMPLE_NORMAL_INTERVAL);
	rf_aoa_aod_iq_data_mode(IQ_20_BIT_MODE);
	rf_aoa_aod_set_trx_mode(RF_RX_ACL_AOA_AOD_EN|RF_TX_ACL_AOA_AOD_EN);
#endif


#if(RF_TRX_MODE==TX)
	rf_set_tx_dma(2,128);
#elif(RF_TRX_MODE==RX)
	rf_set_rx_dma(rx_packet,RX_FIFO_NUM-1,RX_FIFO_DEP);
#if(RF_RX_IRQ_EN)

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_ZB_RT);
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

#if(PKT_TYPE == ACL_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;
	ble_tx_packet[4]=0x80;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#elif(PKT_TYPE == ACL_AOA_HYPER_LRNGTH)// HE ==1 and CP == 1;
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;
	ble_tx_packet[4]=0xa0;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
	ble_tx_packet[7]=0x42;//CTE information of iqsample and switch (AOA or AOD)
#elif(PKT_TYPE == BIS_HYPER_LRNGTH || PKT_TYPE == CIS_HYPER_LRNGTH)

#if((TX_PKT_PAYLOAD_H & 0x80) != 0x80)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;
	ble_tx_packet[4]=0x80;//HE == 1
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#elif((TX_PKT_PAYLOAD_H & 0x80)== 0x80)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+3;
	ble_tx_packet[4]=0x80;//HE == 1
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#endif

#elif(PKT_TYPE == ADV1_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+1;//HE == 1
	ble_tx_packet[4]=0x10;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#elif(PKT_TYPE == ADV1_AOA_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<<8| TX_PKT_PAYLOAD)+2+1;

#if(PDU_TYPE == 0)
	ble_tx_packet[4]=0x17;
#elif(PDU_TYPE == 1 )
	ble_tx_packet[4]=0x18;
#endif

	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
	ble_tx_packet[7] = 0x02;//Expand the length of the header according to the protocol.
	ble_tx_packet[8] = 0x04;//The header of the first byte of the extension determines that the content of the extension is CTEifo.

#if (CTE_TYPE == 0)
	ble_tx_packet[9]=0x02;//Contents of CTEifo
#elif(CTE_TYPE == 1)
	ble_tx_packet[9]=0x42;
#elif(CTE_TYPE == 2)
	ble_tx_packet[9]=0x82;
#endif

#elif(PKT_TYPE == ADV2_HYPER_LENGTH)//adv2 aoa or aod hyper length
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;
	ble_tx_packet[4]=reg_rf_aux_hl_adv;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
	ble_tx_packet[7]=0x00;
#elif(PKT_TYPE == ADV2_AOA_HYPER_LENGTH)
	unsigned int rf_data_len = (TX_PKT_PAYLOAD_H<< 8 | TX_PKT_PAYLOAD)+2+2;
	ble_tx_packet[4]=reg_rf_aux_hl_adv;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=TX_PKT_PAYLOAD_H;
#if(PDU_TYPE == 0)
	ble_tx_packet[7]=0x70;
#elif(PDU_TYPE == 1 )
	ble_tx_packet[7]=0x80;
#endif
	ble_tx_packet[8]=0x02;//Expand the length of the header according to the protocol.
	ble_tx_packet[9]=0x04;//The header of the first byte of the extension determines that the content of the extension is CTEifo.
#if (CTE_TYPE == 0)
	ble_tx_packet[10]=0x02;//Contents of CTEifo
#elif(CTE_TYPE == 1)
	ble_tx_packet[10]=0x42;
#elif(CTE_TYPE == 2)
	ble_tx_packet[10]=0x82;
#endif

#elif(PKT_TYPE == ADV2_AOA)//Compatible with the functions of the old version (without length extension)
	unsigned int rf_data_len = TX_PKT_PAYLOAD + 2;
	ble_tx_packet[4]=0x07;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	ble_tx_packet[6]=0x02;
	ble_tx_packet[7]=0x04;
	ble_tx_packet[8]=0x02;
#endif

	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;

	rf_set_txmode();
	delay_us(113);//Wait for calibration to stabilize

	while(1)
	{
		delay_ms(100);
		rf_tx_pkt(ble_tx_packet);
		while(!(rf_get_irq_status(FLD_RF_IRQ_TX)));
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		gpio_toggle(LED1);
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

			if(!rf_get_crc_err())
			{
				for(int i = 0;i<16;i++)
				{
					int j;
					j=(rx_packet[0]|(rx_packet[1]<<8))-4+i;
					check_packet[i]=rx_packet[j];
				}

				gpio_toggle(LED1);
				rx_cnt++;
				delay_ms(100);
				while(1);
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

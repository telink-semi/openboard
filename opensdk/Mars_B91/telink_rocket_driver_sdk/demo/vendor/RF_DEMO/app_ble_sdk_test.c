/********************************************************************************************************
 * @file    app_ble_sdk_test.c
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

#if (RF_MODE == RF_BLE_SDK_TEST)

#define TEST_CHN   				1		// 37/38/39 adv channel

#define BLE_ACCESS_CODE			0x9A3CC36A//0xA5CC336A//0xd6be898e//


#define TEST_MAC				0xEE

#define TX_INTERVAL_TICK		(10*SYSTEM_TIMER_TICK_1MS)   //10ms
#define TX_INTERVAL_US			(TX_INTERVAL_TICK/16)


#define TEST_MODE_MANUAL_TX		1
#define TEST_MODE_STX			2
#define TEST_MODE_BTX_TX		3  //test TX only, do not care RX of BTX
#define TEST_MODE_BTX			4

#define TEST_MODE_MANUAL_RX	    10
#define TEST_MODE_SRX	    	11
#define TEST_MODE_BRX_RX	    12	//test RX only, do not care TX of BRX
#define TEST_MODE_BRX	    	13

#define TEST_MODE_SELECT		TEST_MODE_BRX_RX


volatile unsigned int tx_cnt = 0;

volatile unsigned int rx_cnt = 0;

unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) ={3,0,0,0,0,10,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xc,0xf};


#define DEBUG_GPIO_ENABLE							1

#if(DEBUG_GPIO_ENABLE)

	#define GPIO_CHN0							GPIO_PB0
	#define GPIO_CHN1							GPIO_PB1
	#define GPIO_CHN2							GPIO_PB6
	#define GPIO_CHN3							GPIO_PB7

	#define DBG_CHN0_LOW		gpio_set_level(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_set_level(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_set_level(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_set_level(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_set_level(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_set_level(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_set_level(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_set_level(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_set_level(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_set_level(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_set_level(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_set_level(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_set_level(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_set_level(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)

#else

	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE

#endif






void ble_manual_tx_test(void);
void ble_stx_test(void);
void ble_btx_tx_test(void);

void ble_manual_rx_test(void);
void ble_brx_rx_test(void);

typedef enum advChannelPDUType_e {
	LL_TYPE_ADV_IND = 0x00,
	LL_TYPE_ADV_DIRECT_IND = 0x01,
	LL_TYPE_ADV_NONCONN_IND = 0x02,
	LL_TYPE_SCAN_REQ = 0x03,
	LL_TYPE_SCAN_RSP = 0x04,
	LL_TYPE_CONNECT_REQ = 0x05,
	LL_TYPE_ADV_SCAN_IND = 0x06,
} advChannelPDUType_t;


typedef struct {
	unsigned char type   :4;
	unsigned char rfu1   :2;
	unsigned char txAddr :1;
	unsigned char rxAddr :1;
}rf_adv_head_t;

typedef struct{
	unsigned long dma_len;            //won't be a fixed number as previous, should adjust with the mouse package number

	rf_adv_head_t  header;	//RA(1)_TA(1)_RFU(2)_TYPE(4)
	unsigned char  rf_len;				//LEN(6)_RFU(2)

	unsigned char	advA[6];			//address
	unsigned char	data[31];			//0-31 byte
}rf_packet_adv_t;

/*
 * app_rf_test.c
 *
 *  Created on: 2018-1-11
 *      Author: Administrator
 */


rf_packet_adv_t	debug_pkt_adv = {
		rf_tx_packet_dma_len(sizeof (rf_packet_adv_t) - 4),		// dma_len
		{LL_TYPE_ADV_NONCONN_IND, 0, 0, 0},					// type
		sizeof (rf_packet_adv_t) - 6,		// rf_len
		{TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC, TEST_MAC},	// advA
		// data
		{0},
};

unsigned char debug_pkt_adv_2[30] = { 10,0,0,0,  0, 8, TEST_MAC,TEST_MAC,TEST_MAC,TEST_MAC,TEST_MAC, TEST_MAC,0x01};






void user_init(void)
{
#if(DEBUG_GPIO_ENABLE)
	gpio_function_en(GPIO_CHN0);
	gpio_function_en(GPIO_CHN1);
	gpio_function_en(GPIO_CHN2);
	gpio_function_en(GPIO_CHN3);

	gpio_output_en(GPIO_CHN0);
	gpio_output_en(GPIO_CHN1);
	gpio_output_en(GPIO_CHN2);
	gpio_output_en(GPIO_CHN3);
#endif
	unsigned char rf_data_len = 0x25+2;
	ble_tx_packet[4]=0;
	ble_tx_packet[5]=0x25;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;
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

void main_loop (void)
{

#if( TEST_MODE_SELECT == TEST_MODE_STX)
	ble_stx_test();
#elif ( TEST_MODE_SELECT == TEST_MODE_BTX_TX)
	ble_btx_tx_test();
#elif ( TEST_MODE_SELECT == TEST_MODE_MANUAL_TX)
	ble_manual_tx_test();
#elif (TEST_MODE_SELECT == TEST_MODE_MANUAL_RX)
	ble_manual_rx_test();
#elif (TEST_MODE_SELECT == TEST_MODE_BRX_RX)
	ble_brx_rx_test();
#endif
}




void ble_stx_test(void)
{
	rf_set_power_level (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_access_code_comm(BLE_ACCESS_CODE);

	rf_set_tx_dma(2,128);

	unsigned long tx_begin_tick;

	while(1)
	{

		tx_begin_tick = stimer_get_tick();

		rf_set_tx_rx_off_auto_mode();
		rf_set_ble_chn (TEST_CHN);  //2402

		debug_pkt_adv.data[0] ++;


		rf_start_stx ((void *)&debug_pkt_adv, stimer_get_tick() + 100);

		delay_us(2000);  //2mS is enough for packet sending

		if(rf_get_irq_status(FLD_RF_IRQ_TX))
		{
			rf_clr_irq_status(FLD_RF_IRQ_TX);
			gpio_toggle(LED4);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}

}

void ble_manual_tx_test()
{

	rf_set_power_level (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_access_code_comm(BLE_ACCESS_CODE);

	rf_set_ble_chn (TEST_CHN);  //2402

	rf_set_tx_dma(2,128);

	rf_set_txmode();

	delay_us(113);

	reg_rf_irq_status = 0xffff;

	unsigned long tx_begin_tick;

	while(1)
	{

		tx_begin_tick = stimer_get_tick();

		debug_pkt_adv.data[0] ++;


		rf_tx_pkt ((void *)&debug_pkt_adv);

		delay_us(2000);  //2mS is enough for packet sending

		if(rf_get_irq_status(FLD_RF_IRQ_TX))
		{
			rf_clr_irq_status(FLD_RF_IRQ_TX);
			tx_cnt++;
			gpio_toggle(LED4);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}

}


void ble_btx_tx_test()
{
	rf_set_power_level (RF_POWER);

	rf_set_ble_crc_adv ();

	rf_access_code_comm(BLE_ACCESS_CODE);

	rf_set_tx_dma(2,128);

	unsigned long tx_begin_tick;

	while(1)
	{

		tx_begin_tick = stimer_get_tick();

		rf_set_tx_rx_off_auto_mode();
		rf_set_ble_chn (TEST_CHN);  //2402

		debug_pkt_adv.data[0] ++;


		rf_start_btx ((void *)&debug_pkt_adv, stimer_get_tick() + 100);

		delay_us(2000);  //2mS is enough for packet sending

		rf_set_tx_rx_off_auto_mode();

		if(rf_get_irq_status(FLD_RF_IRQ_TX))
		{
			rf_clr_irq_status(FLD_RF_IRQ_TX);
			tx_cnt++;
			gpio_toggle(LED1);
			gpio_toggle(LED2);
			DBG_CHN0_TOGGLE;
		}

		while(!clock_time_exceed(tx_begin_tick, TX_INTERVAL_US) );   //10mS
	}
}



#define				BLE_LL_BUFF_SIZE		80
unsigned char  		blt_rxbuffer[BLE_LL_BUFF_SIZE*4] __attribute__((aligned(4)));
int					blt_rx_wptr = 0;



int AA_rx_irq_cnt = 0;
int AA_rx_crc_ok_cnt = 0;
int AA_rx_match_cnt = 0;

int AA_rx_match_flag;
int AA_rx_miss_cnt;
int AA_rx_miss_rate;
int AA_rx_set_number = 0;





volatile int brx_mode_flag = 0;

unsigned long bltc_tick_1st_rx = 0;
unsigned long bltc_connExpectTime = 0;
int bltParam_conn_rx_num = 0;

volatile 	int   RX_rcvd_tick;
volatile	int	  RX_rcvd_flag = 0;

unsigned long blt_timeStamp;
unsigned long blt_tick_now;


unsigned char	blt_tx_empty_packet[6] = {2, 0, 0, 0, 1, 0};




_attribute_ram_code_sec_noinline_ void rf_irq_handler(void)
{

	 if(rf_get_irq_status(FLD_RF_IRQ_RX))
	 {

		blt_tick_now = stimer_get_tick();

		DBG_CHN0_TOGGLE;

		unsigned char * raw_pkt = (unsigned char *) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE);
		blt_rx_wptr = (blt_rx_wptr + 1) & 3;
		unsigned char * next_pkt_addr = (unsigned char *) (blt_rxbuffer + blt_rx_wptr * BLE_LL_BUFF_SIZE);
		rf_set_rx_buffer(next_pkt_addr);

		rf_clr_irq_status(FLD_RF_IRQ_RX);

		AA_rx_irq_cnt ++;

		if	(rf_ble_packet_crc_ok(blt_rxbuffer) )  //CRC OK
		{
			DBG_CHN1_TOGGLE;
			AA_rx_crc_ok_cnt ++;
			rx_cnt++;
			gpio_toggle(LED2);
			#if (TEST_MODE_SELECT == TEST_MODE_BRX_RX)
				blt_timeStamp = reg_rf_timestamp;

				if(!brx_mode_flag)
				{
					bltParam_conn_rx_num = 0;
				}

				if(!bltc_tick_1st_rx && !bltParam_conn_rx_num && (unsigned long)(blt_tick_now - blt_timeStamp) < SYSTEM_TIMER_TICK_1MS)
				{
					bltc_tick_1st_rx = blt_timeStamp;
					if(!brx_mode_flag)
					{
						brx_mode_flag = 1;
						bltc_connExpectTime = blt_timeStamp + TX_INTERVAL_TICK;
					}
				}
			#endif

		}


		raw_pkt[0] = 1;  //must
	 }

	bltParam_conn_rx_num ++;

	if(rf_get_irq_status(FLD_RF_IRQ_TX))
	{
		 rf_clr_irq_status(FLD_RF_IRQ_TX);
	}

}
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)







_attribute_ram_code_sec_noinline_ void ble_manual_rx_test(void)
{
	rf_set_rx_dma(blt_rxbuffer,4-1,128);

	rf_set_power_level (RF_POWER);
	rf_set_ble_crc_adv ();


	rf_access_code_comm(BLE_ACCESS_CODE);


	rf_set_ble_chn (TEST_CHN);  //37:2402;   8:2420

	rf_clr_irq_status(FLD_RF_IRQ_RX);

	rf_set_rxmode ();
	delay_us(85);

	core_interrupt_enable();
	plic_interrupt_enable(IRQ_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX);

	while(1)
	{

	}


}





void ble_brx_rx_test(void)
{
	rf_set_rx_dma(blt_rxbuffer,4-1,128);

	rf_set_power_level (RF_POWER);
	rf_set_ble_crc_adv ();
	rf_access_code_comm(BLE_ACCESS_CODE);

	rf_clr_irq_status(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
	plic_interrupt_enable(IRQ_ZB_RT);
	rf_set_irq_mask(FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
	brx_mode_flag = 0;

	core_interrupt_enable();

//enter manual RX first, get peer TX device timeStamp
	rf_set_ble_chn(TEST_CHN);
	rf_set_rxmode ();
	while(!brx_mode_flag)
	{

	}
	rf_set_tx_rx_off();  //stop manual RX
//manual RX exit, timeStamp capture OK

	AA_rx_match_cnt = 0;
	AA_rx_set_number = 0;

	while(1)
	{

		// 2mS before peer device TX
		while( (unsigned long)(stimer_get_tick() - (bltc_connExpectTime - 2 * SYSTEM_TIMER_TICK_1MS)) > BIT(30) );

		bltc_tick_1st_rx = 0;
		bltParam_conn_rx_num = 0;
		AA_rx_match_flag = 0;

		rf_set_ble_chn(TEST_CHN);
		rf_start_brx ((void *)blt_tx_empty_packet, stimer_get_tick () + 100);   //BRX

		DBG_CHN2_HIGH;

		delay_us(4 * 1000);  //4mS RX duration

		DBG_CHN2_LOW;

		AA_rx_set_number ++;
		AA_rx_miss_rate = AA_rx_miss_cnt*1000/AA_rx_set_number;

		//stop CRX
		core_interrupt_disable();
		rf_set_tx_rx_off_auto_mode();
		delay_us(100);
		rf_clr_irq_status( FLD_RF_IRQ_RX | FLD_RF_IRQ_TX);
		core_interrupt_enable();

		//timing control
		if(bltc_tick_1st_rx)
		{
			bltc_connExpectTime = bltc_tick_1st_rx + TX_INTERVAL_TICK;
		}
		else
		{
			bltc_connExpectTime += TX_INTERVAL_TICK;
		}

	}



}
#endif

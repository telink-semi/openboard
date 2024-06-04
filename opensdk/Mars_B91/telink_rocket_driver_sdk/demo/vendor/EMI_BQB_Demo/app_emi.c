/********************************************************************************************************
 * @file    app_emi.c
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
#if (TEST_DEMO == EMI_DEMO)
#include "calibration.h"
#include "PA/pa.h"


/*
 * @brief 	this macro definition serve to open the setting to deal with problem of zigbee mode 2480Mhz
 * 			band edge can't pass the spec.only use it at the time of certification.
 * */
#define	FIX_ZIGBEE_BANDEDGE_EN	0

#if !EMI_SUPPORT_SETTING

/*
 * @brief 	this macro definition serve to close internal cap.
 * */
#define	CLOSE_INTERNAL_CAP_EN	0//0:Internal capacitance,1:External capacitance

/*
 * @brief 	this macro definition serve to enable function of swire-through-usb.
 * */
#define	SWIRE_THROUGH_USB_EN	0//0:disable swire-through-usb,1:enable swire-through-usb.

#endif

#define TX_PACKET_MODE_ADDR 		     0x00000005
#define RUN_STATUE_ADDR 			     0x00000006
#define TEST_COMMAND_ADDR			     0x00000007
#define POWER_ADDR 					     0x00000008
#define CHANNEL_ADDR				     0x00000009
#define RF_MODE_ADDR				     0x0000000a
#define CD_MODE_HOPPING_CHN			     0x0000000b
#define RSSI_ADDR                        0x00000004
#define RX_PACKET_NUM_ADDR               0x0000000c

#define MAX_RF_CHANNEL                   40

/**
 * @brief   The emi test command and function
 */
typedef struct {
	unsigned char  cmd_id; 										/**< emi command id */
	void	 (*func)(rf_mode_e, unsigned char , signed char);   /**< emi function */
}test_list_t;

/**
 * @brief   The emi test mode.
 */
typedef enum{
	ble2m = 0,
	ble1m = 1,
	zigbee250k = 2,
	ble125K = 3,
	ble500K = 4,
	pri2m = 5,
	pri1m = 6,
}emi_rf_mode_e;

/**
 * @brief   Init the emi frequency hopping channel
 */
const unsigned char rf_chn_hipping[MAX_RF_CHANNEL] = {
	48,  4, 66, 62, 44,
	12, 38, 16 ,26, 20,
	22, 68, 18, 28,	42,
	32, 34, 36, 14,	40,
	30, 54, 46, 2,	50,
	52, 80, 56, 78,	74,
	8,  64, 6,  24,	70,
	72, 60, 76, 58, 10,
};

/**
 * @brief   Init the emi reference parameter
 */
unsigned char  g_power_level = 0;  // 0
unsigned char  g_chn = 2;          // 2
unsigned char  g_cmd_now = 1;      // 1
unsigned char  g_run = 1;          // 1
unsigned char  g_hop = 0;          // 0
unsigned char  g_tx_cnt = 0;       // 0
#if(MCU_CORE_B92)
unsigned char  g_pkt_tone = 0;		//0
#endif
emi_rf_mode_e  g_mode = ble1m;     // 1

#if EMI_SUPPORT_SETTING
/**
* @brief global variable to save EMI initial setting
*/
volatile emi_setting_t g_emi_setting;

#endif

void emi_init(void);
void emicarrieronly(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_prbs9(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emirx(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emitxprbs9(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emitx55(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emitx0f(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx55(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx0f(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emitxaa(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void emitxf0(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
#if(MCU_CORE_B92)
void rf_emi_tx_current_test(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
void rf_emi_rx_current_test(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn);
#endif
/**
 * @brief   Init the structure of the emi test command and function
 */
test_list_t  ate_list[] = {
		{0x01,emicarrieronly},
		{0x02,emi_con_prbs9},
		{0x03,emirx},
		{0x04,emitxprbs9},
		{0x05,emitx55},
		{0x06,emitx0f},
		{0x07,emi_con_tx55},
		{0x08,emi_con_tx0f},
		{0x0d,emitxaa},
		{0x0f,emitxf0},
#if(MCU_CORE_B92)
		{0x10,rf_emi_tx_current_test},
		{0x11,rf_emi_rx_current_test}
#endif
};

#if(MCU_CORE_B92)
/**
 * @brief      This function serves to test RF tx current
 * @param[in]  rf_mode     - mode of RF.
 * @param[in]  pwr         - power level of RF.
 * @param[in]  rf_chn      - channel of RF.
 * @return     none
 */
void rf_emi_tx_current_test(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	(void)(rf_mode);
	extern void rf_set_power_level_index_singletone (rf_power_level_e level);
	extern unsigned char g_single_tong_freqoffset;
	rf_mode_init();
	rf_power_level_e  power = rf_power_Level_list[pwr];
	g_single_tong_freqoffset = 1;
	rf_set_chn(rf_chn);
	g_single_tong_freqoffset = 0;
	rf_set_power_level_index_singletone(power);
	rf_set_txmode();
	gpio_shutdown(GPIO_ALL);
	rf_current_test_cfg();
	while(1);
}

/**
 * @brief      This function serves to test RF rx current
 * @param[in]  rf_mode     - mode of RF.
 * @param[in]  pwr         - power level of RF.
 * @param[in]  rf_chn      - channel of RF.
 * @return     none
 */
void rf_emi_rx_current_test(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	(void)rf_mode;
	(void)pwr;
	rf_emi_rx_setup(RF_MODE_BLE_1M_NO_PN, rf_chn);
	gpio_shutdown(GPIO_ALL);
	rf_current_test_cfg();
	while(1);
}
#endif

void read_calibration_flash(void)
{
#if (MCU_CORE_B91||MCU_CORE_B92) //Wait for the B92 calibration function to be added before changing here
	unsigned char flash_type = (flash_read_mid() >> 16) & 0xff;
	switch (flash_type)
	{
		case FLASH_SIZE_64K:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_64K);
			break;
		case FLASH_SIZE_128K:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_128K);
			break;
		case FLASH_SIZE_512K:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_512K);
			break;
		case FLASH_SIZE_1M:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_1M);
			break;
		case FLASH_SIZE_2M:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_2M);
			break;
		case FLASH_SIZE_4M:
			user_calib_freq_offset(USER_CALIB_FROM_FLASH, FLASH_CAP_VALUE_ADDR_4M);
			break;
		default:
			break;
	}
#endif
}

/**
 * @brief		This function serves to EMI Init
 * @return 		none
 */

void emi_init(void)
{
	read_calibration_flash();

#if EMI_SUPPORT_SETTING
	if(g_emi_setting.general_setting.cap)
	{
		rf_turn_off_internal_cap();
	}
#else

#if CLOSE_INTERNAL_CAP_EN
	rf_turn_off_internal_cap();
#endif

#endif

	rf_access_code_comm(EMI_ACCESS_CODE);             // access code
#if(MCU_CORE_B91)
	write_sram8(TX_PACKET_MODE_ADDR,g_tx_cnt); 		 // tx_cnt,pkt+tone
#elif(MCU_CORE_B92)
	write_sram8(TX_PACKET_MODE_ADDR,g_tx_cnt|(g_pkt_tone<<7)); // tx_cnt,pkt+tone
#endif
    write_sram8(RUN_STATUE_ADDR,g_run);               // run
    write_sram8(TEST_COMMAND_ADDR,g_cmd_now);         // cmd
    write_sram8(POWER_ADDR,g_power_level);            // power
    write_sram8(CHANNEL_ADDR,g_chn);                  // chn
    write_sram8(RF_MODE_ADDR,g_mode);                 // mode
    write_sram8(CD_MODE_HOPPING_CHN,g_hop);           // hop
    write_sram8(RSSI_ADDR,0);                         // rssi
    write_sram32(RX_PACKET_NUM_ADDR,0);               // rx_packet_num
}

/**
 * @brief		This function serves to EMI CarryOnly
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */

void emicarrieronly(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{

	pa_operation(PA_SETTING_STATE_TX);
	(void)(rf_mode);
#if(MCU_CORE_B92)
	unsigned int t0 = reg_system_tick,chnidx = 1;
#endif
	rf_power_level_e  power = rf_power_Level_list[pwr];
	g_hop = read_sram8(CD_MODE_HOPPING_CHN);
	rf_emi_tx_single_tone(power,rf_chn);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
#if(MCU_CORE_B92)
		if(g_hop)
		{
			trng_init();
			while(!clock_time_exceed(t0,10000))    // run 10ms
				rf_emi_tx_single_tone(power,chnidx);

			while(!clock_time_exceed(t0,20000));   // stop 20ms
			t0 = reg_system_tick;
			rf_emi_stop();
			chnidx = (trng_rand()&0x7f);
			if(chnidx>80)
				chnidx -= 80;
		}
#endif
	}
	rf_emi_stop();
	rf_set_tx_rx_off();
	dma_reset();
	rf_emi_reset_baseband();
}

/**
 * @brief		This function serves to EMI ConPrbs9
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr 	- power level of RF.
 * @param[in]   rf_chn 	- channel of RF.
 * @return 		none
 */
void emi_con_prbs9(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int t0 = reg_system_tick,chnidx = 1;
	rf_power_level_e power = rf_power_Level_list[pwr];
	g_hop = read_sram8(CD_MODE_HOPPING_CHN);
	rf_emi_tx_continue_update_data(rf_mode,power,rf_chn,0);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
#if FIX_ZIGBEE_BANDEDGE_EN
		if(rf_mode == RF_MODE_ZIGBEE_250K)
		{
			if (rf_chn == 80)
			{
				write_reg8(0x140c23,0x00);
			}
			else
			{
				write_reg8(0x140c23,0x80);
			}
		}
#endif
		rf_continue_mode_run();
		if(g_hop)
		{
			while(!clock_time_exceed(t0,10000))    // run 10ms
				rf_continue_mode_run();

			while(!clock_time_exceed(t0,20000));   // stop 20ms
			t0 = reg_system_tick;
			rf_emi_tx_continue_update_data(rf_mode,power,rf_chn_hipping[chnidx - 1],0);
			(chnidx >= MAX_RF_CHANNEL)?(chnidx = 1):(chnidx++);
		}
	}
	rf_emi_stop();

	rf_set_tx_rx_off();
	dma_reset();
	rf_emi_reset_baseband();
}

/**
 * @brief		This function serves to EMI Rx
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr	    - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emirx(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_RX);
	(void)(pwr);
	rf_emi_rx_setup(rf_mode,rf_chn);
	write_sram8(RSSI_ADDR,0);
	write_sram32(RX_PACKET_NUM_ADDR,0);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_rx_loop();
		if(rf_emi_get_rxpkt_cnt() != read_sram32(RX_PACKET_NUM_ADDR))
		{
		    write_sram8(RSSI_ADDR,rf_emi_get_rssi_avg());
		    write_sram32(RX_PACKET_NUM_ADDR,rf_emi_get_rxpkt_cnt());
		}
	}
	rf_emi_stop();
}

/**
 * @brief		This function serves to EMI TxPrbs
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */

void emitxprbs9(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int tx_num = 0;
	rf_power_level_e  power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_tx_burst_loop(rf_mode,0);
		if(g_tx_cnt)
		{
			tx_num++;
			if(tx_num >= 1000)
				break;
		}
#if(MCU_CORE_B92)
		if(g_pkt_tone)
		{
			rf_set_tx_modulation_index(RF_MI_P0p00);
			rf_set_power_level_singletone(power);
			delay_us(40);
			rf_set_power_off_singletone();
			rf_set_tx_modulation_index(RF_MI_P0p50);
		}
#endif
	}
	rf_emi_stop();
}

/**
 * @brief		This function serves to EMI Tx55
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */

void emitx55(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int tx_num = 0;
	rf_power_level_e  power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_tx_burst_loop(rf_mode,2);
		if(g_tx_cnt)
		{
			tx_num++;
			if(tx_num >= 1000)
				break;
		}
#if(MCU_CORE_B92)
		if(g_pkt_tone)
		{
			rf_set_tx_modulation_index(RF_MI_P0p00);
			rf_set_power_level_singletone(power);
			delay_us(40);
			rf_set_power_off_singletone();
			rf_set_tx_modulation_index(RF_MI_P0p50);
		}
#endif
	}
	rf_emi_stop();
}

/**
 * @brief		This function serves to EMI Tx0f
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emitx0f(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int tx_num = 0;
	rf_power_level_e  power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_tx_burst_loop(rf_mode,1);
		if(g_tx_cnt)
		{
			tx_num++;
			if(tx_num >= 1000)
				break;
		}
#if(MCU_CORE_B92)
		if(g_pkt_tone)
		{
			rf_set_tx_modulation_index(RF_MI_P0p00);
			rf_set_power_level_singletone(power);
			delay_us(40);
			rf_set_power_off_singletone();
			rf_set_tx_modulation_index(RF_MI_P0p50);
		}
#endif
	}
	rf_emi_stop();
}
/**
 * @brief		This function serves to EMI Txaa
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emitxaa(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int tx_num = 0;
	rf_power_level_e  power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,3);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_tx_burst_loop(rf_mode,3);
		if(g_tx_cnt)
		{
			tx_num++;
			if(tx_num >= 1000)
				break;
		}
#if(MCU_CORE_B92)
		if(g_pkt_tone)
		{
			rf_set_tx_modulation_index(RF_MI_P0p00);
			rf_set_power_level_singletone(power);
			delay_us(40);
			rf_set_power_off_singletone();
			rf_set_tx_modulation_index(RF_MI_P0p50);
		}
#endif
	}
	rf_emi_stop();
}

/**
 * @brief		This function serves to EMI Txf0
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emitxf0(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	unsigned int tx_num = 0;
	rf_power_level_e  power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,4);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_emi_tx_burst_loop(rf_mode,4);
		if(g_tx_cnt)
		{
			tx_num++;
			if(tx_num >= 1000)
				break;
		}
#if(MCU_CORE_B92)
		if(g_pkt_tone)
		{
			rf_set_tx_modulation_index(RF_MI_P0p00);
			rf_set_power_level_singletone(power);
			delay_us(40);
			rf_set_power_off_singletone();
			rf_set_tx_modulation_index(RF_MI_P0p50);
		}
#endif
	}
	rf_emi_stop();
}



/**
 * @brief		This function serves to EMI_CON_TX55
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emi_con_tx55(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	rf_power_level_e power = rf_power_Level_list[pwr];
	rf_emi_tx_continue_update_data(rf_mode,power,rf_chn,2);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_continue_mode_run();
	}
	rf_emi_stop();
}

/**
 * @brief		This function serves to EMI_CON_TX0f
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emi_con_tx0f(rf_mode_e rf_mode,unsigned char pwr,signed char rf_chn)
{
	pa_operation(PA_SETTING_STATE_TX);
	rf_power_level_e power = rf_power_Level_list[pwr];
	rf_emi_tx_continue_update_data(rf_mode,power,rf_chn,1);
	while( ((read_sram8(RUN_STATUE_ADDR)) == g_run ) &&  ((read_sram8(TEST_COMMAND_ADDR)) == g_cmd_now )\
			&& ((read_sram8(POWER_ADDR)) == g_power_level ) &&  ((read_sram8(CHANNEL_ADDR)) == g_chn )\
			&& ((read_sram8(RF_MODE_ADDR)) == g_mode))
	{
		rf_continue_mode_run();
	}
	rf_emi_stop();
}

/**
 * @brief	  This function serves to EMI ServiceLoop
 * @return 	  none
 */
void emi_serviceloop(void)
{
   unsigned char i = 0;

   while(1)
   {
	   g_run = read_sram8(RUN_STATUE_ADDR);  // get the run state!
	   if(g_run != 0)
	   {
		   g_power_level = read_sram8(POWER_ADDR);
		   g_chn = read_sram8(CHANNEL_ADDR);
		   g_mode = read_sram8(RF_MODE_ADDR);
		   g_cmd_now = read_sram8(TEST_COMMAND_ADDR);  // get the command!
		   g_tx_cnt = (read_sram8(TX_PACKET_MODE_ADDR)&0x7f);
		   g_hop = read_sram8(CD_MODE_HOPPING_CHN);
#if(MCU_CORE_B92)
		   g_pkt_tone = (read_sram8(TX_PACKET_MODE_ADDR)&BIT(7));
#endif
		   pa_operation(PA_SETTING_STATE_INIT);

		   for (i = 0;i < sizeof(ate_list)/sizeof(test_list_t);i++)
		   {
				if(g_cmd_now == ate_list[i].cmd_id)
				{
					switch(g_mode)
					{
					  case ble2m:
						  ate_list[i].func(RF_MODE_BLE_2M,g_power_level,g_chn);
						  break;

					  case ble1m:
						  ate_list[i].func(RF_MODE_BLE_1M_NO_PN,g_power_level,g_chn);
						  break;

					  case zigbee250k:
						  ate_list[i].func(RF_MODE_ZIGBEE_250K,g_power_level,g_chn);
						  break;

					  case ble125K:
						  ate_list[i].func(RF_MODE_LR_S8_125K,g_power_level,g_chn);
						  break;

					  case ble500K:
						  ate_list[i].func(RF_MODE_LR_S2_500K,g_power_level,g_chn);
						  break;

					  case pri2m:
						  ate_list[i].func(RF_MODE_PRIVATE_2M,g_power_level,g_chn);
						  break;

					  case pri1m:
						  ate_list[i].func(RF_MODE_PRIVATE_1M,g_power_level,g_chn);
						  break;

					  default:break;
					}
				}
		   }
		   g_run = 0;
		   write_sram8(RUN_STATUE_ADDR,g_run);
	   }
   }
}

#if EMI_SUPPORT_SETTING

/**
 * @brief		This function serves to read EMI initial setting.
 * @return 		none
 */

void emi_setting_init(void)
{
	g_emi_setting.general_setting.ptr[0] = read_data8(GENERAL_SETTING_ADDR);
	g_emi_setting.pa_setting_pos = read_data32(PA_SETTING_ADDR);
}

#endif


/**
 * @brief		This function serves to User Init
 * @return 		none
 */

void user_init(void)
{

#if EMI_SUPPORT_SETTING
	pa_setting_init(g_emi_setting.pa_setting_pos, g_emi_setting.general_setting.pa_bypass_en);
	if(g_emi_setting.general_setting.swire_through_usb_en)
	{
		usb_set_pin_en();
		gpio_set_up_down_res(GPIO_DM, GPIO_PIN_PULLUP_10K);
	}
#else

#if SWIRE_THROUGH_USB_EN
	usb_set_pin_en();
	gpio_set_up_down_res(GPIO_DM, GPIO_PIN_PULLUP_10K);
#endif

#endif

	emi_init();
}

/**
 * @brief		This function serves to main
 * @return 		none
 */
void main_loop(void)
{
	emi_serviceloop();
}

#endif

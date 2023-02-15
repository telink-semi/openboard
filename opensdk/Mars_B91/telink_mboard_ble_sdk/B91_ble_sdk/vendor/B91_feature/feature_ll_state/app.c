/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_config.h"
#include "app.h"
#include "app_buffer.h"
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "../default_att.h"

#if (FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || \
	 FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE )


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P2p79dBm



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * SYSTEM_TIMER_TICK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio pad wakeup suspend/deepsleep
	}
}


_attribute_data_retention_ u8 Adbg_cnt_check = 0;




/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_CONNECT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void	task_connect (u8 e, u8 *p, int n)
{
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  // 1 S
}



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_TERMINATE"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void 	task_terminate(u8 e,u8 *p, int n) //*p is terminate reason
{
	if(*p == HCI_ERR_CONN_TIMEOUT){

	}
	else if(*p == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

	}
	else if(*p == HCI_ERR_CONN_TERM_MIC_FAILURE){

	}
	else{

	}

	u8 read_cnt = 0;
	flash_read_page(0x80000,1,(u8*)&read_cnt);
	if(read_cnt == 0xff){
		read_cnt = 0;
	}

	Adbg_cnt_check = read_cnt + 1;
	sleep_us(100);

	flash_erase_sector(0x80000);
	sleep_us(100);
	flash_write_page(0x80000,1,(u8*)&Adbg_cnt_check);
}



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void	task_suspend_exit (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}





/**
 * @brief      power management code for application
 * @param	   none
 * @return     none
 */
_attribute_ram_code_
void blt_pm_proc(void)
{

#if(BLE_APP_PM_ENABLE)
	#if (PM_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif



#endif  //end of BLE_APP_PM_ENABLE
}

#if (FEATURE_TEST_MODE == TEST_SCANNING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE \
	  || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE )
//////////////////////////////////////////////////////////
// event call back
//////////////////////////////////////////////////////////
#define DBG_ADV_REPORT_ON_RAM 				0
#if (DBG_ADV_REPORT_ON_RAM)  //debug adv report on ram
	#define  RAM_ADV_MAX		64
	u8 AA_advRpt[RAM_ADV_MAX][48];
	u8 AA_advRpt_index = 0;
#endif

/**
 * @brief      callback function of HCI Controller Event
 * @param[in]  h - HCI Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
int controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//ble controller hci event
	{
		u8 evtCode = h & 0xff;

		if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvt_code = p[0];
			if (subEvt_code == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
			{
				DBG_CHN6_TOGGLE;
				//after controller is set to scan state, it will report all the adv packet it received by this event

//				event_adv_report_t *pa = (event_adv_report_t *)p;
//				s8 rssi = (s8)pa->data[pa->len];//rssi has already plus 110.
//				printf("LE advertising report (rssi:%ddb, len:%d):\n", rssi, pa->len+11);

				#if (DBG_ADV_REPORT_ON_RAM)
					if(pa->len > 31){
						pa->len = 31;
					}
					memcpy( (u8 *)AA_advRpt[AA_advRpt_index++],  p, pa->len + 11);
					if(AA_advRpt_index >= RAM_ADV_MAX){
						AA_advRpt_index = 0;
					}
				#endif
			}
		}
	}
	return 0;
}

#endif



/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();  //this is must



//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	/* for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M   Flash, flash_sector_mac_address equals to 0x1FF000*/
	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);		//mandatory
	blc_ll_initLegacyAdvertising_module(); 		//legacy advertising module: mandatory for BLE slave

#if(FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,
#endif


#if (FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY)
	/**
	 * @brief	Adv Packet data
	 */
	u8 tbl_advData[] = {
		 0x08, 0x09, 't', 'e', 's', 't', 'a', 'd', 'v',
		 0x02, 0x01, 0x05,
		};

	/**
	 * @brief	Scan Response Packet data
	 */
	u8	tbl_scanRsp [] = {
			 0x08, 0x09, 'T', 'E', 'S', 'T', 'A', 'D', 'V',	//scan name
		};
	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

	u8 status = bls_ll_setAdvParam( ADV_INTERVAL_100MS, ADV_INTERVAL_100MS, \
									ADV_TYPE_NONCONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
									 0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

	if(status != BLE_SUCCESS){  //adv setting err
		while(1);
	}

	//blc_ll_setAdvCustomedChannel(37, 38, 39);
	bls_ll_setAdvEnable(1);  //adv enable

#elif (FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE)

		blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CONN_MAX_TX_OCTETS);

		blc_ll_initAclConnTxFifo(app_acl_txfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM);
		blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);

		u8 check_status = blc_controller_check_appBufferInitialization();
		if(check_status != BLE_SUCCESS){
			/* here user should set some log to know which application buffer incorrect */
			write_log32(0x88880000 | check_status);
			while(1);
		}
		//////////// Controller Initialization  End /////////////////////////


		//////////// Host Initialization  Begin /////////////////////////
		/* Host Initialization */
		/* GAP initialization must be done before any other host feature initialization !!! */
		blc_gap_peripheral_init();    //gap initialization
		extern void my_att_init ();
		my_att_init (); //gatt initialization

		/* L2CAP Initialization */
		blc_l2cap_register_handler (blc_l2cap_packet_receive);

		/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
		 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
		 *   should re_stored) , so it must be done after battery check */
		blc_smp_peripheral_init();

		// Hid device on android7.0/7.1 or later version
		// New paring: send security_request immediately after connection complete
		// reConnect:  send security_request 1000mS after connection complete. If master start paring or encryption before 1000mS timeout, slave do not send security_request.
		blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
		//////////// Host Initialization  End /////////////////////////

	//////////////////////////// BLE stack Initialization  End //////////////////////////////////


		////// Host Initialization  //////////
		blc_gap_peripheral_init();    //gap initialization
		my_att_init (); //gatt initialization
		blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization
		blc_smp_peripheral_init();									//smp initialization

	///////////////////// USER application initialization ///////////////////
		/**
		 * @brief	Adv Packet data
		 */
		u8 tbl_advData[] = {
			 0x09, 0x09, 's', 'l', 'a', 'v', 'e', 'a', 'd', 'v',
			 0x02, 0x01, 0x05,
			};

		/**
		 * @brief	Scan Response Packet data
		 */
		u8	tbl_scanRsp [] = {
				 0x09, 0x09, 'S', 'L', 'A', 'V', 'E', 'A', 'D', 'V',
			};
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
		bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

		u8 status = bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, \
										 ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
										 0,  NULL,  BLT_ENABLE_ADV_37, ADV_FP_NONE);
		if(status != BLE_SUCCESS){  //adv setting err
			while(1);
		}


		bls_ll_setAdvEnable(1);  //adv enable


		//add advertising in connection slave role
		/**
		 * @brief	Adv test Packet data
		 */
		u8 tbl_advData_test[] = {
			 0x09, 0x09, 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
			 0x02, 0x01, 0x05,
			};

		/**
		 * @brief	Scan Response test Packet data
		 */
		u8	tbl_scanRsp_test [] = {
				 0x09, 0x09, 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B',
			};
		blc_ll_addAdvertisingInConnSlaveRole();  //adv in conn slave role
		blc_ll_setAdvParamInConnSlaveRole(  (u8 *)tbl_advData_test, sizeof(tbl_advData_test), \
											(u8 *)tbl_scanRsp_test, sizeof(tbl_scanRsp_test), \
											ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

		//ble event call back
		bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
		bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);

#elif (FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE)

		blt_ll_initScanState();
		blc_ll_addScanningInAdvState();  //add scan in adv state
		blc_ll_addScanningInConnSlaveRole();  //add scan in conn slave role

		//scan setting
		blc_hci_le_setEventMask_cmd(HCI_LE_EVT_MASK_ADVERTISING_REPORT);
		blc_hci_registerControllerEventHandler(controller_event_callback);


		blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CONN_MAX_TX_OCTETS);

		blc_ll_initAclConnTxFifo(app_acl_txfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM);
		blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);


		////// Host Initialization  //////////
		blc_gap_peripheral_init();    //gap initialization
		my_att_init (); //gatt initialization
		blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization
		blc_smp_peripheral_init();									//smp initialization

	///////////////////// USER application initialization ///////////////////
		/**
		 * @brief	Adv Packet data
		 */
		u8 tbl_advData[] = {
			 0x0A, 0x09, 's', 'l', 'a', 'v', 'e', 's', 'c', 'a', 'n',
			 0x02, 0x01, 0x05,
			};

		/**
		 * @brief	Scan Response Packet data
		 */
		u8	tbl_scanRsp [] = {
				 0x0A, 0x09, 'S', 'L', 'A', 'V', 'E', 'S', 'C', 'A','N'
			};
		bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
		bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

		u8 status = bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, \
										 ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
										 0,  NULL,  BLT_ENABLE_ADV_37, ADV_FP_NONE);
		if(status != BLE_SUCCESS){  //adv setting err
			while(1);
		}


		bls_ll_setAdvEnable(1);  //adv enable


		#if 0  //report all adv
			ll_whiteList_reset();
			u8 test_adv[6] = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33};
			ll_whiteList_add(BLE_ADDR_PUBLIC, test_adv);
			blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,
									  OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_WL);

		#endif





		//ble event call back
		bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
		bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);

#endif


	///////////////////// Power Management initialization///////////////////
#if(BLE_APP_PM_ENABLE)
	blc_ll_initPowerManagement_module();

	#if (PM_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(95, 95);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(360);
		//blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K); //default use 32k deep retention
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif

	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif

}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
#if (PM_DEEPSLEEP_RETENTION_ENABLE)

	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (MY_RF_POWER_INDEX);
	blc_ll_recoverDeepRetention();

	DBG_CHN0_HIGH;    //debug
	irq_enable();


#endif
}


/////////////////////////////////////////////////////////////////////s
// main loop flow
/////////////////////////////////////////////////////////////////////




/**
 * @brief		This is main_loop function
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void main_loop (void)
{
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	////////////////////////////////////// PM Process /////////////////////////////////
#if(FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#else
	blt_pm_proc();
#endif

}





#endif  //end of (FEATURE_TEST_MODE == ...)

/********************************************************************************************************
 * @file     feature_config.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef FEATURE_CONFIG_H_
#define FEATURE_CONFIG_H_



/////////////////// TEST FEATURE SELECTION /////////////////////////////////
//ble link layer test
#define TEST_ADVERTISING_ONLY							1
#define TEST_ADVERTISING_IN_CONN_SLAVE_ROLE				2
#define TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE        3

//power test
#define TEST_POWER_ADV									10
#define TEST_POWER_CONN									11


//smp test
#define TEST_SMP_SECURITY								20 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//gatt secure test
#define TEST_GATT_SECURITY								21 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//slave data length exchange test
#define TEST_SDATA_LENGTH_EXTENSION						22

//other test
#define TEST_USER_BLT_SOFT_TIMER						30
#define TEST_WHITELIST									31
 //phy test
#define TEST_BLE_PHY									32		// BQB PHY_TEST demo
#define TEST_EMI                                        33      // EMI Test demo

#define TEST_EXTENDED_ADVERTISING						40		//  Extended ADV demo


#define TEST_2M_CODED_PHY_EXT_ADV						50		//  2M/Coded PHY used on Extended ADV

#define TEST_2M_CODED_PHY_CONNECTION					60		//  2M/Coded PHY used on Legacy_ADV/Extended_ADV and Connection


#define	TEST_STUCK_KEY									90
#define	TEST_AUDIO										91
#define	TEST_IR											92
#define	TEST_L2CAP_PREPARE_WRITE_BUFF					93

#define	TEST_OTA										95

#define	TEST_MULTIPLE_LOCAL_DEVICE						96

#define TEST_FEATURE_BACKUP								200


#define FEATURE_TEST_MODE								TEST_FEATURE_BACKUP






#endif /* FEATURE_CONFIG_H_ */

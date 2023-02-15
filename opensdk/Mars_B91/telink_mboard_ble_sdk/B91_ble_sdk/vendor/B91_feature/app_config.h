/********************************************************************************************************
 * @file     app_config.h
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

#pragma once


#include "feature_config.h"


#if (FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE || FEATURE_TEST_MODE == TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE )
	#include "feature_ll_state/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_POWER_ADV)
	#include "feature_adv_power/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_POWER_CONN)
	#include "feature_conn_power/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_SMP_SECURITY)
	#include "feature_smp_security/app_config.h"
#elif(FEATURE_TEST_MODE == TEST_GATT_SECURITY)
	#include "feature_gatt_security/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)
	#include "feature_slave_dle/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER)
	#include "feature_soft_timer/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_WHITELIST)
	#include "feature_whitelist/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_BLE_PHY)
	#include "feature_PHY_test/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_EMI)
	#include "feature_emi/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_EXTENDED_ADVERTISING)
	#include "feature_extend_adv/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_EXT_ADV)
	#include "feature_phy_extend_adv/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_CONNECTION)
	#include "feature_phy_conn/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_STUCK_KEY)
	#include "feature_stuck_key/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_AUDIO)
	#include "feature_audio/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_IR)
	#include "feature_ir/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_OTA)
	#include "feature_ota/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_L2CAP_PREPARE_WRITE_BUFF)
	#include "feature_use_reg_buffer/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_MULTIPLE_LOCAL_DEVICE)
	#include "feature_multi_local_dev/app_config.h"
#elif (FEATURE_TEST_MODE == TEST_FEATURE_BACKUP)
	#include "feature_backup/app_config.h"
#endif



/********************************************************************************************************
 * @file    main.c
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
#include "calibration.h"

extern void user_init(void);
extern void main_loop (void);

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main(void)
{
#if(MCU_CORE_B91)
	sys_init(LDO_1P4_LDO_1P8, VBAT_MAX_VALUE_GREATER_THAN_3V6);
	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	user_read_flash_value_calib();
#elif(MCU_CORE_B92)
	sys_init(LDO_1P4_LDO_2P0, VBAT_MAX_VALUE_GREATER_THAN_3V6, GPIO_VOLTAGE_3V3);
	wd_32k_stop();
	//Note: This function can improve the performance of some modules, which is described in the function comments.
	//Called immediately after sys_init, set in other positions, some calibration values may not take effect.
	calibration_func(GPIO_VOLTAGE_3V3);
#endif
	CCLK_24M_HCLK_24M_PCLK_24M;

	rf_mode_init();
#if(RF_MODE==RF_BLE_1M)//1
	rf_set_ble_1M_mode();
#elif(RF_MODE==RF_BLE_1M_NO_PN)//2
 	rf_set_ble_1M_NO_PN_mode();
#elif(RF_MODE==RF_BLE_2M)//3
 	rf_set_ble_2M_mode();
#elif(RF_MODE==RF_BLE_2M_NO_PN)//4
 	rf_set_ble_2M_NO_PN_mode();
#elif(RF_MODE==RF_LR_S2_500K)//5
 	rf_set_ble_500K_mode();
#elif(RF_MODE==RF_LR_S8_125K)//6
 	rf_set_ble_125K_mode();
#elif(RF_MODE==RF_ZIGBEE_250K)//7
 	rf_set_zigbee_250K_mode();
#elif(RF_MODE==RF_PRIVATE_250K)//8
 	rf_set_pri_250K_mode();
#elif(RF_MODE==RF_PRIVATE_500K)//9
 	rf_set_pri_500K_mode();
#elif(RF_MODE==RF_PRIVATE_1M)
	 rf_set_pri_1M_mode();
#elif(RF_MODE==RF_PRIVATE_2M)
 	rf_set_pri_2M_mode();
#elif(RF_MODE==RF_HYBEE_500K)
 	rf_set_hybee_500K_mode();
#elif(RF_MODE==RF_HYBEE_1M)
 	rf_set_hybee_1M_mode();
#elif(RF_MODE==RF_HYBEE_2M)
 	rf_set_hybee_2M_mode();
#elif(RF_MODE==RF_ANT)
 	rf_set_ant_mode();
#elif(RF_MODE==RF_BLE_STX2RX)
 	rf_set_ble_1M_mode();
#elif(RF_MODE==RF_BLE_SDK_TEST)
 	rf_set_ble_1M_mode();
#endif

#if(MCU_CORE_B92)
#if(RF_MODE==RF_USER_DEFINE_PKT)
 	rf_set_ble_1M_NO_PN_mode();
#elif(RF_MODE == RF_BLE_1M_HYPER_LENGTH)
 	rf_set_ble_1M_mode();
#endif
#endif
    user_init();

    while(1)
    {
    	main_loop();
    }
    return 0;
}



/********************************************************************************************************
 * @file     app_buffer.c
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

#include "app_buffer.h"
#include "app_config.h"

#if (FEATURE_TEST_MODE == TEST_L2CAP_PREPARE_WRITE_BUFF)

/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ********************************/
_attribute_data_retention_	u8	app_acl_rxfifo[ACL_RX_FIFO_SIZE * ACL_RX_FIFO_NUM] = {0};
_attribute_data_retention_  u8	app_acl_txfifo[ACL_TX_FIFO_SIZE * ACL_TX_FIFO_NUM] = {0};
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/




#if (L2CAP_BUFF_REG_BY_APP)
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, Begin ********************************/
_attribute_data_retention_	u8	mtu_rx_fifo[MTU_RX_BUFF_SIZE_MAX] = {0};
_attribute_data_retention_	u8	mtu_tx_fifo[MTU_TX_BUFF_SIZE_MAX] = {0};
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, End **********************************/
#endif




#if (PREPARE_WRITE_BUFF_REG_BY_APP)
/**************************** ACL connection Prepare write rx buffer allocation, Begin ********************************/
_attribute_data_retention_	u8 prepare_write_fifo[ACL_CONN_RX_PREPARE_WRITE_MAX] = {0};
/***************************** ACL connection Prepare write rx buffer allocation, End *********************************/
#endif




#endif  //end of (FEATURE_TEST_MODE == ...)

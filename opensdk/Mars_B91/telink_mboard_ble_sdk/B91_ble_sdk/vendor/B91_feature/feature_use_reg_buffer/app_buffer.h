/********************************************************************************************************
 * @file     app_buffer.h
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

#ifndef VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_
#define VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_

#if (FEATURE_TEST_MODE == TEST_L2CAP_PREPARE_WRITE_BUFF)

/**
 * @brief	connMaxRxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_RX_OCTETS			27


/**
 * @brief	connMaxTxOctets
 * refer to BLE SPEC "4.5.10 Data PDU length management" & "2.4.2.21 LL_LENGTH_REQ and LL_LENGTH_RSP"
 * usage limitation:
 * 1. connMaxTxOctets should be in range of 27 ~ 251
 */
#define ACL_CONN_MAX_TX_OCTETS			27




/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin ************************************************/
/**
 * @brief	ACL RX buffer size & number
 *  		ACL RX buffer is used to hold LinkLayer RF RX data.
 * usage limitation for ACL_RX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxRxOctets + 21)
 * 2. should be be an integer multiple of 16 (16 Byte align). user can use formula:  size = CAL_LL_ACL_RX_FIFO_SIZE(connMaxRxOctets)
 * usage limitation for ACL_RX_FIFO_NUM:
 * 1. must be: 2^n, (power of 2)
 * 2. at least 4; recommended value: 8, 16
 */
#define ACL_RX_FIFO_SIZE				48  // ACL_CONN_MAX_RX_OCTETS + 21, then 16 Byte align
#define ACL_RX_FIFO_NUM					8	// must be: 2^n


/**
 * @brief	ACL TX buffer size & number
 *  		ACL TX buffer is used to hold LinkLayer RF TX data.
 * usage limitation for ACL_TX_FIFO_SIZE:
 * 1. should be greater than or equal to (connMaxTxOctets + 10)
 * 2. should be be an integer multiple of 16 (16 Byte align). user can use formula:  size = CAL_LL_ACL_TX_FIFO_SIZE(connMaxTxOctets)
 * usage limitation for ACL_TX_FIFO_NUM:
 * 1. must be: (2^n) + 1, (power of 2, then add 1)
 * 2. at least 9; recommended value: 9, 17, 33; other value not allowed.
 * usage limitation for size * (number - 1)
 * 1. (ACL_TX_FIFO_SIZE * (ACL_TX_FIFO_NUM - 1)) can not exceed 4096 (4K)
 *    so when ACL TX FIFO size bigger than 256(when connMaxTxOctets bigger than 246), ACL TX FIFO number can only be 9(can not use 17)
 */
#define ACL_TX_FIFO_SIZE				48	// ACL_CONN_MAX_TX_OCTETS + 10, then 16 Byte align
#define ACL_TX_FIFO_NUM					17	// must be: (2^n) + 1




extern	u8	app_acl_rxfifo[];
extern	u8	app_acl_txfifo[];
/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***************************************************/




/*
 * If the stack default buffer is not enough, you can register a larger buffer.
 */
#define L2CAP_BUFF_REG_BY_APP			1 //0:Use default l2cap buffer
#define PREPARE_WRITE_BUFF_REG_BY_APP	1 //0:Use default prepare write buffer


#if (L2CAP_BUFF_REG_BY_APP)

/**
 * @brief	connMaxRxMTU
 * refer to BLE SPEC "3.2.8 Exchanging MTU size" & "3.2.9 Long attribute values"
 * usage limitation:
 * 1. If the user uses the stack default buffer, the range should be between 23~250.
 * 2. If the user uses his own registered l2cap buffer instead of the stack default buffer,
 *    the range should be limited by the maximum buffer length.
 */
#define MTU_SIZE_SETTING				290


/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, Begin ********************************/
/*Note:
 * MTU Buff size = Extra_Len(6)+ ATT_MTU_MAX
 *  1. should be greater than or equal to (ATT_MTU + 6)
 */
#define	MTU_TX_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(MTU_SIZE_SETTING)

/** if support LE Secure Connections, L2CAP buffer must >= 70 */
#define MTU_RX_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(MTU_SIZE_SETTING)

extern	u8 mtu_rx_fifo[];
extern	u8 mtu_tx_fifo[];
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, End **********************************/
#endif


#if (PREPARE_WRITE_BUFF_REG_BY_APP)
/**
 * @brief	connMaxRxMTU
 * refer to BLE SPEC "3.4.6 Queued writes" & "3.2.9 Long attribute values"
 * usage limitation:
 * 1. If the user uses the stack default buffer, the buffer size should be 260, the last 3 bytes
 *    are used to store length information.
 * 2. If the user uses his own registered prepare write buffer instead of the stack default buffer,
 *    the buffer size should be set to the size of the buffer, the last 3 bytes are used to store
 *    length information.
 *
 * Note: The buffer size is set according to the actual size of the buffer to be written, if the
 *       prepare write buffer is larger than the default size of the stack, the user's own registered
 *       larger buffer can be used.
 */
#define ACL_CONN_RX_PREPARE_WRITE_MAX	360


/**************************** ACL connection Prepare write rx buffer allocation, Begin ********************************/
extern	u8 prepare_write_fifo[];
/***************************** ACL connection Prepare write rx buffer allocation, End *********************************/
#endif


#endif  //end of (FEATURE_TEST_MODE == ...)
#endif /* VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_ */

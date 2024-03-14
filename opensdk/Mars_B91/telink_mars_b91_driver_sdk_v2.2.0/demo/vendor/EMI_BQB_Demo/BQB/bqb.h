/********************************************************************************************************
 * @file    bqb.h
 *
 * @brief   This is the header file for B91m
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
#ifndef BQB_H_
#define BQB_H_

#include "../app_config.h"

#if(TEST_DEMO==BQB_DEMO)

/*Set to 1, open the code related to BQB tool configuration (uart port, CAP value, TX Power, PA port, calibration position, etc.).*/
#define SUPPORT_CONFIGURATION	0

/* set power of Tx */
#if(MCU_CORE_B91)
#define BQB_TX_POWER		RF_POWER_P6p98dBm
#else
#define BQB_TX_POWER		RF_POWER_P7p00dBm
#endif

#define ACCESS_CODE        	0x29417671

/* set configuration flash address */
#if SUPPORT_CONFIGURATION
#define CONFIGURATION_ADDR_UART_TX		0x12
#define	CONFIGURATION_ADDR_UART_RX		0x13
#define CONFIGURATION_ADDR_PA_TX		0x14
#define	CONFIGURATION_ADDR_PA_RX		0x15
#define CONFIGURATION_ADDR_CAP			0x16
#define	CONFIGURATION_ADDR_POWER		0x17
#endif
#define CALIBRATION_CAP_RAM_ADDR		0x06

/* set flash address to set internal cap value and switch internal/external cap */
#define CAP_SET_FLASH_ADDR_4M       0x3fe000
#define CAP_SET_FLASH_ADDR_2M 		0x1fe000
#define CAP_SET_FLASH_ADDR_1M 		0xfe000
#define CAP_SET_FLASH_ADDR_512K 	0x7e000//B85m:0x77000 B91m:0x7e000
#define CAP_SET_FLASH_ADDR_128K 	0x1e000
#define CAP_SET_FLASH_ADDR_64K		0xe000
#if !SUPPORT_CONFIGURATION
#define SWITCH_INTERNAL_CAP 		1		// 0: external cap, 1: internal cap
#define CAP_SET_FLASH_ADDR 			0xfe000 // 512K Flash: 0x7e000, 1M FLASH: 0xfe000, 2M FLASH: 0x1fe000
#endif

/* set uart port */
#if(MCU_CORE_B91)
#define BQB_UART_TX_PORT   	UART0_TX_PB2
#define BQB_UART_RX_PORT   	UART0_RX_PB3
#elif(MCU_CORE_B92)
#define BQB_UART_TX_PORT   	GPIO_FC_PB1
#define BQB_UART_RX_PORT   	GPIO_FC_PB2
#endif
#define BQB_UART_BAUD	   	115200

/* set pa port */
#if !SUPPORT_CONFIGURATION
#define BQB_PA_TX_PORT   	GPIO_PA0
#define BQB_PA_RX_PORT   	GPIO_PA0
#endif

/* choose calibration position */
#if !SUPPORT_CONFIGURATION
#define SWITCH_CALIBRATION_POSITION		1	//0:SRAM 1:FLASH
#endif

/* switch power mode */
#if !SUPPORT_CONFIGURATION
#define SWITCH_POWER_MODE				1	//0:DCDC 1:LDO
#endif
/* set TX return result */
#define RETURN_TX_LEN_EN    0      //1: return tx length, 0:do not return tx length


/**
 *  @brief  command type for BQB Test
 */
typedef enum{
	CMD_SETUP=0,
	CMD_RX_TEST=1,
	CMD_TX_TEST=2,
	CMD_END=3,
}Test_Command_e;

/**
 *  @brief  command status for BQB Test
 */
typedef enum {
	SETUP_STATE=0x10,
	RX_STATE,
	TX_STATE,
	END_STATE
}Test_Status_e;

typedef enum
{
	UART_NUM0 = UART0,
	UART_NUM1 = UART1,
	UART_NONE,
}uart_num_redef_e;

#if SUPPORT_CONFIGURATION
/**
 *  @brief  union structure for configuration
 */
typedef struct
{
	unsigned char uart_tx;
	unsigned char uart_rx;
	unsigned char pa_tx;
	unsigned char pa_rx;
	unsigned char flash:7;
	unsigned char cap:1;
	unsigned char cal_pos:2;
	unsigned char power_mode:2;
	unsigned char power;
}usr_def_t;

/* global value for configuration */
extern usr_def_t usr_config;
#define get_pin(value) (((unsigned short)((value) >> 3) << 8) | BIT((value) & 0x07))
#endif

/* global value for uart */
extern uart_num_redef_e uart_using;





/**
 * @brief   	This function serves to initialize  BQB
 * @param[in]   flash_size - flash size: 0->512K, 1->2M, 2-0xff->1M.
 * @return  	none.
 */
void  bqbtest_init();


/**
 * @brief   This function serves to read the uart data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void);


/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    none
 */
void bqb_uart_send_byte(unsigned char uartData);

extern void bqb_pa_init();

extern void bqb_pa_set_mode(unsigned char rtx); //0:rx, 1:tx, other:off
#endif

#endif /* BQB_H_ */

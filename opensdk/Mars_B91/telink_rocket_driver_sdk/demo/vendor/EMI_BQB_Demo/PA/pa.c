/********************************************************************************************************
 * @file    pa.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "pa.h"
#include "gpio.h"
#include "flash.h"
#if(EMI_SUPPORT_SETTING)
/**
 * @brief	Enumeration type of PA operation.
 */
typedef enum{
	PA_SETTING_OPT_OUTPUT = 0xa5,
}pa_setting_OPT_e;

/**
 * @brief	Enumeration type of PA pin level.
 */
typedef enum{
	PA_SETTING_PIN_LEVEL_LOW = 0x00,
	PA_SETTING_PIN_LEVEL_HIGH = 0x01,
}pa_setting_pin_level_e;

/**
 * @brief	Structure used to store an operation instruction.
 */
typedef union{
	unsigned int cmd;
	struct{
		unsigned char state;
		unsigned char opt;
		unsigned char para0;
		unsigned char para1;
	}data;
}pa_setting_cmd_t, *pa_setting_cmd_t_ptr;

/**
 * @brief	Structure used to store PA Setting information.
 */
typedef struct{
	unsigned int addr;
	unsigned int bit1_cnt;
	unsigned int cmd_cnt;
}pa_setting_data_t, *pa_setting_data_t_ptr;

/**
 * @brief	Structure used to store PA operation information.
 */
typedef struct{
	pa_setting_data_t pa_setting_data;
	unsigned char pa_en;
	unsigned char bypass_en;
}pa_operation_t, *pa_operation_t_ptr;

/**
 * @brief	Local variable used to hold data related to the PA operation.
 */
static pa_operation_t s_pa_operation = {{0, 0, 0}, 0, 0};

/**
 * @brief		This function serves to read data saved in binary file.
 * @param[in]   addr   - Position in binary file.
 * @return 		One-byte result data.
 */
unsigned char read_data8(unsigned int addr)
{
	unsigned char ret = 0;
#if MCU_STARTUP_FLASH
	flash_read_page(addr, 1, &ret);
#else
	ret = read_sram8(addr);
#endif
	return ret;
}

/**
 * @brief		This function serves to read data saved in binary file.
 * @param[in]   addr   - Position in binary file.
 * @return 		Two-byte result data.
 */
unsigned short read_data16(unsigned int addr)
{
	unsigned short ret = 0;
#if MCU_STARTUP_FLASH
	flash_read_page(addr, 2, (unsigned char*)(&ret));
#else
	ret = read_sram16(addr);
#endif
	return ret;
}

/**
 * @brief		This function serves to read data saved in binary file.
 * @param[in]   addr   - Position in binary file.
 * @return 		Four-byte result data.
 */
unsigned int read_data32(unsigned int addr)
{
	unsigned int ret = 0;
#if MCU_STARTUP_FLASH
	flash_read_page(addr, 4, (unsigned char*)(&ret));
#else
	ret = read_sram32(addr);
#endif
	return ret;
}

/**
 * @brief		This function serves to initialize PA setting.
 * @param[in]   addr       - Address to save PA related operations.
 * @param[in]   bypass_en  - To set PA bypass mode, 0:disable 1:enable.
 * @return 		none
 */
void pa_setting_init(unsigned int addr, unsigned char bypass_en)
{
	s_pa_operation.pa_setting_data.addr = read_data32(addr);

	s_pa_operation.bypass_en = bypass_en;

	//check address
	if(s_pa_operation.pa_setting_data.addr != addr)
	{
		s_pa_operation.pa_en = 0;
		return;
	}

	s_pa_operation.pa_setting_data.bit1_cnt = read_data32(addr+4);
	s_pa_operation.pa_setting_data.cmd_cnt = read_data32(addr+8);

	//check bit1 count
	unsigned int data_check;
	unsigned int bit1_cnt = 0;
	unsigned int data_len = 1 + s_pa_operation.pa_setting_data.cmd_cnt;
	for(unsigned int i = 0; i < data_len; i++)
	{
		data_check = read_data32(s_pa_operation.pa_setting_data.addr + 8 + 4*i);
		for(unsigned char j = 0; j < 32; j++)
		{
			bit1_cnt += (data_check>>j) & 1;
		}
	}

	if(bit1_cnt != s_pa_operation.pa_setting_data.bit1_cnt)
	{
		s_pa_operation.pa_en = 0;
		return;
	}

	s_pa_operation.pa_en = 1;
}

/**
 * @brief		This function serves to execute PA operations according to the cmd.
 * @param[in]   cmd      - PA operation command.
 * @return 		none
 */
static inline void cmd_operation(pa_setting_cmd_t cmd)
{
	unsigned short pin = get_pin(cmd.data.para0);
	switch(cmd.data.opt)
	{
	case PA_SETTING_OPT_OUTPUT:
		gpio_function_en(pin);
		gpio_input_dis(pin);
		gpio_output_en(pin);
		if(cmd.data.para1)
		{
			gpio_set_high_level(pin);
		}
		else
		{
			gpio_set_low_level(pin);
		}
		break;
	default:
		break;
	}
}
#endif

/**
 * @note The pin status of the BY PASS mode varies according to the PA chip.
 *       You need to change the pin status according to the actual situation
 */
#define PA_BYPASS   0
/**
 * @brief		This function serves to execute PA operations according to the state.
 * @param[in]   state      - PA working state.
 * @return 		none
 */
void pa_operation(pa_setting_state_e state)
{

#if(EMI_SUPPORT_SETTING)
	if(!s_pa_operation.pa_en) return;
	if(s_pa_operation.bypass_en) state = PA_SETTING_STATE_BYPASS;

	pa_setting_cmd_t cmd;
	for(unsigned int i = 0; i < s_pa_operation.pa_setting_data.cmd_cnt; i++)
	{
		cmd.cmd = read_data32(s_pa_operation.pa_setting_data.addr + 12 + 4*i);

		if(state == cmd.data.state)
		{
			cmd_operation(cmd);
		}
	}

#else


	switch(state)
	{
	case PA_SETTING_STATE_INIT:
			//to do:To use the PA function, enable related PA control pins
		break;
#if(!PA_BYPASS)
	case PA_SETTING_STATE_TX:
			//to do:To use the PA function, enable related PA control pins
		break;
	case PA_SETTING_STATE_RX:
			//to do:To use the PA function, enable related PA control pins
		break;
#else

	case PA_SETTING_STATE_TX:
	case PA_SETTING_STATE_RX:
	case PA_SETTING_STATE_BYPASS:
			//to do:To use the PA function, enable related PA control pins
		break;

#endif
	default:
		break;
	}


#endif

}

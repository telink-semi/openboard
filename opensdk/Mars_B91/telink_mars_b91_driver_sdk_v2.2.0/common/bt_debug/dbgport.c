/********************************************************************************************************
 * @file    dbgport.c
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
#include "dbgport.h"

void bt_dbg_set_pin(btdbg_pin_e pin)
{
	unsigned char val,v=0;
	unsigned char mask= 0;
	unsigned char n=0;
	if(pin&0xf)
	{
		v = pin&0xf;
		do{
			n++;
			v= v>>1;
		}while(v);

		mask = ((unsigned char)~(3<<((n-1)*2)));
		val = ((3<<((n-1)*2)));
	}
	else if(pin&0xf0)
	{
		v = ((pin>>4)&0xf);
		do{
			n++;
			v= v>>1;
		}while(v);

		mask = ((unsigned char)~(3<<((n-1)*2)));
		val = ((3<<((n-1)*2)));
	}
	else
	{
		while(1);
	}

	//note:  setting pad the function  must before  setting no_gpio function, cause it will lead to uart transmit extra one byte data at begin.(confirmed by minghai&sunpeng)
	reg_gpio_func_mux(pin)=(reg_gpio_func_mux(pin)& mask)|val;

	gpio_function_dis((gpio_pin_e)pin);

}


void bt_dbg_set_signal(bt_signal_group_e signal_group,unsigned char signal_byte)
{
	if(signal_group&0xf000)
	{
		switch(signal_group)
		{
			case BT_DBG_SEL_DBG0:
				bt_diagcntl_diag0_setf(signal_byte);
				bt_diagcntl_diag0_en_setf(1);
				break;
			case BT_DBG_SEL_DBG1:
				bt_diagcntl_diag1_setf(signal_byte);
				bt_diagcntl_diag1_en_setf(1);
				break;
			case BT_DBG_SEL_DBG2:
				bt_diagcntl_diag2_setf(signal_byte);
				bt_diagcntl_diag2_en_setf(1);
				break;
			case BT_DBG_SEL_DBG3:
				bt_diagcntl_diag3_setf(signal_byte);
				bt_diagcntl_diag3_en_setf(1);
				break;
			default:break;
		}
		reg_bt_dbg_sel |= signal_group;
		reg_bt_dbg_sel |= BT_DBG_SEL_EN;
	}
	else
	{
		while(1);
	}
}

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
#include "printf.h"

extern void user_init(void);
extern void main_loop (void);
extern int main_coremark (void);
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
extern float coremark_result;


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
	CCLK_24M_HCLK_24M_PCLK_24M;;
	user_init();
	while ((read_reg8(0x10080b) & 0x7f) == 0);
	reg_usb_ep8_send_thres = 0x1;
	printf ("\r\n\r\n Core Mark Starts(wait about 10s~20s...) ...\r\n");
	delay_ms (100);
	reg_usb_ep8_send_thres = 0x40;
	main_coremark ();
	reg_usb_ep8_send_thres = 1;
	printf("coremark result = %f (24M)\r\n",coremark_result);
	printf("coremark result/clk(Mhz) = %f \r\n",(coremark_result/24));
    while(1)
    {
    	main_loop();
    }

    return 0;
}

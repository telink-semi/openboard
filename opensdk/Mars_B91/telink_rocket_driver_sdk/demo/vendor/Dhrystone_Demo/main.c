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

extern void user_init(void);
extern void main_loop (void);
extern void dhry_main (void);
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */

extern float Dhrystone_DMIPS_Per_MHz;

int main(void)
{
    PLATFORM_INIT;
    CLOCK_INIT;

	user_init();
	while ((read_reg8(0x10080b) & 0x7f) == 0);

	for (int i=0; i<1; i++)
	{
		printf ("\r\n\r\n Drystone Benchmark %d Starts ...", i);
		dhry_main();
	}
	printf("\r\n[dhrystone] : %6.2f\r\n",Dhrystone_DMIPS_Per_MHz);
	delay_ms (100);
	reg_usb_ep8_send_thres = 1;


    while(1)
    {
    	main_loop();
    }

    return 0;
}

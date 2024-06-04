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
extern void main_loop(void);



/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
	PLATFORM_INIT;


#if(SPI_MODE == SPI_XIP_MODE)//SPI_XIP_MODE need fast frequent
	CCLK_96M_HCLK_48M_PCLK_24M;
#else
	CCLK_24M_HCLK_24M_PCLK_24M;
#endif

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}


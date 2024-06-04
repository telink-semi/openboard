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

extern volatile unsigned char irq_flag;

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
#if(MCU_CORE_B91)
_attribute_ram_code_sec_ void rf_dm_irq_handler(void)
{
	if(aes_get_irq_status(FLD_CRYPT_IRQ))
	{
		gpio_toggle(LED1);
		irq_flag = 1;
	}
	aes_clr_irq_status(FLD_CRYPT_IRQ);
}
PLIC_ISR_REGISTER(rf_dm_irq_handler, IRQ_ZB_DM)
#elif(MCU_CORE_B92)
_attribute_ram_code_sec_ void rf_bt_irq_handler(void)
{
	if(aes_get_irq_status(FLD_CRYPT_IRQ))
	{
		gpio_toggle(LED1);
		irq_flag = 1;
	}
	aes_clr_irq_status(FLD_CRYPT_IRQ);
}
PLIC_ISR_REGISTER(rf_bt_irq_handler, IRQ_ZB_BT)

#endif

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main(void)
{
    PLATFORM_INIT;
    CLOCK_INIT;
    user_init();

    while(1)
    {
    	main_loop();
    }
    return 0;
}

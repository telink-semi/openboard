/********************************************************************************************************
 * @file    app.c
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
void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
#if(DEBUG_BUS==USB_PRINT_DEBUG_ENABLE)
	usb_set_pin_en();
	usbhw_set_printer_threshold(1);
	usbhw_set_ep8_fifo_mode();
	delay_ms(1000);
	printf("\n Driver version: %2x \n Copyright (c) %d Telink semiconductor (%s) Ltd, Co \n",0xa001,2019,"shanghai");
#elif(DEBUG_BUS==UART_PRINT_DEBUG_ENABLE)
	printf("  \n");								// caution: The first byte will be error
	printf("\n Driver version: %2x \n",0xa001);
	printf("\n Copyright (c) %d \n",2020);
	printf("\n Telink semiconductor (%s)Ltd, Co \n","shanghai");

#endif

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	printf(" Hello world! \n");
	gpio_toggle(LED1);
	gpio_toggle(LED2);
	gpio_toggle(LED3);
	gpio_toggle(LED4);
	delay_ms(50);
}





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
#include <FreeRTOS.h>
#include <task.h>


static void led_task(void *pvParameters){
	reg_gpio_pb_oen &= ~ GPIO_PB7;
	while(1){
		reg_gpio_pb_out |= GPIO_PB7;
		vTaskDelay(1000);
		reg_gpio_pb_out &= ~GPIO_PB7;
		vTaskDelay(1000);
	}
	(void)(pvParameters);
}

static void led_task1(void *pvParameters){
	reg_gpio_pb_oen &= ~ GPIO_PB4;
	while(1){
		reg_gpio_pb_out |= GPIO_PB4;
		vTaskDelay(500);
		reg_gpio_pb_out &= ~GPIO_PB4;
		vTaskDelay(1000);
	}
	(void)(pvParameters);
}

extern void vPortRestoreTask();

/**
 * @brief		This is main function
 * @return      none
 */
int main (void)
{
	PLATFORM_INIT;
	CLOCK_INIT;

	if(0 ){						// deepRetWakeUp
		vPortRestoreTask();
	}else{
		xTaskCreate( led_task, "tLed", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		xTaskCreate( led_task1, "tLed1", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		vTaskStartScheduler();
	}

	return 0;
}

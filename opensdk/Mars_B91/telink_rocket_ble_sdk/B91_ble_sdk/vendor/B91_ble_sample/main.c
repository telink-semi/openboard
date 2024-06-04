/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

#if(FREERTOS_ENABLE)
#include <FreeRTOS.h>
#include <task.h>
#endif
/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN14_LOW;
}



/**
 * @brief		BLE SDK System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN15_LOW;
}




#if (FREERTOS_ENABLE)
	#if(!TEST_CONN_CURRENT_ENABLE)
		static void led_task(void *pvParameters){
			reg_gpio_pb_oen &= ~ GPIO_PB7;
			while(1){
				reg_gpio_pb_out |= GPIO_PB7;
				printf("LED ON;\r\n");
				vTaskDelay(1000);
				printf("LED OFF;\r\n");
				reg_gpio_pb_out &= ~GPIO_PB7;
				vTaskDelay(1000);
			}
		}
	#endif
void proto_task( void *pvParameters );
#endif

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)   //must on ramcode
{
	DBG_CHN0_LOW;
	blc_pm_select_internal_32k_crystal();

	sys_init(DCDC_1P4_DCDC_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6);

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_16M_HCLK_16M_PCLK_16M;

	rf_drv_ble_init();

	gpio_init(!deepRetWakeUp);



	if(!deepRetWakeUp){//read flash size
		#if (BATT_CHECK_ENABLE)
			user_battery_power_check();
		#endif

		blc_readFlashSize_autoConfigCustomFlashSector();

		#if (FLASH_FIRMWARE_CHECK_ENABLE)
			blt_firmware_completeness_check();
		#endif

		#if FIRMWARES_SIGNATURE_ENABLE
			blt_firmware_signature_check();
		#endif
	}

	/* load customized freq_offset cap value. */
	blc_app_loadCustomizedParameters();

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
#if (FREERTOS_ENABLE)
		extern void blc_ll_set_freertos_en(u8 en);
		blc_ll_set_freertos_en(1);
#endif
	}

#if (FREERTOS_ENABLE)

	extern void vPortRestoreTask();
	if( deepRetWakeUp ){
		printf("enter restor work.\r\n");
		vPortRestoreTask();

	}else{
		#if(!TEST_CONN_CURRENT_ENABLE)
			xTaskCreate( led_task, "tLed", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		#endif
		xTaskCreate( proto_task, "tProto", 2*configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
	//	xTaskCreate( ui_task, "tUI", configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY + 1, 0 );
		vTaskStartScheduler();
	}
#else
	irq_enable();

	while (1) {
		main_loop ();
	}
	return 0;
#endif
}

#if (FREERTOS_ENABLE)
//  !!! should notify those tasks that ulTaskNotifyTake long time and should be wakeup every time PM wakeup
void vPortWakeupNotify(){
}
#endif

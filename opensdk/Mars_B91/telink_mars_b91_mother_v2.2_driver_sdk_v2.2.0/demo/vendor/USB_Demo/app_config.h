/********************************************************************************************************
 * @file    app_config.h
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
#pragma once
#include "driver.h"

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#if(MCU_CORE_B91)
#define LED1            GPIO_PB4
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6
#define LED4            GPIO_PB7
#elif(MCU_CORE_B92)
#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PE6
#define LED4            GPIO_PE7
/**
 * @brief	The POWER_SUPPLY_MODE defaults to VBUS_POWER_SUPPLY.
 */
#define VBAT_POWER_SUPPLY   0
#define VBUS_POWER_SUPPLY   1
#define POWER_SUPPLY_MODE   VBUS_POWER_SUPPLY

#define AISO              0
#define INT               1


#define USB_MODE          INT
#endif

#define	 USB_MOUSE          1
#define	 USB_KEYBOARD		2
#define	 USB_MICROPHONE		3
#define	 USB_SPEAKER		4
#define	 USB_CDC	        5
#define	 USB_MIC_SPEAKER    6
#if (MCU_CORE_B91)
#define	 USB_MASS_STORAGE   7
#endif

#define	 USB_DEMO_TYPE		USB_MOUSE



#if	(USB_DEMO_TYPE == USB_MOUSE)
	#define	USB_MOUSE_ENABLE 		1
  #define	USB_KEYBOARD_ENABLE 	1
#elif (USB_DEMO_TYPE == USB_KEYBOARD)
	#define	USB_KEYBOARD_ENABLE 	1
#elif (USB_DEMO_TYPE == USB_MICROPHONE)
	#define	  USB_MIC_ENABLE 		1
#elif (USB_DEMO_TYPE == USB_SPEAKER)
	#define	USB_SPEAKER_ENABLE 		1
#elif (USB_DEMO_TYPE == USB_CDC)
	#define USB_CDC_ENABLE          1
#elif (USB_DEMO_TYPE == USB_MIC_SPEAKER)
	#define	USB_MIC_ENABLE 			1
	#define	USB_SPEAKER_ENABLE 		1
#endif
#if ((USB_DEMO_TYPE == USB_MASS_STORAGE)&&MCU_CORE_B91)
	#define	USB_MASS_STORAGE_ENABLE     1

	#define SYS_NOR_FLASH_SUPPORT       1 //sys NOR_FLASH  1.Please refer to "sys_norflash.h" for the allocated address and capacity,2.Must be formatted for the first use
    #define SD_NAND_FLASH_SUPPORT       0 // External SD_NAND flash must be connected
#if  (SD_NAND_FLASH_SUPPORT) && (SYS_NOR_FLASH_SUPPORT)
	#define USB_DISK_NUM     1 //1 means 2 disk,0 means 1 disk, and so on
#elif (SD_NAND_FLASH_SUPPORT) || (SYS_NOR_FLASH_SUPPORT)
	#define USB_DISK_NUM     0 //1 means 2 disk,0 means 1 disk, and so on
#endif
#endif




#define CHIP_VER_A0              0
#define CHIP_VER_A1              1
#define CHIP_VER             CHIP_VER_A1










/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

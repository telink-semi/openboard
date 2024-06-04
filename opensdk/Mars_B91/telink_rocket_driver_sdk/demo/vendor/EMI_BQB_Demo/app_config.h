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
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#include "driver.h"



#define EMI_DEMO	0
#define BQB_DEMO	1
#define TEST_DEMO	EMI_DEMO


#if TEST_DEMO == EMI_DEMO

/**
 * @brief Generate EMI binary that support setting, Open by default
 * @note  1.The emi bin provided in the BDT tool is compiled under the EMI_SUPPORT_SETTING option.
 *        2.When compiling sram bin, you need to delete (NOLOAD) after the sbss and bss segments in ram_boot.link.
 *          At the same time, delete the AT instruction content after sdk_version segment.
 *          (The ram_boot.link file does not need to be modified when compiling the flash bin)
 */
#define EMI_SUPPORT_SETTING			1

#if EMI_SUPPORT_SETTING
/**
 * @brief Structure to save general setting.
 */
typedef struct{
	unsigned char ptr[0];
	unsigned char calib_pos:2;
	unsigned char pa_bypass_en:1;
	unsigned char :1;
	unsigned char swire_through_usb_en:1;
	unsigned char power_mode:2;
    unsigned char cap:1;
}general_setting_t, *general_setting_t_ptr;

/**
 * @brief Structure to save emi configuration.
 */
typedef struct{
	unsigned int pa_setting_pos;
	general_setting_t general_setting;
}emi_setting_t, *emi_setting_t_ptr;
#endif

#endif

/**
 * @brief Define where the configuration data will be saved in the binary file.
 */
#define VERSION_ADDR				(0x02)
#define PA_SETTING_ADDR				(0x04)
#define GENERAL_SETTING_ADDR		(0x16)

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

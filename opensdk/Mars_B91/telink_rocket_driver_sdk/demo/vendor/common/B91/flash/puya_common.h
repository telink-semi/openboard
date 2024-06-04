/********************************************************************************************************
 * @file    puya_common.h
 *
 * @brief   This is the header file for B91
 *
 * @author  Driver Group
 * @date    2022
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
 *
 *******************************************************************************************************/
#ifndef VENDOR_COMMON_FLASH_PUYA_COMMON_H_
#define VENDOR_COMMON_FLASH_PUYA_COMMON_H_
#include "driver.h"
#include "compiler.h"
void audio_set_chn_wl(audio_channel_wl_mode_e chn_wl);
void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode , audio_in_mode_e ain1_mode,audio_out_mode_e i2s_aout_mode);
void audio_i2s_config(i2s_mode_select_e i2s_format,i2s_data_select_e wl,  i2s_codec_m_s_mode_e m_s , audio_i2s_invert_config_t * i2s_config_t);
/**
 * @brief 		This function serves to pull up wp.
 * @return 		none.
 */
_attribute_ram_code_sec_ static inline void mspi_wp_low(void){
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
}
/**
 * @brief 		This function serves to pull down wp.
 * @return 		none.
 */
_attribute_ram_code_sec_ static inline void mspi_wp_high(void){
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
}
/**
 * @brief 		This function set GPIO_PF5 as gpio function.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void mspi_as_gpio();
/**
 * @brief 		This function set GPIO_PF5 as mspi function.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void mspi_as_mspi();
/**
 * @brief 		This function serves to enter flash test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_enter_test_mode(unsigned char *data);

/**
 * @brief 		This function serves to exit flash test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ void flash_exit_test_mode(void);

/**
 * @brief 		This function serves to flash read in test mode.
 * @param[in]   cmd			- the read command.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte, must be above 0) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_read_testmode(unsigned char cmd,unsigned int addr,unsigned long len, unsigned char *buf);

/**
 * @brief 		This function serves to flash write in test mode.
 * @param[in]   cmd			- the write command.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte, must be above 0) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_write_testmode(unsigned char cmd,unsigned int addr,unsigned long len, unsigned char *buf);


/**
 * @brief     This function serves to configuration the audio work on.
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 * @note	  This function serve to initialize the audio module. copy from audio_init() function in audio.c file.
 * 			  but have some simplify to save running time. It is configure to make 1V8 power down more quickly.And
 * 			  it can not use as the normal audio init.
 */
void audio_config_on(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl);


/**
 * @brief     This function serves to power off flash.
 * @return    none
 */
_attribute_text_sec_ void flash_power_off_on(unsigned int flash_off_time_ms);

#endif /* VENDOR_COMMON_FLASH_PUYA_COMMON_H_ */

/********************************************************************************************************
 * @file     blt_common.h
 *
 * @brief    This is the header file for BLE SDK
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

#ifndef BLT_COMMON_H_
#define BLT_COMMON_H_

#include "drivers.h"




//////////////////////////// Flash  Address Configuration ///////////////////////////////

/**************************** 128 K Flash *****************************/
#ifndef		CFG_ADR_MAC_128K_FLASH
#define		CFG_ADR_MAC_128K_FLASH								0x1F000
#endif

#ifndef		CFG_ADR_CALIBRATION_128K_FLASH
#define		CFG_ADR_CALIBRATION_128K_FLASH						0x1E000
#endif

/**************************** 512 K Flash *****************************/
#ifndef		CFG_ADR_MAC_512K_FLASH
#define		CFG_ADR_MAC_512K_FLASH								0x7F000	//B91 and later IC
#endif

#ifndef		CFG_ADR_CALIBRATION_512K_FLASH
#define		CFG_ADR_CALIBRATION_512K_FLASH						0x7E000	//B91 and later IC
#endif

/**************************** 1 M Flash *******************************/
#ifndef		CFG_ADR_MAC_1M_FLASH
#define		CFG_ADR_MAC_1M_FLASH		   						0xFF000
#endif


#ifndef		CFG_ADR_CALIBRATION_1M_FLASH
#define		CFG_ADR_CALIBRATION_1M_FLASH						0xFE000
#endif

/**************************** 2 M Flash *******************************/
#ifndef		CFG_ADR_MAC_2M_FLASH
#define		CFG_ADR_MAC_2M_FLASH		   						0x1FF000
#endif


#ifndef		CFG_ADR_CALIBRATION_2M_FLASH
#define		CFG_ADR_CALIBRATION_2M_FLASH						0x1FE000
#endif

/**************************** 4 M Flash *******************************/
#ifndef		CFG_ADR_MAC_4M_FLASH
#define		CFG_ADR_MAC_4M_FLASH		   						0x3FF000
#endif


#ifndef		CFG_ADR_CALIBRATION_4M_FLASH
#define		CFG_ADR_CALIBRATION_4M_FLASH						0x3FE000
#endif

/** Calibration Information FLash Address Offset of  CFG_ADR_CALIBRATION_xx_FLASH ***/
#define		CALIB_OFFSET_CAP_INFO								0x0
#define		CALIB_OFFSET_TP_INFO								0x40

#define		CALIB_OFFSET_ADC_VREF								0xC0

#define		CALIB_OFFSET_FIRMWARE_SIGNKEY						0x180







extern u32 flash_sector_mac_address;
extern u32 flash_sector_calibration;
extern bool    adc_check_efuse_flag;




/**
 * @brief		This function is used to enable the external crystal capacitor
 * @param[in]	en - enable the external crystal capacitor
 * @return      none
 */
static inline void blc_app_setExternalCrystalCapEnable(u8  en)
{
	blt_miscParam.ext_cap_en = en;

	analog_write_reg8(0x8a,analog_read_reg8(0x8a)|0x80);//close internal cap

}


/**
 * @brief		This function is used to load customized parameters from flash sector for application
 * @param[in]	none
 * @return      none
 */
static inline void blc_app_loadCustomizedParameters(void)
{
	/* Attention:
	 * for 1M  Flash, flash_sector_calibration equals to 0xFE000
	 * for 2M  Flash, flash_sector_calibration equals to 0x1FE000
	 * Other Size, SDK do not support now, user should contact Telink to process this special situation */
	if(flash_sector_calibration)
	{
		if(!blt_miscParam.ext_cap_en){
			//customize freq_offset adjust cap value, if not customized, default ana_8A is 0x60
			u8 cap_frqoft;
			flash_read_page(flash_sector_calibration + CALIB_OFFSET_CAP_INFO, 1, &cap_frqoft);
			if( cap_frqoft != 0xff ){
				analog_write_reg8(0x8A, (analog_read_reg8(0x8A) & 0xc0)|(cap_frqoft & 0x3f));
			}
		}



		//read flash value-->efuse value-->one point value
		unsigned char adc_vref_calib_value[7] = {0};
		//load adc vref value from flash
		if(adc_vref_cfg.adc_calib_en)
		{
			flash_read_page(flash_sector_calibration + CALIB_OFFSET_ADC_VREF, 7, adc_vref_calib_value);
			if((adc_vref_calib_value[4] != 0xff) && (((adc_vref_calib_value[6] << 8) | adc_vref_calib_value[5]) != 0xffff)){
				/****** Method of calculating two-point gpio calibration Flash_gain and Flash_offset value: ********/
				/****** Vref = [(Seven_Byte << 8) + Six_Byte + 1000]mv ********/
				/****** offset = [Five_Byte - 20] mv. The range of the offset value must be between -20 and 20.********/
				/****** The range of the offset value must be between -20 and 20.********/
				if(adc_vref_calib_value[4] <= 0x7f){
					adc_vref_cfg.adc_vref = ((adc_vref_calib_value[6] << 8) | adc_vref_calib_value[5]) + 1000;
					adc_vref_cfg.adc_vref_offset = adc_vref_calib_value[4] - 20;
					adc_set_gpio_calib_vref(adc_vref_cfg.adc_vref);
					adc_set_gpio_two_point_calib_offset(adc_vref_cfg.adc_vref_offset);
				}
			}
			else{
				/* if no ADC Vref calibration value on EFUSE, check if ADC Vref one_point calibration value on Flash */
				if(!blt_miscParam.adc_efuse_calib_flag){
					if(((adc_vref_calib_value[1] << 8) | adc_vref_calib_value[0]) != 0xffff)
					{
						/****** Method of calculating one-point calibration Flash_gpio_Vref value: ********/
						/****** Vref = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
						adc_vref_cfg.adc_vref = 920 + adc_vref_calib_value[0] + adc_vref_calib_value[1];
						/****** Check the calibration value whether is correct ********/
						if ((adc_vref_cfg.adc_vref > 1047) && (adc_vref_cfg.adc_vref < 1302)){
							adc_set_gpio_calib_vref(adc_vref_cfg.adc_vref);
						}
						else
						{
							adc_vref_cfg.adc_vref = 1175;//Use default values
						}
					}
				}
			}
		}//if(adc_vref_cfg.adc_calib_en)
	}

}

/**
 * @brief		This function can automatically recognize the flash size,
 * 				and the system selects different customized sector according
 * 				to different sizes.
 * @param[in]	none
 * @return      none
 */
void blc_readFlashSize_autoConfigCustomFlashSector(void);

/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static);




#endif /* BLT_COMMON_H_ */

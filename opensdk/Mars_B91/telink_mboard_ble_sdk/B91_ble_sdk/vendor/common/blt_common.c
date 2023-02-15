/********************************************************************************************************
 * @file     blt_common.c
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

#if 1
#include "tl_common.h"
#include "drivers.h"
#include "blt_common.h"
#include "stack/ble/ble.h"



_attribute_data_retention_	u32 flash_sector_mac_address = CFG_ADR_MAC_1M_FLASH;			//default flash is 1M
_attribute_data_retention_	u32 flash_sector_calibration = CFG_ADR_CALIBRATION_1M_FLASH;	//default flash is 1M




/**
 * @brief		This function can automatically recognize the flash size,
 * 				and the system selects different customized sector according
 * 				to different sizes.
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void blc_readFlashSize_autoConfigCustomFlashSector(void)
{
	u32 temp_buf;
	temp_buf = flash_read_mid();
	u8	flash_cap = (u8)(temp_buf>>16);

	if(flash_cap == FLASH_SIZE_512K){
		flash_sector_mac_address = CFG_ADR_MAC_512K_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_512K_FLASH;
	}
	else if(flash_cap == FLASH_SIZE_1M){
		flash_sector_mac_address = CFG_ADR_MAC_1M_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_1M_FLASH;
	}
	else if(flash_cap == FLASH_SIZE_2M){
		flash_sector_mac_address = CFG_ADR_MAC_2M_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_2M_FLASH;
	}
	else if(flash_cap == FLASH_SIZE_4M){
		flash_sector_mac_address = CFG_ADR_MAC_4M_FLASH;
		flash_sector_calibration = CFG_ADR_CALIBRATION_4M_FLASH;
	}
	else{
		//This SDK do not support flash size other than 1M/2M
		//If code stop here, please check your Flash
		while(1);
	}


	flash_set_capacity(flash_cap);
}




/*
 *B85: 	VVWWXX38C1A4YYZZ
 *B87:  VVWWXXD119C4YYZZ
 *B91:  	VVWWXX
 * public_mac:
 * 				B85 : VVWWXX 38C1A4
 * 				B87 : VVWWXX D119C4
 * 				B91	: VVWWXX
 * random_static_mac: VVWWXXYYZZ C0
 */
/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
	if(flash_sector_mac_address == 0){
		return;
	}


	u8 mac_read[8];
	flash_read_page(flash_addr, 8, mac_read);

	u8 value_rand[5];
	generateRandomNum(5, value_rand);

	u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	if ( memcmp(mac_read, ff_six_byte, 6) ) {
		memcpy(mac_public, mac_read, 6);  //copy public address from flash
	}
	else{  //no public address on flash
		mac_public[0] = value_rand[0];
		mac_public[1] = value_rand[1];
		mac_public[2] = value_rand[2];

		//TODO
		//company id:
		mac_public[3] = 0xD1;             //company id: 0xC119D1
		mac_public[4] = 0x19;
		mac_public[5] = 0xC4;


		flash_write_page (flash_addr, 6, mac_public);
	}





	mac_random_static[0] = mac_public[0];
	mac_random_static[1] = mac_public[1];
	mac_random_static[2] = mac_public[2];
	mac_random_static[5] = 0xC0; 			//for random static

	u16 high_2_byte = (mac_read[6] | mac_read[7]<<8);
	if(high_2_byte != 0xFFFF){
		memcpy( (u8 *)(mac_random_static + 3), (u8 *)(mac_read + 6), 2);
	}
	else{
		mac_random_static[3] = value_rand[3];
		mac_random_static[4] = value_rand[4];

		flash_write_page (flash_addr + 6, 2, (u8 *)(mac_random_static + 3) );
	}
}

//To fix bug
/**
 * @brief      This function is used to request the Controller to start or stop scanning.Not support scanning only.This Api is unavailable.
 * @param[in]  scan_enable - scanning Enable
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setScanEnable (scan_en_t scan_enable, dupFilter_en_t filter_duplicate){return 0;}

#endif

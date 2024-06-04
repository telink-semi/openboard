/********************************************************************************************************
 * @file    sys_norflash.c
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
#include "../../mass_storage/sys_norflash_driver/sys_norflash.h"

#include <stdio.h>
#include <string.h>
#include "driver.h"
#include <nds_intrinsic.h>

#if(SYS_NOR_FLASH_SUPPORT)
volatile unsigned int dtag[256];
volatile unsigned int dcache[16];

static void sys_norflash_clear_dcachetag(void)
{
	unsigned int i;
	__nds__csrw(0,0x7cb);
	for(i = 0; i < 256; i++){
		__nds__csrw(0,0x7cd);
		__nds__csrw(0x15,0x7cc);
	}
}

void sys_norflash_write_lba(unsigned char * buffer, unsigned int lba, unsigned int count)
{
	flash_write_page(SYS_NORFLASH_DISK_ADDRESS + (lba * 512), 512*count, buffer);
	sys_norflash_clear_dcachetag();
}

void sys_norflash_erase_lba(unsigned int lba, unsigned int total_bytes)
{
	unsigned int erase_current = SYS_NORFLASH_DISK_ADDRESS + (lba* 512);
	unsigned int erase_end = erase_current + total_bytes;
	while(erase_current < erase_end)
	{
		if((!(erase_current & 0xfff))&& ((erase_current + 0x1000) <= erase_end))//4k align
		{
			flash_erase_sector(erase_current);
			erase_current += 0x1000;
		}
		else if((!(erase_current & 0xfff)) && ((erase_current + 0x1000) > erase_end))
		{
			unsigned char temp[4096];
            flash_read_data(erase_end, 0x1000 - (erase_end & 0xfff), &temp[erase_end & 0xfff]);
			flash_erase_sector(erase_current & 0xfffff000);
			flash_write_page(erase_end, (0x1000 - (erase_end & 0xfff)), &(temp[erase_end & 0xfff]));
			erase_current += 0x1000;
		}
		else if((erase_current & 0xfff)&&(((erase_current + 0x1000)&0xfffff000) <= erase_end))
		{
			unsigned char temp[4096];
            flash_read_data(erase_current & 0xfffff000, erase_current & 0xfff, temp);
			flash_erase_sector(erase_current & 0xfffff000);
			flash_write_page(erase_current & 0xfffff000, erase_current & 0xfff, temp);
			sys_norflash_clear_dcachetag();
			erase_current = ((erase_current + 0x1000)&0xfffff000);
		}
		else if((erase_current < erase_end) && ((erase_current + 0x1000) > erase_end))
		{
			unsigned char temp[4096];
            flash_read_data(erase_current & 0xfffff000, erase_current & 0xfff, temp);
            flash_read_data(erase_end, 0x1000 - (erase_end & 0xfff), &temp[erase_end & 0xfff]);
			flash_erase_sector(erase_current);
			flash_write_page(erase_current & 0xfffff000, erase_current & 0xfff, temp);
			flash_write_page(erase_end, (0x1000 - (erase_end & 0xfff)), &(temp[erase_end & 0xfff]));
			sys_norflash_clear_dcachetag();
			erase_current += 0x1000;
		}
	}
	sys_norflash_clear_dcachetag();
}

void sys_norflash_read_lba(unsigned char * buffer, unsigned int lba,unsigned int count)
{
    flash_read_data(SYS_NORFLASH_DISK_ADDRESS + (lba * 512), 512 * count, buffer);
}
#endif

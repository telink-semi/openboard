#include <stdio.h>
#include <string.h>
#include "../app_config.h"
#include "sys_norflash.h"

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
	flash_read_page(SYS_NORFLASH_DISK_ADDRESS + (lba * 512), 512 * count, buffer);
}


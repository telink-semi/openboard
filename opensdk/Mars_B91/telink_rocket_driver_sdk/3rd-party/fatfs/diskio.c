/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions */

#include "../../demo/vendor/USB_Demo/app_config.h"
#include "driver.h"

#include "ff.h"			/* Obtains integer types */
#include "../../demo/vendor/USB_Demo/application/mass_storage/spi_sdnand_driver/sdcard.h"
#include "../../demo/vendor/USB_Demo/application/mass_storage/sys_norflash_driver/sys_norflash.h"
/* Definitions of physical drive number for each drive */
#define DEV_NOR_FLASH	0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    pdrv++;
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	unsigned char res=0;
	switch(pdrv)
	{
#if(SYS_NOR_FLASH_SUPPORT)
	case DEV_NOR_FLASH:
				res = 0;//SD_Initialize()
		break;
#endif
#if(SD_NAND_FLASH_SUPPORT)
	case DEV_MMC://SD
		res = 0;//sd_nand_flash_init();//sd_nand_flash_init in msc_app.c, No need to repeat at here
			break;

#endif
		default:
			res=1;
	}
	if(res)return  STA_NOINIT;
	else return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff __attribute__((unused)),		/* Data buffer to store read data */
	LBA_t sector __attribute__((unused)),	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if (!count)return RES_PARERR;
	switch (pdrv) {
#if(SYS_NOR_FLASH_SUPPORT)
		case DEV_NOR_FLASH:
			sys_norflash_read_lba(buff, sector, count);
			return RES_OK;
#endif
#if(SD_NAND_FLASH_SUPPORT)
		case DEV_MMC :

			if(SD_ReadDisk(buff,sector,count))
			{
				return RES_ERROR;
			}
			return RES_OK;

#endif

		default:
			break;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff __attribute__((unused)),	/* Data to be written */
	LBA_t sector __attribute__((unused)),		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	unsigned char res=0;
	if (!count)return RES_PARERR;
	switch (pdrv) {
#if(SYS_NOR_FLASH_SUPPORT)
	case DEV_NOR_FLASH:
		sys_norflash_erase_lba(sector,count*512);
		sys_norflash_write_lba((BYTE *)buff, sector, count);
		res=0;
		break;
#endif
#if(SD_NAND_FLASH_SUPPORT)
	case DEV_MMC :
		if(SD_WriteDisk((unsigned char*)buff,sector,count))
		{
			res=RES_ERROR;
			return res;
		}	
		res=0;
		break;
#endif

		default:
				res=1;
	}
	if(res == 0x00)return RES_OK;
	else return RES_ERROR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
extern void SD_CS_LOW(void);
extern void SD_CS_HIGH(void);
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	unsigned char res = 0;
#if(SD_NAND_FLASH_SUPPORT)
	if(pdrv==DEV_MMC)//SD
	{
		switch(cmd)
		{
			case CTRL_SYNC:
				SD_CS_LOW();
				if(SD_WaitReady()==0)res = RES_OK;
				else res = RES_ERROR;
				SD_CS_HIGH();
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = 8;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = SD_GetSectorCount();
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
#endif
	if(pdrv==DEV_NOR_FLASH)//SD
	{
		switch(cmd)
		{
			case CTRL_SYNC:
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = 512;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = 8;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = SYS_NORFLASH_DISK_SIZE/512;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
	return res;
}
DWORD get_fattime (void)
{
	return 0;
}

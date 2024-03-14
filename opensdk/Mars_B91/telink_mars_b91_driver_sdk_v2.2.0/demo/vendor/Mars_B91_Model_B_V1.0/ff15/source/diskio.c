/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "../../device/sdcard.h"
#include "../../device/sys_norflash.h"
#include "string.h"

/* Definitions of physical drive number for each drive */
#define  DEV_SD_CARD			 			0
#define  DEV_INTERNAL_FLASH 	 			1

#if(3 == FF_VOLUMES)

#define  DEV_INTERNAL_RAM 	 	 			2

#define  DEV_INTERNAL_RAM_SIZE 	 			(64*1024)
#define  DEV_INTERNAL_RAM_SECTOR_SIZE 	 	(512)
#define  DEV_INTERNAL_RAM_BLOCK_SIZE 	 	(8)

volatile BYTE ram_disk_buf[DEV_INTERNAL_RAM_SIZE/DEV_INTERNAL_RAM_SECTOR_SIZE][DEV_INTERNAL_RAM_SECTOR_SIZE];

void VirualRAMDiskRead(BYTE *buff,uint32_t sector,uint32_t count){
	for(uint32_t i = sector; i < sector + count; i++){
		for(uint32_t j = 0; j < DEV_INTERNAL_RAM_SECTOR_SIZE; j++){
			buff[((i - sector)*DEV_INTERNAL_RAM_SECTOR_SIZE) + j] = ram_disk_buf[i][j];
		}
	}
}

void VirualRAMDiskWrite(BYTE *buff,uint32_t sector,uint32_t count){
	for(uint32_t i = sector; i < sector + count; i++){
		for(uint32_t j = 0; j < DEV_INTERNAL_RAM_SECTOR_SIZE; j++){
			ram_disk_buf[i][j] = buff[((i - sector)*DEV_INTERNAL_RAM_SECTOR_SIZE) + j];
		}
	}
}
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv __attribute__((unused))  /* Physical drive nmuber to identify the drive */
)
{
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	switch (pdrv) {
		case DEV_SD_CARD:
			stat = SD_Card_init();
			break;
		case DEV_INTERNAL_FLASH:
			stat = RES_OK;
			break;
#if(DEV_INTERNAL_RAM)
		case DEV_INTERNAL_RAM:
		//	memset((char*)ram_disk_buf,0xff,DEV_INTERNAL_RAM_SIZE);
			stat = RES_OK;
			break;
#endif
		default:
			stat = STA_NOINIT;
			break;
	}
	if(stat) return  STA_NOINIT;
	else return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
  DRESULT res = RES_PARERR;
  if (!count)	return RES_PARERR;
	switch (pdrv) {
		case DEV_SD_CARD :
			res = SD_ReadDisk((BYTE *)buff,sector,count);
		break;
		case DEV_INTERNAL_FLASH:
			sys_norflash_read_lba((BYTE *)buff, sector, count);
			res = RES_OK;
			break;
#if(DEV_INTERNAL_RAM)
		case DEV_INTERNAL_RAM:
			VirualRAMDiskRead((BYTE *)buff,sector,count);
			res = RES_OK;
			break;
#endif
		default:
			res = RES_PARERR;
			break;
	}
    if(res==0x00) return RES_OK;
	return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	switch (pdrv) {
		case DEV_SD_CARD :
			res = SD_WriteDisk((BYTE *)buff,sector,count);
		break;
		case DEV_INTERNAL_FLASH:
			sys_norflash_erase_lba(sector,count*512);
			sys_norflash_write_lba((BYTE *)buff, sector, count);
			res = RES_OK;
			break;
#if(DEV_INTERNAL_RAM)
		case DEV_INTERNAL_RAM:
			VirualRAMDiskWrite((BYTE *)buff,sector,count);
			res = RES_OK;
			break;
#endif
		default:
			res = RES_PARERR;
			break;
	}
    if(res == RES_OK) return RES_OK;
	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	if(pdrv == DEV_SD_CARD)
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
	else if(pdrv == DEV_INTERNAL_FLASH) // Flexible architecture: 4 KB per sector, 64 KB/32 KB per block
	{
		switch(cmd)
		{
			case CTRL_SYNC:
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = 4096;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = 16; // 8/16
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = SYS_NORFLASH_DISK_SIZE/4096;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
#if(DEV_INTERNAL_RAM)
	else if(pdrv == DEV_INTERNAL_RAM)
	{
		switch(cmd)
		{
			case CTRL_SYNC:
				res = RES_OK;
				break;
			case GET_SECTOR_SIZE:
				*(WORD*)buff = DEV_INTERNAL_RAM_SECTOR_SIZE;
				res = RES_OK;
				break;
			case GET_BLOCK_SIZE:
				*(WORD*)buff = DEV_INTERNAL_RAM_BLOCK_SIZE;
				res = RES_OK;
				break;
			case GET_SECTOR_COUNT:
				*(DWORD*)buff = DEV_INTERNAL_RAM_SIZE/DEV_INTERNAL_RAM_SECTOR_SIZE;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
#endif
	else res = RES_ERROR;
	return res;
}


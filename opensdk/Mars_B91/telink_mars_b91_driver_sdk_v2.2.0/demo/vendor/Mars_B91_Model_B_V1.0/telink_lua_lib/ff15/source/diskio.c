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

#include "../extend/tlkfs_disk.h"

/* Definitions of physical drive number for each drive */
//#define  DEV_INTERNAL_FLASH 	 			0
//#define  DEV_SD_CARD			 			1

/******************************************************************************
 * Function: disk_status
 * Descript: Get Drive Status.
 * Params:
 *     @pdrv[IN]--Physical drive nmuber to identify the drive.
 * Return: Refer to DSTATUS.
 * Others: None.
*******************************************************************************/
DSTATUS disk_status(BYTE pdrv)
{
	tlkfs_disk_t *pDisk;
	pDisk = tlkfs_getDisk(pdrv);
	if(pDisk == nullptr) return RES_NOTRDY;
	return RES_OK;
}

/******************************************************************************
 * Function: disk_initialize
 * Descript: Inidialize a Drive.
 * Params:
 *     @pdrv[IN]--Physical drive nmuber to identify the drive.
 * Return: Refer to DSTATUS.
 * Others: None.
*******************************************************************************/
DSTATUS disk_initialize(BYTE pdrv)
{
	tlkfs_disk_t *pDisk;
	pDisk = tlkfs_getDisk(pdrv);
	if(pDisk == nullptr) return RES_NOTRDY;
	if(pDisk->Init == nullptr) return RES_OK;
	if(pDisk->Init() != 0) return RES_ERROR;
	return RES_OK;
}

/******************************************************************************
 * Function: disk_read
 * Descript: Read Sector(s).
 * Params:
 *     @pdrv[IN]--Physical drive nmuber to identify the drive.
 *     @sector[IN]--Start sector in LBA.
 *     @count[IN]--Number of sectors to read.
 *     @buff[OUT]--Data buffer to store read data.
 * Return: Refer to DRESULT.
 * Others: None.
*******************************************************************************/
DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
	tlkfs_disk_t *pDisk;
	pDisk = tlkfs_getDisk(pdrv);
	if(pDisk == nullptr || pDisk->Read == nullptr) return RES_NOTRDY;
	if(pDisk->Read(buff, sector, count) < 0) return RES_ERROR;
	return RES_OK;
}

/******************************************************************************
 * Function: disk_write
 * Descript: Write Sector(s).
 * Params:
 *     @pdrv[IN]--Physical drive nmuber to identify the drive.
 *     @buff[IN]--Data to be written.
 *     @sector[IN]--Start sector in LBA.
 *     @count[IN]--Number of sectors to write.
 * Return: Refer to DRESULT.
 * Others: None.
*******************************************************************************/
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
	tlkfs_disk_t *pDisk;
	pDisk = tlkfs_getDisk(pdrv);
	if(pDisk == nullptr || pDisk->Write == nullptr) return RES_NOTRDY;
	if(pDisk->Write((unsigned char*)buff, sector, count) < 0) return RES_ERROR;
	return RES_OK;
}

/******************************************************************************
 * Function: disk_ioctl
 * Descript: Miscellaneous Functions.
 * Params:
 *     @pdrv[IN]--Physical drive nmuber (0..) .
 *     @cmd[IN]--Control code.
 *     @buff[IN/OUT]--Buffer to send/receive control data.
 * Return: Refer to DRESULT.
 * Others: None.
*******************************************************************************/
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
	DRESULT res;
	tlkfs_disk_t *pDisk;
	pDisk = tlkfs_getDisk(pdrv);
	if(pDisk == nullptr) return RES_NOTRDY;

	switch(cmd){
		case CTRL_SYNC:
			if(pDisk->isReady) res = RES_OK;
			else res = RES_ERROR;
			break;
		case GET_SECTOR_SIZE:
			*(WORD*)buff = pDisk->sectorSize;
			res = RES_OK;
			break;
		case GET_BLOCK_SIZE:
			*(WORD*)buff = pDisk->blockSize;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = pDisk->sectorCount;
			res = RES_OK;
			break;
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}


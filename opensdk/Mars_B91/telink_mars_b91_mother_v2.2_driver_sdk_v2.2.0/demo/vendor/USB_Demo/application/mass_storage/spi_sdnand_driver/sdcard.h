/********************************************************************************************************
 * @file    sdcard.h
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

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"
#include "types.h"
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

#define CMD0    0
#define CMD1    1
#define CMD8    8
#define CMD9    9
#define CMD10   10
#define CMD12   12
#define CMD16   16
#define CMD17   17
#define CMD18   18
#define CMD23   23
#define CMD24   24
#define CMD25   25
#define CMD41   41
#define CMD55   55
#define CMD58   58
#define CMD59   59

#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF



extern unsigned char  SD_Type;




//// add flash_driver_reentry_flag in these function////
unsigned char sd_nand_flash_init(void);
unsigned char SD_ReadDisk(unsigned char*buf,unsigned int sector,unsigned char cnt);
unsigned char SD_WriteDisk(unsigned char*buf,unsigned int sector,unsigned char cnt);
unsigned int SD_GetSectorCount(void);
////////////////////////////////////////////////////////

unsigned char SD_SPI_ReadWriteByte(unsigned char data);
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
unsigned char SD_WaitReady(void);
unsigned char SD_GetResponse(unsigned char Response);

unsigned char SD_GetCID(unsigned char *cid_data);
unsigned char SD_GetCSD(unsigned char *csd_data);
unsigned char SD_WriteDisk_lba512(unsigned char*buf,unsigned int sector);
unsigned char SD_ReadDisk_lba512(unsigned char*buf,unsigned int sector);
unsigned char SD_WriteDisk_lba512_continue(unsigned char*buf,unsigned int sector,unsigned char cnt);
unsigned char SD_SPI_ReadByte();
void sd_read_data(unsigned char *data_buff, unsigned int length);
#ifdef __cplusplus
}
#endif


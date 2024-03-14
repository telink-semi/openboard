/********************************************************************************************************
 * @file    sys_norflash.h
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
#ifndef  __SYS_NORFLASH_H__
#define  __SYS_NORFLASH_H__

#include "../../../app_config.h"
#include <stdio.h>
#define SYS_NORFLASH_DISK_ADDRESS 	0x80000
#define SYS_NORFLASH_DISK_SIZE 		400*1024//400k  Must be greater than or equal to 32K

void sys_norflash_write_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
void sys_norflash_read_lba(unsigned char * buffer, unsigned int lba, unsigned int count);
void sys_norflash_erase_lba(unsigned int lba, unsigned int total_bytes);
#endif

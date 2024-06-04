/********************************************************************************************************
 * @file    codec_0581_eq.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2023
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
/*!
* EQ Address of memories on codec_0581:
* Program: "0xA000"
* Bank 0: "0xA200"
* Bank 1: "0xA400"
*/

#include <stdint.h>

unsigned int eq_program[22] =
{
    0x00010000, 0x00024280, 0x0000C000, 0x00001004, 0x00014304, 0x0000C000, 0x00001004, 0x00014384, 0x0000C000, 0x00001004, 0x00014080, 0x0000C001, 0x0000C202, 0x0000C183, 0x0000C104, 0x0000A004, 0x00001002, 0x00028000, 0x00018000, 0x0003C000, 0x00000000, 0x00000000
};

unsigned int eq_param_bank0[10] =
{
    0x01E65E0D, 0x0F156F8F, 0x01014ECA, 0x0E19A1F3, 0x00E941A7, 0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000
};

unsigned int eq_param_bank1[10] =
{
    0x01E65E0D, 0x0F156F8F, 0x01014ECA, 0x0E19A1F3, 0x00E941A7, 0x01000000, 0x01000000, 0x01000000, 0x01000000, 0x01000000
};

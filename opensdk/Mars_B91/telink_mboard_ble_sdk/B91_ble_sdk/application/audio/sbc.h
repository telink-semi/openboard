/********************************************************************************************************
 * @file     sbc.h
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

#ifndef SBC_H_
#define SBC_H_

#include "../../common/types.h"

typedef signed long long int64_t;
//typedef signed int 	int32_t;
typedef signed int 	ssize_t;
typedef signed short int16_t;
typedef signed char int8_t;
typedef unsigned long long uint64_t;
//typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef signed int sbc_fixed_t;

uint64_t mul64(uint64_t x, unsigned int y);
uint64_t div64(uint64_t x, uint16_t y);
uint64_t shift_left64(uint64_t x, uint8_t bits);

void sbcenc_reset(void);
unsigned int sbc_enc(const unsigned char* buf, uint16_t len, uint8_t *outbuf, unsigned int outbuf_len, unsigned int* out_len);
size_t sbc_decode(const void *input, size_t input_len,void *output, size_t output_len, size_t *written);
/* Returns the output block size in bytes */
size_t sbc_get_frame_length(int type);

/* Returns the input block size in bytes */
size_t sbc_get_codesize(int type);

void msbc_init_ctx();
void msbc_release_ctx();
#endif /* SBC_H_ */

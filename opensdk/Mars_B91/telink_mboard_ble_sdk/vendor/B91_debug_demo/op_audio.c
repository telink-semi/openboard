/********************************************************************************************************
 * @file	op_audio.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "op_audio.h"
#include "drivers.h"
#include <stack/ble/trace.h>
#include "../../application/audio/opus/include/opus.h"


int md_long =0;
int md_short =0;
int md_im =0;
int md_noise = 0;
int md_gain = 256;

static const signed char idxtbl[] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};
static const unsigned short steptbl[] = {
 7,  8,  9,  10,  11,  12,  13,  14,  16,  17,
 19,  21,  23,  25,  28,  31,  34,  37,  41,  45,
 50,  55,  60,  66,  73,  80,  88,  97,  107, 118,
 130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
 337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767   };


#define				NUM_OF_ORIG_SAMPLE				2
#if 0
const unsigned short inbuf[2048] = {
	0xFFED,0xFFDF,0xFFE2,0xFFE0,0xFFDE,0xFFDE,0xFFDE,0xFFDE,
	0xFFDD,0xFFDC,0xFFDD,0xFFDC,0xFFDB,0xFFDB,0xFFDB,0xFFD9,
	0xFFDA,0xFFD9,0xFFD9,0xFFD8,0xFFD8,0xFFD8,0xFFDA,0xFFD9,
	0xFFD6,0xFFD7,0xFFD9,0xFFD6,0xFFD6,0xFFD7,0xFFD7,0xFFD6,
	0xFFD6,0xFFD7,0xFFD7,0xFFD7,0xFFD7,0xFFD7,0xFFD6,0xFFD5,
	0xFFD7,0xFFD8,0xFFD6,0xFFD7,0xFFD6,0xFFD7,0xFFD5,0xFFD5,
	0xFFD6,0xFFD7,0xFFD8,0xFFD6,0xFFD8,0xFFD7,0xFFD8,0xFFD7,
	0xFFD8,0xFFD9,0xFFD9,0xFFD9,0xFFDA,0xFFDB,0xFFDB,0xFFDA,
	0xFFDA,0xFFDA,0xFFDB,0xFFDC,0xFFDD,0xFFDD,0xFFDE,0xFFDC,
	0xFFDC,0xFFDE,0xFFE0,0xFFDF,0xFFE1,0xFFE1,0xFFE0,0xFFE2,
	0xFFE3,0xFFE3,0xFFE4,0xFFE4,0xFFE6,0xFFE6,0xFFE6,0xFFE6,
	0xFFE8,0xFFE8,0xFFE9,0xFFE9,0xFFEB,0xFFEB,0xFFED,0xFFED,
	0xFFEE,0xFFEF,0xFFEF,0xFFF1,0xFFF0,0xFFF3,0xFFF1,0xFFF4,
	0xFFF3,0xFFF5,0xFFF5,0xFFF5,0xFFF7,0xFFF8,0xFFF9,0xFFF7,
	0xFFFA,0xFFF9,0xFFFB,0xFFFC,0xFFFD,0xFFFC,0xFFFE,0x0000,
	0xFFFF,0x0000,0x0002,0x0002,0x0002,0x0002,0x0005,0x0005,
	0x0005,0x0009,0x0005,0x0005,0x0006,0x0007,0x0008,0x0007,
	0x0008,0x0009,0x000A,0x000A,0x000B,0x000B,0x000C,0x000D,
	0x000E,0x000C,0x000E,0x000E,0x000E,0x000F,0x000F,0x0010,
	0x0011,0x0011,0x0011,0x0011,0x0010,0x0012,0x0010,0x0012,
	0x0012,0x0013,0x0014,0x0014,0x0015,0x0014,0x0013,0x0016,
	0x0016,0x0016,0x0013,0x0016,0x0016,0x0016,0x0018,0x0018,
	0x0016,0x0015,0x0017,0x0016,0x0017,0x0018,0x0016,0x0016,
	0x0016,0x0015,0x0016,0x0016,0x0017,0x0016,0x0014,0x0013,
	0x0014,0x0016,0x0013,0x0015,0x0015,0x0016,0x0015,0x0015,
	0x0017,0x0016,0x0017,0x0015,0x0015,0x0015,0x0017,0x0016,
	0x0016,0x0016,0x0016,0x0015,0x0015,0x0016,0x0016,0x0015,
	0x0015,0x0016,0x0015,0x0015,0x0016,0x0014,0x0015,0x0014,
	0x0013,0x0014,0x0015,0x0013,0x0013,0x0012,0x0014,0x0012,
	0x0012,0x000F,0x000E,0x0010,0x000E,0x000F,0x000D,0x0010,
	0x000E,0x000E,0x000C,0x000D,0x000C,0x000B,0x0009,0x000A,
	0x0009,0x0008,0x0008,0x0007,0x0006,0x0006,0x0004,0x0006,
	0x0004,0x0005,0x0001,0x0003,0x0001,0x0002,0x0003,0x0001,
	0xFFFF,0x0000,0x0001,0xFFFE,0xFFFF,0xFFFE,0xFFFE,0xFFFC,
	0xFFFC,0xFFFC,0xFFFC,0xFFFA,0xFFF9,0xFFFA,0xFFF9,0xFFF9,
	0xFFF8,0xFFF8,0xFFF7,0xFFF6,0xFFF6,0xFFF8,0xFFF6,0xFFF5,
	0xFFF5,0xFFF2,0xFFF4,0xFFF4,0xFFF4,0xFFF1,0xFFF2,0xFFF2,
	0xFFF1,0xFFF1,0xFFF1,0xFFEF,0xFFF0,0xFFEF,0xFFF0,0xFFEF,
	0xFFF0,0xFFED,0xFFED,0xFFED,0xFFED,0xFFEC,0xFFEC,0xFFEB,
	0xFFEC,0xFFEC,0xFFEA,0xFFEC,0xFFEA,0xFFEB,0xFFE9,0xFFE9,
	0xFFEB,0xFFEA,0xFFE8,0xFFEA,0xFFE8,0xFFE8,0xFFE7,0xFFE6,
	0xFFE6,0xFFE6,0xFFE5,0xFFE6,0xFFE5,0xFFE4,0xFFE3,0xFFE4,
	0xFFE4,0xFFE3,0xFFE3,0xFFE3,0xFFE4,0xFFE3,0xFFE3,0xFFE2,
	0xFFE1,0xFFE1,0xFFE2,0xFFE0,0xFFE1,0xFFE1,0xFFE2,0xFFE1,
	0xFFE1,0xFFE4,0xFFE3,0xFFE4,0xFFE3,0xFFE3,0xFFE4,0xFFE3,
	0xFFE5,0xFFE4,0xFFE3,0xFFE4,0xFFE5,0xFFE7,0xFFE5,0xFFE7,
	0xFFE5,0xFFE5,0xFFE6,0xFFE8,0xFFE8,0xFFE8,0xFFEA,0xFFE7,
	0xFFE9,0xFFE9,0xFFEA,0xFFE9,0xFFEB,0xFFEA,0xFFEC,0xFFED,
	0xFFEA,0xFFEB,0xFFED,0xFFEE,0xFFEE,0xFFED,0xFFEE,0xFFED,
	0xFFF0,0xFFEE,0xFFEE,0xFFF0,0xFFEF,0xFFEF,0xFFF0,0xFFF1,
	0xFFF1,0xFFF1,0xFFF2,0xFFF2,0xFFF3,0xFFF2,0xFFF2,0xFFF2,
	0xFFF3,0xFFF5,0xFFF3,0xFFF4,0xFFF5,0xFFF4,0xFFF4,0xFFF5,
	0xFFF5,0xFFF6,0xFFF6,0xFFF8,0xFFF9,0xFFF7,0xFFF7,0xFFFA,
	0xFFF9,0xFFFB,0xFFFB,0xFFFA,0xFFFB,0xFFFA,0xFFFB,0xFFFB,
	0xFFFC,0xFFFD,0xFFFE,0xFFFC,0xFFFE,0xFFFD,0xFFFF,0xFFFE,
	0xFFFF,0xFFFF,0xFFFF,0x0000,0x0000,0x0001,0x0001,0x0000,
	0x0002,0x0003,0x0001,0x0004,0x0004,0x0005,0x0004,0x0005,
	0x0005,0x0006,0x0007,0x0006,0x0007,0x0007,0x0008,0x0006,
	0x0008,0x0008,0x0008,0x0009,0x000A,0x000A,0x000A,0x000C,
	0x000B,0x000B,0x000B,0x000C,0x000C,0x000C,0x000D,0x000E,
	0x000D,0x000E,0x000C,0x000F,0x000F,0x000F,0x000D,0x0010,
	0x0010,0x000E,0x0010,0x000F,0x000E,0x000F,0x0010,0x0010,
	0x000F,0x000E,0x0010,0x000E,0x000E,0x000F,0x000F,0x000D,
	0x000E,0x000F,0x000F,0x000E,0x000D,0x000F,0x000F,0x000D,
	0x000C,0x000C,0x000C,0x000C,0x000B,0x000C,0x000A,0x000B,
	0x0009,0x0009,0x0007,0x0009,0x0009,0x000A,0x0008,0x0008,
	0x0008,0x0008,0x0008,0x0006,0x0008,0x0007,0x0008,0x0009,
	0x0006,0x0006,0x0007,0x0006,0x0008,0x0006,0x0007,0x0006,
	0x0004,0x0005,0x0004,0x0006,0x0003,0x0004,0x0004,0x0003,
	0x0004,0x0003,0x0002,0x0000,0x0000,0x0000,0x0000,0x0001,
	0xFFFF,0xFFFD,0xFFFE,0x0000,0xFFFD,0xFFFD,0xFFFC,0xFFFC,
	0xFFFC,0xFFFB,0xFFFC,0xFFFB,0xFFFD,0xFFFB,0xFFFA,0xFFF9,
	0xFFF8,0xFFF9,0xFFF8,0xFFF7,0xFFF7,0xFFF6,0xFFF6,0xFFF4,
	0xFFF5,0xFFF4,0xFFF3,0xFFF4,0xFFF2,0xFFF1,0xFFF1,0xFFF0,
	0xFFF0,0xFFEF,0xFFEE,0xFFED,0xFFEC,0xFFEC,0xFFEA,0xFFE9,
	0xFFEA,0xFFE9,0xFFE8,0xFFE8,0xFFE7,0xFFE7,0xFFE6,0xFFE5,
	0xFFE5,0xFFE4,0xFFE5,0xFFE3,0xFFE2,0xFFE1,0xFFE1,0xFFDE,
	0xFFDE,0xFFDF,0xFFDD,0xFFDD,0xFFDB,0xFFDB,0xFFDB,0xFFD9,
	0xFFD8,0xFFDA,0xFFD8,0xFFD7,0xFFD6,0xFFD7,0xFFD6,0xFFD6,
	0xFFD4,0xFFD4,0xFFD4,0xFFD3,0xFFD3,0xFFD2,0xFFD2,0xFFD0,
	0xFFD2,0xFFD2,0xFFD1,0xFFD0,0xFFD0,0xFFD0,0xFFCF,0xFFD0,
	0xFFCE,0xFFCF,0xFFCE,0xFFCF,0xFFCE,0xFFCD,0xFFCE,0xFFD0,
	0xFFCE,0xFFCE,0xFFCE,0xFFCE,0xFFCF,0xFFD0,0xFFCF,0xFFCE,
	0xFFCE,0xFFD0,0xFFCF,0xFFD0,0xFFCE,0xFFD0,0xFFD0,0xFFD0,
	0xFFCF,0xFFD2,0xFFD1,0xFFD2,0xFFD1,0xFFD1,0xFFD1,0xFFD1,
	0xFFD3,0xFFD1,0xFFD3,0xFFD2,0xFFD2,0xFFD3,0xFFD3,0xFFD3,
	0xFFD1,0xFFD4,0xFFD5,0xFFD5,0xFFD4,0xFFD7,0xFFD6,0xFFD7,
	0xFFD8,0xFFD7,0xFFD6,0xFFD7,0xFFD7,0xFFD7,0xFFD9,0xFFD8,
	0xFFD9,0xFFDA,0xFFDA,0xFFDA,0xFFDA,0xFFDB,0xFFDB,0xFFDC,
	0xFFDD,0xFFDC,0xFFDC,0xFFDC,0xFFDD,0xFFDE,0xFFDE,0xFFDE,
	0xFFDD,0xFFDF,0xFFE1,0xFFE1,0xFFE1,0xFFE1,0xFFE2,0xFFE1,
	0xFFE3,0xFFE3,0xFFE3,0xFFE6,0xFFE4,0xFFE6,0xFFE6,0xFFE8,
	0xFFE8,0xFFE9,0xFFE8,0xFFE9,0xFFE9,0xFFE9,0xFFEA,0xFFEA,
	0xFFEB,0xFFEB,0xFFEC,0xFFEE,0xFFEE,0xFFEE,0xFFEE,0xFFEF,
	0xFFF1,0xFFEE,0xFFF0,0xFFF0,0xFFF1,0xFFF1,0xFFF2,0xFFF3,
	0xFFF3,0xFFF4,0xFFF6,0xFFF3,0xFFF4,0xFFF5,0xFFF4,0xFFF6,
	0xFFF7,0xFFF6,0xFFF6,0xFFF7,0xFFF9,0xFFF8,0xFFFA,0xFFF8,
	0xFFFB,0xFFFA,0xFFFA,0xFFFB,0xFFFB,0xFFFB,0xFFFA,0xFFFC,
	0xFFFD,0xFFFD,0xFFFD,0xFFFD,0xFFFC,0xFFFD,0xFFFF,0xFFFE,
	0xFFFF,0xFFFE,0xFFFD,0xFFFF,0x0000,0x0002,0x0002,0x0000,
	0x0001,0x0002,0x0003,0x0003,0x0003,0x0003,0x0002,0x0005,
	0x0004,0x0004,0x0003,0x0003,0x0003,0x0005,0x0004,0x0006,
	0x0004,0x0006,0x0005,0x0005,0x0004,0x0005,0x0006,0x0006,
	0x0008,0x0007,0x0006,0x0005,0x0008,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0008,0x0008,0x0008,0x0007,0x0009,0x000A,
	0x0009,0x0009,0x000A,0x000A,0x000A,0x000A,0x000A,0x0009,
	0x0009,0x000A,0x0009,0x000A,0x0009,0x0009,0x000A,0x0009,
	0x000A,0x0009,0x0009,0x0009,0x0009,0x0009,0x000A,0x000A,
	0x000A,0x0009,0x0009,0x0008,0x0009,0x0008,0x000A,0x0008,
	0x000A,0x000A,0x0008,0x0009,0x0009,0x0009,0x0008,0x0009,
	0x0009,0x0009,0x0008,0x0007,0x0007,0x0007,0x0009,0x0008,
	0x0007,0x0007,0x0007,0x0006,0x0006,0x0005,0x0005,0x0006,
	0x0006,0x0006,0x0006,0x0005,0x0005,0x0005,0x0004,0x0004,
	0x0004,0x0006,0x0003,0x0003,0x0002,0x0001,0x0002,0x0001,
	0x0002,0x0000,0xFFFF,0xFFFF,0x0001,0x0000,0xFFFF,0x0000,
	0xFFFF,0x0000,0xFFFE,0xFFFF,0xFFFD,0x0000,0xFFFE,0xFFFF,
	0xFFFE,0xFFFF,0xFFFF,0x0000,0xFFFE,0xFFFE,0xFFFE,0xFFFF,
	0xFFFE,0xFFFF,0xFFFE,0xFFFC,0xFFFD,0xFFFC,0xFFFB,0xFFFC,
	0xFFFC,0xFFFB,0xFFFB,0xFFFB,0xFFFC,0xFFFC,0xFFFA,0xFFFB,
	0xFFF9,0xFFFB,0xFFFA,0xFFFA,0xFFFB,0xFFF9,0xFFF9,0xFFF9,
	0xFFFB,0xFFFA,0xFFF8,0xFFF9,0xFFF9,0xFFF9,0xFFF8,0xFFF7,
	0xFFFA,0xFFF9,0xFFF8,0xFFF8,0xFFF7,0xFFF8,0xFFF7,0xFFF7,
	0xFFF5,0xFFF9,0xFFF7,0xFFF8,0xFFF9,0xFFF7,0xFFF7,0xFFF7,
	0xFFF7,0xFFF8,0xFFF9,0xFFF7,0xFFF6,0xFFF7,0xFFF7,0xFFF8,
	0xFFF6,0xFFF5,0xFFF6,0xFFF7,0xFFF6,0xFFF5,0xFFF7,0xFFF8,
	0xFFF8,0xFFF8,0xFFF7,0xFFF8,0xFFF7,0xFFF7,0xFFF7,0xFFF9,
	0xFFF8,0xFFF9,0xFFF9,0xFFF8,0xFFF8,0xFFF8,0xFFF7,0xFFF8,
	0xFFF9,0xFFF8,0xFFFA,0xFFFD,0xFFFC,0xFFFD,0xFFFB,0xFFFD,
	0xFFFD,0xFFFC,0xFFFF,0xFFFD,0xFFFE,0xFFFF,0x0001,0xFFFF,
	0x0001,0x0000,0x0000,0x0002,0x0002,0x0000,0x0001,0x0003,
	0x0003,0x0004,0x0005,0x0005,0x0004,0x0004,0x0004,0x0004,
	0x0004,0x0007,0x0007,0x0008,0x0006,0x0007,0x0008,0x0008,
	0x0008,0x0008,0x0008,0x0009,0x000B,0x000A,0x000B,0x000A,
	0x000B,0x000C,0x000D,0x000D,0x000C,0x000D,0x000D,0x000C,
	0x000D,0x000D,0x000C,0x000E,0x000E,0x000D,0x000E,0x000D,
	0x000F,0x0010,0x000E,0x000E,0x000F,0x0010,0x000F,0x000F,
	0x000F,0x000F,0x0011,0x0011,0x0011,0x0011,0x0010,0x0011,
	0x0011,0x0010,0x0011,0x0013,0x0013,0x0011,0x0013,0x0012,
	0x000F,0x0011,0x0011,0x0012,0x0010,0x0012,0x0012,0x0013,
	0x0013,0x0011,0x0013,0x0012,0x0012,0x0011,0x0013,0x0012,
	0x0013,0x0013,0x0013,0x0013,0x0012,0x0013,0x0014,0x0013,
	0x0014,0x0013,0x0015,0x0012,0x0013,0x0016,0x0016,0x0014,
	0x0014,0x0014,0x0015,0x0013,0x0013,0x0015,0x0014,0x0015,
	0x0014,0x0014,0x0014,0x0015,0x0014,0x0015,0x0013,0x0013,
	0x0014,0x0013,0x0014,0x0015,0x0014,0x0017,0x0014,0x0015,
	0x0016,0x0014,0x0014,0x0014,0x0014,0x0013,0x0016,0x0014,
	0x0015,0x0014,0x0017,0x0014,0x0016,0x0013,0x0015,0x0015,
	0x0013,0x0015,0x0014,0x0013,0x0014,0x0014,0x0013,0x0012,
	0x0016,0x0014,0x0014,0x0015,0x0015,0x0014,0x0014,0x0014,
	0x0013,0x0013,0x0013,0x0013,0x0013,0x0012,0x0013,0x0014,
	0x0014,0x0013,0x0013,0x0014,0x0013,0x0013,0x0013,0x0012,
	0x0014,0x0013,0x0011,0x0012,0x0012,0x0013,0x0013,0x0014,
	0x0015,0x0014,0x0014,0x0011,0x0014,0x0012,0x0014,0x0012,
	0x0014,0x0014,0x0013,0x0012,0x0014,0x0012,0x0013,0x0013,
	0x0010,0x0010,0x0010,0x0011,0x0014,0x0011,0x000F,0x0010,
	0x0010,0x0010,0x0011,0x000F,0x000F,0x000F,0x0010,0x000E,
	0x000F,0x000F,0x000C,0x000E,0x000E,0x000F,0x000E,0x000D,
	0x000D,0x000C,0x000D,0x000D,0x000D,0x000C,0x000B,0x000B,
	0x0009,0x0009,0x000B,0x000B,0x000A,0x000A,0x0008,0x0008,
	0x0009,0x0008,0x0008,0x0009,0x0007,0x0005,0x0006,0x0005,
	0x0004,0x0005,0x0004,0x0003,0x0005,0x0002,0x0001,0x0000,
	0x0000,0x0000,0xFFFE,0xFFFF,0x0000,0xFFFE,0xFFFD,0xFFFC,
	0xFFFC,0xFFFC,0xFFFB,0xFFFA,0xFFFA,0xFFFA,0xFFF9,0xFFF9,
	0xFFF9,0xFFF9,0xFFF7,0xFFF7,0xFFF8,0xFFF8,0xFFF6,0xFFF6,
	0xFFF5,0xFFF5,0xFFF4,0xFFF4,0xFFF4,0xFFF4,0xFFF3,0xFFF2,
	0xFFF2,0xFFF3,0xFFF1,0xFFF4,0xFFF2,0xFFF0,0xFFF0,0xFFF2,
	0xFFF1,0xFFEF,0xFFEF,0xFFED,0xFFEF,0xFFEF,0xFFEF,0xFFF0,
	0xFFEE,0xFFEE,0xFFEE,0xFFED,0xFFED,0xFFEE,0xFFED,0xFFEC,
	0xFFEE,0xFFEE,0xFFED,0xFFEE,0xFFEF,0xFFED,0xFFED,0xFFEC,
	0xFFED,0xFFEE,0xFFED,0xFFEC,0xFFEC,0xFFED,0xFFEE,0xFFED,
	0xFFEE,0xFFED,0xFFED,0xFFED,0xFFEF,0xFFED,0xFFED,0xFFEE,
	0xFFEF,0xFFEE,0xFFEF,0xFFEF,0xFFF0,0xFFF0,0xFFEF,0xFFF0,
	0xFFF0,0xFFF1,0xFFF2,0xFFF1,0xFFF2,0xFFF2,0xFFF2,0xFFF3,
	0xFFF3,0xFFF4,0xFFF5,0xFFF6,0xFFF6,0xFFF6,0xFFF7,0xFFF9,
	0xFFF8,0xFFF8,0xFFFA,0xFFFB,0xFFF9,0xFFFA,0xFFFA,0xFFFB,
	0xFFFA,0xFFFB,0xFFFD,0xFFFD,0xFFFC,0xFFFD,0xFFFD,0xFFFE,
	0xFFFE,0x0000,0x0001,0x0001,0x0000,0x0002,0x0003,0x0002,
	0x0004,0x0004,0x0003,0x0005,0x0007,0x0006,0x0007,0x0008,
	0x0007,0x0008,0x000A,0x0009,0x0009,0x0009,0x0009,0x0009,
	0x000B,0x000E,0x000D,0x000B,0x000B,0x000C,0x000C,0x000D,
	0x000C,0x000C,0x000A,0x000C,0x000B,0x000B,0x000C,0x000A,
	0x000A,0x000B,0x000B,0x000C,0x000A,0x0009,0x000B,0x0009,
	0x0009,0x000B,0x0009,0x000A,0x0007,0x000A,0x0008,0x0009,
	0x0008,0x000A,0x0009,0x0009,0x000A,0x000A,0x0008,0x0009,
	0x000A,0x000B,0x000A,0x000C,0x000C,0x000E,0x000D,0x000C,
	0x000D,0x000C,0x000D,0x000F,0x000D,0x000C,0x000F,0x000F,
	0x000F,0x000E,0x000E,0x000F,0x000F,0x000F,0x000E,0x000E,
	0x000E,0x000E,0x000F,0x000C,0x000E,0x000E,0x000D,0x000F,
	0x000F,0x0010,0x000F,0x000F,0x0011,0x000F,0x0010,0x0010,
	0x000F,0x0011,0x0010,0x0010,0x0010,0x0010,0x0012,0x0011,
	0x0010,0x000E,0x0011,0x0011,0x000F,0x0012,0x0012,0x0013,
	0x0012,0x0014,0x0015,0x0013,0x0012,0x0014,0x0017,0x0017,
	0x0017,0x0018,0x0017,0x001A,0x0018,0x001B,0x001A,0x001A,
	0x0019,0x0019,0x001A,0x0017,0x0019,0x0016,0x0015,0x0015,
	0x0013,0x0013,0x0013,0x0013,0x0013,0x0013,0x0012,0x0014,
	0x0015,0x0013,0x0013,0x0015,0x0016,0x0018,0x0016,0x0019,
	0x0018,0x0018,0x0018,0x0016,0x0017,0x0018,0x0016,0x0015,
	0x0015,0x0014,0x0015,0x0015,0x0014,0x0014,0x0015,0x0012,
	0x0011,0x0012,0x0012,0x0010,0x0013,0x0013,0x0011,0x0013,
	0x0013,0x0010,0x0011,0x0010,0x0010,0x0010,0x0010,0x000E,
	0x000F,0x0010,0x000D,0x000E,0x000B,0x000D,0x000C,0x000B,
	0x000B,0x000D,0x000B,0x000C,0x000B,0x000A,0x000A,0x000B,
	0x000C,0x000D,0x000F,0x000F,0x000D,0x000E,0x000D,0x0010,
	0x000E,0x000F,0x000C,0x000D,0x000F,0x0010,0x0010,0x000E,
	0x000E,0x000D,0x000F,0x000F,0x000B,0x000C,0x0008,0x000C,
	0x000C,0x000D,0x0008,0x000A,0x0003,0x0006,0x0008,0x000C,
	0x000D,0x000A,0x0009,0x0009,0x0004,0x0007,0x0004,0x0014,
	0x0016,0x0014,0x000D,0x000A,0x0009,0x0003,0x0003,0x0007,
	0x0008,0x0009,0x0009,0x000E,0x0005,0x0005,0x0006,0x0009,
	0x0007,0x000C,0x0009,0x0009,0x0007,0x0007,0x0006,0x0003,
	0x0003,0x0007,0x0008,0x0012,0x000E,0x0011,0x000D,0x0083,
	0x00AE,0x009F,0x0058,0x0034,0x0050,0x002E,0x0048,0x00B5,
	0x00CB,0x0099,0x0086,0x005D,0x0022,0x000B,0x000E,0xFFFC,
	0x0000,0x0016,0x000B,0x000A,0xFFF9,0xFFE6,0xFFE6,0xFFCC,
	0xFFBC,0xFFD9,0xFFD0,0xFFE2,0x000B,0x0016,0xFFFD,0xFFEC,
	0xFFE7,0xFFED,0xFFFB,0xFFF6,0x000C,0x000D,0xFFFA,0xFFF7,
	0x0001,0xFFF8,0x0000,0xFFF9,0xFFF3,0xFFF2,0xFFEF,0xFFF3,
	0x0006,0x0012,0x0023,0x0015,0x0027,0x001C,0x0017,0x0016,
	0x001E,0x0024,0x0028,0x0024,0x002E,0x0031,0x0026,0x0024,
	0x002A,0x001D,0x0025,0x002A,0x0032,0x002F,0x002C,0x0012,
	0x0006,0x0004,0x0007,0x0011,0x0014,0x001D,0x0009,0x0011,
	0x0007,0x000A,0x0013,0x0001,0x000D,0x0000,0x0003,0x000A,
	0x0016,0x0014,0x001A,0x000D,0x0013,0x000B,0x0006,0x001B,
	0x0015,0x0017,0x0012,0x0017,0x0020,0x0018,0x0012,0x0004,
	0xFFFB,0x000C,0x0012,0x001F,0x003A,0x0035,0x0022,0x0028,
	0x0036,0x0026,0x003E,0x003E,0x0042,0x004F,0x0057,0x0056,
	0x0058,0x004E,0x003A,0x001B,0x000E,0x000A,0x0011,0x0005,
	0xFFE6,0x0005,0xFFF6,0xFFDF,0xFFE9,0xFFE3,0xFFD5,0xFFDE,
	0xFFD4,0xFFD4,0xFFE5,0xFFE1,0xFFF0,0xFFFB,0xFFF1,0xFFE2,
	0x0021,0x000D,0x000E,0x0012,0x0014,0x003E,0x0027,0x0024,
	0x0023,0x0028,0x0002,0x001A,0xFFFF,0x0015,0x0044,0x0014,
	0xFFF3,0xFFF1,0x0038,0x0028,0x0017,0x0015,0x000C,0xFFDB,
	0xFFD6,0xFFDA,0xFFCE,0xFFCE,0xFFCE,0xFFD3,0xFFCF,0xFFC0,
	0xFFB8,0xFF9B,0xFFA1,0xFF97,0xFF8B,0xFF9D,0xFFAF,0xFFEA,
	0x000E,0x000D,0x000E,0x0024,0x0014,0x0011,0xFFFD,0xFFFE,
	0x0015,0x000F,0x0014,0x0021,0x0022,0x0025,0x0013,0xFFF9,
	0xFFF4,0xFFFD,0x000B,0x000B,0x0006,0x000D,0x0003,0xFFFC,
	0xFFFF,0xFFF4,0x0008,0x001B,0x0018,0x0024,0x0020,0x0016,
	0x0018,0x000C,0xFFFF,0x0018,0x0019,0x000C,0x000B,0x0002,
	0x0002,0x0016,0x0027,0x0022,0x0033,0x0036,0x0013,0x0024,
	0x0036,0x0033,0x0034,0x0035,0x0036,0x0048,0x0036,0x0029,
	0x0033,0x0028,0x0024,0x0034,0x0033,0x0022,0x000E,0xFFE5,
	0xFFD4,0xFFD8,0xFFD2,0xFFCE,0xFFD0,0xFFC8,0xFFBF,0xFFC1,
	0xFFAB,0xFF97,0xFF86,0xFF7D,0xFF77,0xFF69,0xFF5C,0xFF58,
	0xFF49,0xFF48,0xFF30,0xFEFC,0xFEDD,0xFED1,0xFEC4,0xFEBF,
	0xFEC9,0xFEB9,0xFEA5,0xFE91,0xFE85,0xFE70,0xFE40,0xFE17,
	0xFE04,0xFDF9,0xFDFA,0xFDEF,0xFDDE,0xFDE3,0xFDDB,0xFDB5,
	0xFD9F,0xFDA0,0xFD95,0xFD9D,0xFDBF,0xFDE8,0xFE33,0xFE7C,
	0xFEBD,0xFEE1,0xFEF7,0xFF2A,0xFF79,0xFFB6,0x0008,0x006F,
	0x00CF,0x013D,0x01A6,0x01F3,0x0230,0x0247,0x0256,0x026A,
	0x0287,0x02A7,0x02C7,0x02DC,0x02CF,0x02BD,0x02B8,0x02A0,
	0x027A,0x0257,0x0229,0x0206,0x01EB,0x01FF,0x01FF,0x01DB,
	0x01C1,0x01AF,0x017C,0x0154,0x0146,0x012E,0x011C,0x0123,
	0x0132,0x011F,0x0105,0x00EE,0x00C6,0x008B,0x005B,0x003A,
	0x0023,0x000E,0x000C,0xFFF4,0xFFCF,0xFFAC,0xFF80,0xFF50,
	0xFF30,0xFF00,0xFEDA,0xFEB9,0xFE88,0xFE62,0xFE50,0xFE45,

};
#endif


#ifndef		ADPCM_PACKET_LEN
#define		ADPCM_PACKET_LEN					128
#endif

#if		TL_MIC_BUFFER_SIZE

#define	BUFFER_PACKET_SIZE		((ADPCM_PACKET_LEN >> 2) * TL_MIC_PACKET_BUFFER_NUM)

int		buffer_mic_enc[BUFFER_PACKET_SIZE];
u8		buffer_mic_pkt_wptr;
u8		buffer_mic_pkt_rptr;

u32		adb_t2;


#define TL_NOISE_SUPRESSION_ENABLE 0 // TODO : too much calculation can have packet drop
#if 	IIR_FILTER_ENABLE
int c1[5] = {5751, 895, 1010, 253, -187};//filter all 
int c2[5] = {4294, -6695, 3220, 1674, -855};//filter 1.2khz 
int c3[5] = {4739, -2293, 1254, 573, -474};//filter 4khz
int filter_1[10];
int filter_2[10];
int filter_3[10];
u8  filter1_shift;
u8  filter2_shift;
u8  filter3_shift;

void voice_iir (signed short * ps, signed short *pd, int* coef, int nsample,u8 shift)
{
      int i = 0;
      int s = 0;
      for (i=0; i<nsample; i++)
      {
            s = (*ps * coef[0])>>shift;                  //input 16-bit
            s += coef[5] * coef[1];
            s += coef[6] * coef[2];       //coef 0,1,2: 12-bit
            s += coef[7] * coef[3];
            s += coef[8] * coef[4];      //coef 4 & 5: 10-bit; coef 7 & 8: 18-bit
            s = s >> 10;                        //18-bit
            if (s >= (1<<18))
                  s = (1<<18) - 1;
            else if (s < -(1<<18))
                  s = - (1<<18);
            coef[6] = coef[5];                  //16-bit
            coef[5] = *ps++;              //16-bit
            coef[8] = coef[7];                  //18-bit
            coef[7] = s;
            *pd++ = s >> 2;
      }
}
#endif
#if ADPCM
void	proc_mic_encoder (void)
{
	static u16	buffer_mic_rptr;
	u16 mic_wptr = (audio_get_mic_buffer_write_pointer (DMA2) - (u32)buffer_mic) >> 1;

	u16 l = (mic_wptr >= buffer_mic_rptr) ? (mic_wptr - buffer_mic_rptr) : 0xffff;

	if (l >=(TL_MIC_BUFFER_SIZE>>3)) {
		s16 *ps = buffer_mic + buffer_mic_rptr;
#if 	TL_NOISE_SUPRESSION_ENABLE
        // for FIR adc sample data, only half part data are effective
		for (int i=0; i<TL_MIC_ADPCM_UNIT_SIZE*2; i++) {
			ps[i] = noise_supression (ps[i]);
        }
#endif
#if 	IIR_FILTER_ENABLE
		extern u8 mic_start_flag;
		if(mic_start_flag){
			mic_start_flag =0;
			memset(filter_1,0,sizeof(filter_1));
			memset(filter_2,0,sizeof(filter_2));
			memset(filter_3,0,sizeof(filter_3));
		}
		memcpy(filter_1,c1,sizeof(c1));
		memcpy(filter_2,c2,sizeof(c2));
		memcpy(filter_3,c3,sizeof(c3));
		#if 1
		voice_iir(ps,ps,filter_2,(TL_MIC_BUFFER_SIZE>>2),filter2_shift);
		voice_iir(ps,ps,filter_3,(TL_MIC_BUFFER_SIZE>>2),filter3_shift);
		voice_iir(ps,ps,filter_1,(TL_MIC_BUFFER_SIZE>>2),filter1_shift);
		#endif
#endif 
		mic_to_adpcm_split (	ps,	TL_MIC_ADPCM_UNIT_SIZE,
						(s16 *)(buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
						(buffer_mic_pkt_wptr & (TL_MIC_PACKET_BUFFER_NUM - 1))), 1);

//		buffer_mic_rptr = buffer_mic_rptr ? 0 : (TL_MIC_BUFFER_SIZE>>2);
		buffer_mic_rptr = buffer_mic_rptr+(TL_MIC_BUFFER_SIZE>>3);
		if(buffer_mic_rptr>=(TL_MIC_BUFFER_SIZE>>1))
		{
			buffer_mic_rptr=0;
		}
		buffer_mic_pkt_wptr++;
		int pkts = (buffer_mic_pkt_wptr - buffer_mic_pkt_rptr) & (TL_MIC_PACKET_BUFFER_NUM*2-1);
		if (pkts > TL_MIC_PACKET_BUFFER_NUM) {
			buffer_mic_pkt_rptr++;
//			log_event (TR_T_adpcm_enc_overflow);
		}

//		log_task_end (TR_T_adpcm);
	}
}

int	*	mic_encoder_data_buffer ()
{
	if (buffer_mic_pkt_rptr == buffer_mic_pkt_wptr) {
			return 0;
	}

	int *ps = buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
			(buffer_mic_pkt_rptr & (TL_MIC_PACKET_BUFFER_NUM - 1));


	return ps;
}

void mic_encoder_data_read_ok (void)
{
	buffer_mic_pkt_rptr++;
}
#else
OpusEncoder* enc = NULL;
void opus_related_init(void)
{
	extern OpusEncoder * opus_para_init();
	enc = opus_para_init();
}
void	proc_mic_encoder (void)
{

	static u16	buffer_mic_rptr;

	u16 mic_wptr = (audio_get_rx_dma_wptr (DMA2) - (u32)buffer_mic) >> 1;

	u16 l = (mic_wptr >= buffer_mic_rptr) ? (mic_wptr - buffer_mic_rptr) : 0xffff;

	if (l >=(TL_MIC_BUFFER_SIZE>>2)) {
		s16 *ps = buffer_mic + buffer_mic_rptr;
#if 	TL_NOISE_SUPRESSION_ENABLE
        // for FIR adc sample data, only half part data are effective
		for (int i=0; i<TL_MIC_ADPCM_UNIT_SIZE*2; i++) {
			ps[i] = noise_supression (ps[i]);
        }
#endif
#if 	IIR_FILTER_ENABLE
		extern u8 mic_start_flag;
		if(mic_start_flag){
			mic_start_flag =0;
			memset(filter_1,0,sizeof(filter_1));
			memset(filter_2,0,sizeof(filter_2));
			memset(filter_3,0,sizeof(filter_3));
		}
		memcpy(filter_1,c1,sizeof(c1));
		memcpy(filter_2,c2,sizeof(c2));
		memcpy(filter_3,c3,sizeof(c3));
		#if 1
		voice_iir(ps,ps,filter_2,(TL_MIC_BUFFER_SIZE>>2),filter2_shift);
		voice_iir(ps,ps,filter_3,(TL_MIC_BUFFER_SIZE>>2),filter3_shift);
		voice_iir(ps,ps,filter_1,(TL_MIC_BUFFER_SIZE>>2),filter1_shift);
		#endif
#endif
#if   1
		opus_encode(enc, ps, 320, (u8 *)(buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
				(buffer_mic_pkt_wptr & (TL_MIC_PACKET_BUFFER_NUM - 1))), 320);
#else
		mic_to_adpcm_split (	ps,	TL_MIC_ADPCM_UNIT_SIZE,
						(s16 *)(buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
						(buffer_mic_pkt_wptr & (TL_MIC_PACKET_BUFFER_NUM - 1))), 1);
#endif
		buffer_mic_rptr = buffer_mic_rptr+(TL_MIC_BUFFER_SIZE>>2);
		if(buffer_mic_rptr>=(TL_MIC_BUFFER_SIZE>>1))
		{
			buffer_mic_rptr=0;
		}
		buffer_mic_pkt_wptr++;
		int pkts = (buffer_mic_pkt_wptr - buffer_mic_pkt_rptr) & (TL_MIC_PACKET_BUFFER_NUM*2-1);
		if (pkts > TL_MIC_PACKET_BUFFER_NUM) {
			buffer_mic_pkt_rptr++;
//			log_event (TR_T_adpcm_enc_overflow);
		}

//		log_task_end (TR_T_adpcm);
	}
}

int	*	mic_encoder_data_buffer ()
{
	if (buffer_mic_pkt_rptr == buffer_mic_pkt_wptr) {
			return 0;
	}

	int *ps = buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
			(buffer_mic_pkt_rptr & (TL_MIC_PACKET_BUFFER_NUM - 1));


	return ps;
}

void mic_encoder_data_read_ok (void)
{
	buffer_mic_pkt_rptr++;
}
#endif

#if 0
void	proc_mic_encoder (void)
{
	u32 t = clock_time ();
	static u16	buffer_mic_rptr;
	u16 mic_wptr = reg_audio_wr_ptr;
	u16 l = ((mic_wptr<<1) - buffer_mic_rptr) & ((TL_MIC_BUFFER_SIZE>>1) - 1);
	if (l >= 128) {
		log_task_begin (TR_T_adpcm);
		s16 *ps = buffer_mic + buffer_mic_rptr;

#if 	TL_NOISE_SUPRESSION_ENABLE
		for (int i=0; i<128; i++) {
			ps[i] = noise_supression (ps[i] >> 16) << 16;
		}
#endif
		mic_to_adpcm_split (	ps,	128,
						(s16 *)(buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
						((buffer_mic_pkt_wptr>>1) & (TL_MIC_PACKET_BUFFER_NUM - 1))), !(buffer_mic_pkt_wptr&1));

		buffer_mic_rptr = (buffer_mic_rptr + 128) & ((TL_MIC_BUFFER_SIZE>>1) - 1);
		buffer_mic_pkt_wptr++;
		int pkts = ((buffer_mic_pkt_wptr>>1) - buffer_mic_pkt_rptr) & (TL_MIC_PACKET_BUFFER_NUM*2-1);
		if (pkts > TL_MIC_PACKET_BUFFER_NUM) {
			buffer_mic_pkt_rptr++;
//			log_event (TR_T_adpcm_enc_overflow);
		}
		adb_t2 = clock_time() - t;
		log_task_end (TR_T_adpcm);
	}
}


int	*	mic_encoder_data_buffer ()
{
	if ((buffer_mic_pkt_rptr & 0x7f) == (buffer_mic_pkt_wptr >> 1)) {
			return 0;
	}

	int *ps = buffer_mic_enc + (ADPCM_PACKET_LEN>>2) *
			(buffer_mic_pkt_rptr & (TL_MIC_PACKET_BUFFER_NUM - 1));

	buffer_mic_pkt_rptr++;

	return ps;
}
#endif

#endif

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
//	hardware dependent
/////////////////////////////////////////////////////////////
#if TL_SDM_BUFFER_SIZE

int		buffer_sdm_wptr;
int		buffer_sdm_dec[ADPCM_PACKET_LEN];

void adpcm_to_sdm (signed short *ps, int len){
	int i;
	int predict_idx = 1;
	int predict=0;

	unsigned char *pcode = (unsigned char *) (ps + 12);
	unsigned char code=0;

	for (i=0; i<len; i++) {

		if (i) {
			int step = steptbl[predict_idx];

			int diffq = step >> 3;

			if (code & 4) {
				diffq = diffq + step;
			}
			step = step >> 1;
			if (code & 2) {
				diffq = diffq + step;
			}
			step = step >> 1;
			if (code & 1) {
				diffq = diffq + step;
			}

			if (code & 8) {
				predict = predict - diffq;
			}
			else {
				predict = predict + diffq;
			}

			if (predict > 32767) {
				predict = 32767;
			}
			else if (predict < -32767) {
				predict = -32767;
			}

			predict_idx = predict_idx + idxtbl[code & 15];

			if(predict_idx < 0) {
				predict_idx = 0;
			}
			else if(predict_idx > 88) {
				predict_idx = 88;
			}

			if (i&1) {
				code = *pcode ++;
			}
			else {
				code = code >> 4;
			}
		}
		else {
			code = *pcode++ >> 4;
			predict = ps[0];
		}

		int t2;
		if (i < 8) {
			t2 = ps[i];
		}
		else {
			t2 = predict;
		}
		//* ((s16 *) (buffer_sdm + buffer_sdm_wptr)) = t2;
		buffer_sdm[buffer_sdm_wptr] = (t2<<0);
		buffer_sdm_wptr = (buffer_sdm_wptr + 1) & ((TL_SDM_BUFFER_SIZE>>1) - 1);
	}
}

void pcm_to_sdm (signed short *ps, int len){
	for (int i=0; i<len; i++) {
		buffer_sdm[buffer_sdm_wptr] = ps[i];
		buffer_sdm_wptr = (buffer_sdm_wptr + 1) & ((TL_SDM_BUFFER_SIZE>>1) - 1);
	}
}

void silence_to_sdm (void){
	for (int i=0; i<TL_SDM_BUFFER_SIZE>>1; i++) {
		* ((s16 *) (buffer_sdm + i)) = 0;
	}
}

int  sdm_decode_ready (int nbyte_to_decode)
{
	u16 sdm_rptr = reg_aud_rptr; //get_sdm_rd_ptr ();
	u16 num = ((buffer_sdm_wptr>>1) - sdm_rptr) & ((TL_SDM_BUFFER_SIZE>>2) - 1);
	return ((nbyte_to_decode>>2) + num) < (TL_SDM_BUFFER_SIZE >> 2);
}

int   sdm_word_in_buffer ()
{
	u16 num = ((buffer_sdm_wptr>>1) - reg_aud_rptr) & ((TL_SDM_BUFFER_SIZE>>2) - 1);
	return num;
}

void  sdm_decode_rate (int step, int adj)
{
	u16 sdm_rptr = reg_aud_rptr; //get_sdm_rd_ptr ();
	u16 num = ((buffer_sdm_wptr>>1) - sdm_rptr) & ((TL_SDM_BUFFER_SIZE>>2) - 1);

	if (num > (TL_SDM_BUFFER_SIZE*3>>5)) {
		reg_ascl_step = step + adj;
	}
	else if (num < (TL_SDM_BUFFER_SIZE>>4)) {
		reg_ascl_step = step - adj;
	}
}


void proc_sdm_decoder (void)
{

}

int  sdm_decode_data (int *ps, int nbyte)
{
	return 0;
}

#endif


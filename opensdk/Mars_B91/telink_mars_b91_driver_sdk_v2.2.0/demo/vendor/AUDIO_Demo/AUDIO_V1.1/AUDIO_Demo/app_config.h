/********************************************************************************************************
 * @file    app_config.h
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

#include "driver.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define LED1            GPIO_PD0
#define LED2            GPIO_PD1
#define LED3            GPIO_PE6
#define LED4            GPIO_PE7
/**
 *  codec has 2  input data streams
 *  stream 0 support line_in/amic_in/dmic0_in
 *  stream 1 support dmic1_in
 */
#define    LINE_INPUT_TO_BUF_TO_LINEOUT        1  //stream 0
#define	   AMIC_INPUT_TO_BUF_TO_LINEOUT        2  //stream 0
#define	   DMIC0_INPUT_TO_BUF_TO_LINEOUT       3  //stream 0
#define	   DMIC1_INPUT_TO_BUF_TO_LINEOUT       4  //stream 1
#define	   AMIC_AND_DMIC1_INPUT_TO_TWO_BUF     5  //amic stream 0 and dmic1 stream 1 to two buff independently
#define	   DMIC0_AND_DMIC1_INPUT_TO_ONE_BUF    6  //dmic0 stream 0 and dmic1 stream 1 share one fifo  to one buff
#define    BUFFER_TO_LINEOUT                   7  //buff to line_out
#define    DMA_IRQ_Test                        8  //dma llp_irq
/**
 * there has 2 i2s
 */
#define    TWO_I2S_ALIGN                       9 //Two I2S align function share a fifo and buff
#define    I2S_OUTPUT_DOUBLE_BUFF               10 /*I2S(I2S0 or I2S1) outputs double buff data: buff_L-->fifo0-->I2S0_L/I2S1_L,buff_R-->fifo1-->I2S0_R/I2S1_R*/
#define    AUDIO_FIFO_IRQ_Test                 11 //fifo threshold irq
#define    I2S_TO_EXT_CODEC_WM                 12 //I2S input->buff->I2S out for WM8731 external codec

#define    AUDIO_MODE                    BUFFER_TO_LINEOUT

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

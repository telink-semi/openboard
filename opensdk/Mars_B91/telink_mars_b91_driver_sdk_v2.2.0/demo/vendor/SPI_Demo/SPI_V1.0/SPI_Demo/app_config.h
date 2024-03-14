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
#if defined(__cplusplus)
extern "C" {
#endif


/* Enable C linkage for C++ Compilers: */


#define LED1            GPIO_PB4//do not use it when set PB4 set as spi mode
#define LED2            GPIO_PB5
#define LED3            GPIO_PB6//do not use it when set PB6 set as spi mode
#define LED4            GPIO_PB7


#define SPI_NDMA_MODE              1
#define SPI_DMA_MODE               2
#define SPI_XIP_MODE               3

#define SPI_MODE                   SPI_NDMA_MODE
#define	CCLK_96M_HCLK_48M_PCLK_24M	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV2_TO_CCLK, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

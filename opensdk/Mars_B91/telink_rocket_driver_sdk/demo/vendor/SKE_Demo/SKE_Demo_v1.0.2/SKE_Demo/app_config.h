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
#ifndef __APP_CONFIG__H_
#define __APP_CONFIG__H_
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C"
{
#endif
#include "driver.h"
#include "common.h"

#define SKE_SAMPLE_TEST                    1
#define SKE_TEST                           2
#define SKE_CCM_TEST                       3
#define SKE_GCM_TEST                       4
#define SKE_GMAC_TEST                      5


#define	 SKE_TEST_MODE                     SKE_SAMPLE_TEST

#ifdef SKE_LP_DMA_FUNCTION
extern int buf_ske_dma[0xC00];
#define DMA_RAM_BASE                        (buf_ske_dma)           //just for temporary use
#endif /* SKE_LP_DMA_FUNCTION */




/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif

#endif /* __APP_CONFIG__H_ */

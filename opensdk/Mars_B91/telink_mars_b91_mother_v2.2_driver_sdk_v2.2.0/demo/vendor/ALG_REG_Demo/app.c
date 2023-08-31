/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for B91m
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
#include "app_config.h"

volatile unsigned char normal_tx_buffer[5] = {0xac, 0xbc, 0xcc, 0xdc, 0xec};
volatile unsigned char normal_rx_buffer[8] = {0x00};
volatile unsigned char normal_rx_buffer1[8] = {0x00};
volatile unsigned short normal_rx_hword;
volatile unsigned short normal_rx_hword1;
volatile unsigned int normal_rx_word  __attribute__((aligned(4)))= 0x00000000;

volatile unsigned int var_dma_tx_word[1] = {0xaabbccdd};
volatile unsigned int var_dma_rx_word[1] = {0x00};

volatile unsigned char var_dma_write_addr_data[8] __attribute__((aligned(4))) = {0x39, 0xbb, 0x3a, 0xcc, 0x3b, 0xdd, 0x3c, 0xee};

volatile unsigned short NN;

void user_init(void)
{
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_set_low_level(LED1);
	gpio_set_low_level(LED2);

#if(ALG_MODE == ALG_BYTE_MODE)
	gpio_toggle(LED2);
	analog_write_reg8(0x38, 0x11);
	analog_write_reg8(0x39, 0x22);
	analog_write_reg8(0x3a, 0x33);
	analog_write_reg8(0x3b, 0x44);
	gpio_toggle(LED2);
	normal_rx_buffer[0] = analog_read_reg8(0x38);
	normal_rx_buffer[1] = analog_read_reg8(0x39);
	normal_rx_buffer[2] = analog_read_reg8(0x3a);
	normal_rx_buffer[3] = analog_read_reg8(0x3b);
	gpio_toggle(LED2);
#elif(ALG_MODE == ALG_HWORD_MODE)
	gpio_toggle(LED2);
	analog_write_reg16(0x38, 0xccdd);
	analog_write_reg16(0x3a, 0xeeff);
	gpio_toggle(LED2);
	normal_rx_hword = analog_read_reg16(0x38);
	normal_rx_hword1 = analog_read_reg16(0x3a);
	gpio_toggle(LED2);
#elif(ALG_MODE == ALG_WORD_MODE)
	gpio_toggle(LED2);
	analog_write_reg32(0x38, 0xaabbccdd);
	gpio_toggle(LED2);
	normal_rx_word=analog_read_reg32(0x38);
	gpio_toggle(LED2);
#elif(ALG_MODE == ALG_BUFF_MODE)
	gpio_toggle(LED2);
	analog_write_buff(0x38, (unsigned char *)normal_tx_buffer, 4);
	gpio_toggle(LED2);
	analog_read_buff(0x38, (unsigned char *)normal_rx_buffer, 4);
	gpio_toggle(LED2);
#elif(ALG_MODE==ALG_DMA_WORD_MODE)
	gpio_toggle(LED2);
	analog_write_reg32_dma(DMA0, 0x38, (unsigned int*)var_dma_tx_word);
	gpio_toggle(LED2);
	analog_read_reg32_dma(DMA0, 0x38, (unsigned int*)var_dma_rx_word);
	gpio_toggle(LED2);
#elif(ALG_MODE==ALG_DMA_BUFF_MODE)
	gpio_toggle(LED2);
	analog_write_buff_dma(DMA0, 0x38, (unsigned char*)normal_tx_buffer, 4);
	gpio_toggle(LED2);
	analog_read_buff_dma(DMA0, 0x38, (unsigned char*)normal_rx_buffer, 4);
	gpio_toggle(LED2);
#elif(ALG_MODE==ALG_DMA_ADDR_DATA_MODE)
	gpio_toggle(LED2);
	analog_write_addr_data_dma(DMA0, (unsigned char*)var_dma_write_addr_data, 8);
	gpio_toggle(LED2);
	normal_rx_word=analog_read_reg32(0x39);
	gpio_toggle(LED2);
#endif
}

void main_loop(void)
{
	 delay_ms(500);
	 gpio_toggle(LED1);
	 NN++;
}


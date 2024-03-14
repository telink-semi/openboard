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

#if(AES_MODE == AES_CRYPT_MODE)
#define CRYPT_NORMAL_MODE  	0
#define CRYPT_IRQ_MODE  	1

#define CRYPT_MODE			0
#endif


unsigned char sKey[16] __attribute__((aligned(4))) = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
unsigned char plaintext[16] __attribute__((aligned(4))) = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};


unsigned char ciphertext[16] __attribute__((aligned(4))) = {0};
unsigned char deciphertext[16] __attribute__((aligned(4))) = {0};

unsigned char cipher[16] __attribute__((aligned(4))) = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30, 0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

volatile unsigned char irq_flag = 0;

volatile unsigned char irk_value = 0;
unsigned char rpa_data[112] __attribute__((aligned(4))) = {
													0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
										 	 	 	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
													0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
													0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
													0x71, 0x4a, 0x57, 0x3d, 0xf6, 0x88, 0x69, 0x0c, 0x57, 0x98, 0x50, 0x51, 0x82, 0xf5, 0x2a, 0xa0,
													0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
													0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f};


void user_init()
{
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_input_dis(LED1);
	gpio_input_dis(LED2);
	gpio_set_low_level(LED1);
	gpio_set_low_level(LED2);

#if(AES_MODE == INTERRUPT_MODE)

#if(MCU_CORE_B91)
	core_interrupt_enable();
	plic_interrupt_enable(IRQ12_ZB_DM);
	aes_set_irq_mask(FLD_CRYPT_IRQ);
#elif(MCU_CORE_B92)
	core_interrupt_enable();
	plic_interrupt_enable(IRQ14_ZB_BT);
	aes_set_irq_mask(FLD_CRYPT_IRQ);
#endif

#endif

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
#if(AES_MODE == AES_CRYPT_MODE)

#if(CRYPT_MODE == CRYPT_NORMAL_MODE)
	aes_encrypt(sKey, plaintext, ciphertext);

	for(unsigned char i = 0; i < 16; i++)
	{
		if(ciphertext[i] != cipher[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}

	aes_decrypt(sKey, ciphertext, deciphertext);

	for(unsigned char i = 0; i < 16; i++)
	{
		if(plaintext[i] != deciphertext[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}
	gpio_toggle(LED1);
	delay_ms(500);

#elif(AES_MODE == AES_IRQ_MODE)

	aes_set_key_data(sKey, plaintext);

	aes_set_mode(AES_ENCRYPT_MODE);   //start cipher

	while(0 == irq_flag);

	aes_get_result(ciphertext);
	irq_flag = 0;

	for(unsigned char i = 0; i < 16; i++)
	{
		if(ciphertext[i] != cipher[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}

	delay_ms(500);

	aes_set_key_data(sKey, ciphertext);

	aes_set_mode(AES_DECRYPT_MODE);   //start decipher

	while(0 == irq_flag);
	aes_get_result(deciphertext);
	irq_flag = 0;

	for(unsigned char i = 0; i < 16; i++)
	{
		if(plaintext[i] != deciphertext[i])
		{
			gpio_toggle(LED2);
			while(1);
		}
	}
	gpio_toggle(LED1);
	delay_ms(500);
#endif

#elif(AES_MODE == AES_RPA_MODE)
	unsigned char rpa[6] = {0x3b, 0x3f, 0xfb, 0xeb, 0x1e, 0x78};

	irk_value = aes_rpa_match(rpa_data, 7, rpa);

	if(0xff == irk_value)
	{
		gpio_toggle(LED2);
		while(1);
	}
	gpio_toggle(LED1);
	delay_ms(500);
#endif

}





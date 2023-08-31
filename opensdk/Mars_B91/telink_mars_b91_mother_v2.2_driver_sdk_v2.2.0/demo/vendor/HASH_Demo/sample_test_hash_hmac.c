/********************************************************************************************************
 * @file    sample_test_hash_hmac.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#if(HASH_TEST_MODE == SAMPLE_HASH_HMAC_TEST)

unsigned char sample_test_sm3();
unsigned char sample_test_hmac_sm3();
unsigned char sample_test_md5();
unsigned char sample_test_hmac_md5();
unsigned char sample_test_sha256();
unsigned char sample_test_hmac_sha256();
unsigned char sample_test_sha384();
unsigned char sample_test_hmac_sha384();
unsigned char sample_test_sha512();
unsigned char sample_test_hmac_sha512();
unsigned char sample_test_sha1();
unsigned char sample_test_hmac_sha1();
unsigned char sample_test_sha224();
unsigned char sample_test_hmac_sha224();
unsigned char sample_test_sha256();
unsigned char sample_test_hmac_sha256();
unsigned char sample_test_sha512_224();
unsigned char sample_test_hmac_sha512_224();
unsigned char sample_test_sha512_256();
unsigned char sample_test_hmac_sha512_256();

unsigned char HASH_HMAC_sample_test()
{
	printf("\r\n\r\n\r\n =================== HASH HMAC sample test =================== ");

//SM3 mode is not supported
#ifdef SUPPORT_HASH_SM3
	if(sample_test_sm3())
		return 1;

	if(sample_test_hmac_sm3())
		return 1;
#endif

#ifdef SUPPORT_HASH_MD5
	if(sample_test_md5())
		return 1;

	if(sample_test_hmac_md5())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA256
	if(sample_test_sha256())
		return 1;

	if(sample_test_hmac_sha256())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA384
	if(sample_test_sha384())
		return 1;

	if(sample_test_hmac_sha384())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512
	if(sample_test_sha512())
		return 1;

	if(sample_test_hmac_sha512())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA1
	if(sample_test_sha1())
		return 1;

	if(sample_test_hmac_sha1())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA224
	if(sample_test_sha224())
		return 1;

	if(sample_test_hmac_sha224())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_224
	if(sample_test_sha512_224())
		return 1;

	if(sample_test_hmac_sha512_224())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_256
	if(sample_test_sha512_256())
		return 1;

	if(sample_test_hmac_sha512_256())
		return 1;
#endif

	return 0;
}

void user_init()
{
	gpio_function_en(LED1);
	gpio_function_en(LED2);
	gpio_function_en(LED3);
	gpio_function_en(LED4);
	gpio_function_en(LED5);
	gpio_function_en(LED6);

	gpio_output_en(LED1);
	gpio_output_en(LED2);
	gpio_output_en(LED3);
	gpio_output_en(LED4);
	gpio_output_en(LED5);
	gpio_output_en(LED6);

	HASH_HMAC_sample_test();

}

void main_loop (void)
{

	delay_ms(500);
//	printf("\r\n hello world!!! \r\n");
	gpio_toggle(LED1);
}

#endif



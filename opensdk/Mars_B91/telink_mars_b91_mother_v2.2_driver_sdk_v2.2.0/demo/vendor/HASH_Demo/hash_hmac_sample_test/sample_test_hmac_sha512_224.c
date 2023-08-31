/********************************************************************************************************
 * @file    sample_test_hmac_sha512_224.c
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
#include "../app_config.h"
#if(HASH_TEST_MODE == SAMPLE_HASH_HMAC_TEST)


void hash_call_manage(void);

#ifdef SUPPORT_HASH_SHA512_224

unsigned char sample_test_hmac_sha512_224()
{
	unsigned char msg[80] = {
		0xCB,0xAC,0x28,0x1C,0xAC,0xD3,0x03,0x5F,0x81,0x5A,0x0E,0x82,0xF0,0xA5,0xEA,0x7A,
		0xC5,0x3C,0x4F,0xF7,0x0B,0xB7,0x9F,0xA6,0x1E,0x80,0xEA,0xF6,0xE6,0x03,0x0F,0x7F,
		0xF8,0x3E,0xF7,0x36,0x0F,0xC4,0x10,0xB0,0x1E,0xDC,0x20,0xD1,0xA1,0xB2,0xBD,0xAF,
		0xBE,0xC0,0x21,0xB4,0xF3,0x68,0xE3,0x06,0x91,0x34,0xE0,0x12,0xC2,0xB4,0x30,0x70,
		0x83,0xD3,0xA9,0xBD,0xD2,0x06,0xE2,0x4E,0x5F,0x0D,0x86,0xE1,0x3D,0x66,0x36,0x65,
	};
	unsigned char key[16] = {
		0x92,0x8C,0x8A,0xBE,0xCB,0xCB,0x38,0x5B,0x8E,0xEA,0xA0,0x05,0x9A,0xC8,0x05,0x84,
	};
	unsigned char std_mac[28] = {
		0xA3,0xB6,0xCC,0xDE,0x76,0xC3,0x21,0x02,0xE1,0x7D,0x4C,0x8D,0x39,0x81,0xDF,0x1E,
		0x17,0xA5,0x8B,0x23,0xE1,0xB8,0xB4,0x31,0xFE,0x86,0x2C,0x7E
	};
	unsigned char mac[28];
	unsigned char msg_byte_len = 80;
	unsigned char key_byte_len = 16;
	unsigned char mac_byte_len = 28;  //digest_byte_len
//	unsigned char block_byte_len = 128;

	unsigned int ret;

	HMAC_SHA512_224_CTX hmac_sha512_224_ctx[1];
	HMAC_CTX *hmac_ctx = hmac_sha512_224_ctx;

//DMA mode is not supported
#ifdef HASH_DMA_FUNCTION
	unsigned int *msg_buf = (unsigned int *)(DMA_RAM_BASE);
	unsigned int *iterator_buf = msg_buf+512;
	unsigned int msg_words = (msg_byte_len - (msg_byte_len%block_byte_len))/4;
	unsigned int remainder_bytes = msg_byte_len - msg_words*4;
	HMAC_SHA512_224_DMA_CTX hmac_sha512_224_dma_ctx[1];
	HMAC_DMA_CTX *hmac_dma_ctx = hmac_sha512_224_dma_ctx;

	uint32_copy(msg_buf, (unsigned int *)msg, (msg_byte_len+3)/4);
#endif

	printf("\r\n\r\n -------------- HMAC-SHA512_224 sample test -------------- ");

	//the following 4 calculations are equivalent

	/**************** test 1 ****************/
	ret = hmac_sha512_224_init(hmac_sha512_224_ctx, key, key_byte_len);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_update(hmac_sha512_224_ctx, msg, 4);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_update(hmac_sha512_224_ctx, msg+4, 44);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_update(hmac_sha512_224_ctx, msg+48, 32);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_final(hmac_sha512_224_ctx, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 sample test 1 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 sample test 1 success");fflush(stdout);
	}


	/**************** test 2 ****************/
	memset(mac, 0, mac_byte_len);
	ret = hmac_sha512_224(key, key_byte_len, msg, msg_byte_len, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 sample test 2 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 sample test 2 success");fflush(stdout);
	}

	/**************** test 3 ****************/
	memset(mac, 0, mac_byte_len);
	ret = hmac_init(hmac_ctx, HASH_SHA512_224, key, key_byte_len);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_update(hmac_sha512_224_ctx, msg, 4);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_update(hmac_sha512_224_ctx, msg+4, 44);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_update(hmac_sha512_224_ctx, msg+48, 32);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_final(hmac_sha512_224_ctx, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 sample test 3 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 sample test 3 success");fflush(stdout);
	}


	/**************** test 4 ****************/
	memset(mac, 0, mac_byte_len);
	ret = hmac(HASH_SHA512_224, key, key_byte_len, msg, msg_byte_len, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 sample test 4 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 sample test 4 success");fflush(stdout);
	}

//DMA mode is not supported
#ifdef HASH_DMA_FUNCTION
	//the following 4 DMA calculations are equivalent

	/**************** dma test 1 ****************/
	ret = hmac_sha512_224_dma_init(hmac_sha512_224_dma_ctx, key, key_byte_len, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_dma_update_blocks(hmac_sha512_224_dma_ctx, msg_buf, msg_words, iterator_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_sha512_224_dma_final(hmac_sha512_224_dma_ctx, msg_buf+msg_words, remainder_bytes, iterator_buf, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 1 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 1 success");fflush(stdout);
	}

	/**************** dma test 2 ****************/
	memset(mac, 0, mac_byte_len);
	ret = hmac_sha512_224_dma(key, key_byte_len, msg_buf, msg_byte_len, iterator_buf, mac, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 2 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 2 success");fflush(stdout);
	}

	/**************** dma test 3 ****************/
	ret = hmac_dma_init(hmac_dma_ctx, HASH_SHA512_224, key, key_byte_len, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_dma_update_blocks(hmac_dma_ctx, msg_buf, msg_words, iterator_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hmac_dma_final(hmac_dma_ctx, msg_buf+msg_words, remainder_bytes, iterator_buf, mac);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 1 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 1 success");fflush(stdout);
	}

	/**************** dma test 4 ****************/
	memset(mac, 0, mac_byte_len);
	ret = hmac_dma(HASH_SHA512_224, key, key_byte_len, msg_buf, msg_byte_len, iterator_buf, mac, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_mac, mac, mac_byte_len))
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 4 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n HMAC-SHA512_224 dma sample test 4 success");fflush(stdout);
	}
#endif

	return 0;
}


#endif

#endif

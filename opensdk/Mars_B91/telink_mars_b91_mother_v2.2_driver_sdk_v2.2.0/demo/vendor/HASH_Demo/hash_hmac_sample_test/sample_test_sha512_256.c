/********************************************************************************************************
 * @file    sample_test_sha512_256.c
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

#ifdef SUPPORT_HASH_SHA512_256

unsigned char sample_test_sha512_256()
{
	unsigned char msg[80] = {
		0xCB,0xAC,0x28,0x1C,0xAC,0xD3,0x03,0x5F,0x81,0x5A,0x0E,0x82,0xF0,0xA5,0xEA,0x7A,
		0xC5,0x3C,0x4F,0xF7,0x0B,0xB7,0x9F,0xA6,0x1E,0x80,0xEA,0xF6,0xE6,0x03,0x0F,0x7F,
		0xF8,0x3E,0xF7,0x36,0x0F,0xC4,0x10,0xB0,0x1E,0xDC,0x20,0xD1,0xA1,0xB2,0xBD,0xAF,
		0xBE,0xC0,0x21,0xB4,0xF3,0x68,0xE3,0x06,0x91,0x34,0xE0,0x12,0xC2,0xB4,0x30,0x70,
		0x83,0xD3,0xA9,0xBD,0xD2,0x06,0xE2,0x4E,0x5F,0x0D,0x86,0xE1,0x3D,0x66,0x36,0x65,
	};
	unsigned char std_digest[32] = {
		0xF8,0xC2,0x81,0x72,0xFB,0x4A,0x62,0xD0,0xA1,0x87,0xEB,0x11,0xB4,0xA3,0x84,0x38,
		0xE8,0xAF,0x31,0x24,0xB9,0xFE,0xBA,0xAA,0x5D,0x63,0x6D,0xEE,0xB8,0x01,0x9B,0x34
	};
	unsigned char digest[32];
	unsigned char msg_byte_len = 80;
	unsigned char digest_byte_len = 32;
//	unsigned char block_byte_len = 128;

	unsigned int ret;

	SHA512_256_CTX sha512_256_ctx[1];
	HASH_CTX *hash_ctx = sha512_256_ctx;

//DMA mode is not supported
#ifdef HASH_DMA_FUNCTION
	unsigned int *msg_buf = (unsigned int *)(DMA_RAM_BASE);
	unsigned int *digest_buf = msg_buf+512;
	unsigned int msg_words = (msg_byte_len - (msg_byte_len%block_byte_len))/4;
	unsigned int remainder_bytes = msg_byte_len - msg_words*4;
	SHA512_256_DMA_CTX sha512_256_dma_ctx[1];
	HASH_DMA_CTX *hash_dma_ctx = sha512_256_dma_ctx;

	uint32_copy(msg_buf, (unsigned int *)msg, (msg_byte_len+3)/4);
#endif

	printf("\r\n\r\n -------------- SHA512_256 sample test -------------- ");

	//the following 4 calculations are equivalent

	/**************** test 1 ****************/
	ret = sha512_256_init(sha512_256_ctx);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_update(sha512_256_ctx, msg, 4);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_update(sha512_256_ctx, msg+4, 44);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_update(sha512_256_ctx, msg+48, 32);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_final(sha512_256_ctx, digest);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_digest, digest, digest_byte_len))
	{
		printf("\r\n SHA512_256 sample test 1 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 sample test 1 success");fflush(stdout);
	}

	/**************** test 2 ****************/
	memset(digest, 0, digest_byte_len);
	ret = sha512_256(msg, msg_byte_len, digest);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_digest, digest, digest_byte_len))
	{
		printf("\r\n SHA512_256 sample test 2 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 sample test 2 success");fflush(stdout);
	}

	/**************** test 3 ****************/
	ret = hash_init(hash_ctx, HASH_SHA512_256);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_update(hash_ctx, msg, 4);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_update(hash_ctx, msg+4, 44);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_update(hash_ctx, msg+48, 32);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_final(hash_ctx, digest);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_digest, digest, digest_byte_len))
	{
		printf("\r\n SHA512_256 sample test 3 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 sample test 3 success");fflush(stdout);
	}

	/**************** test 4 ****************/
	memset(digest, 0, digest_byte_len);
	ret = hash(HASH_SHA512_256, msg, msg_byte_len, digest);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(memcmp(std_digest, digest, digest_byte_len))
	{
		printf("\r\n SHA512_256 sample test 4 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 sample test 4 success");fflush(stdout);
	}

//DMA mode is not supported
#ifdef HASH_DMA_FUNCTION
	//the following 4 DMA calculations are equivalent

	/**************** dma test 1 ****************/
	ret = sha512_256_dma_init(sha512_256_dma_ctx, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_dma_update_blocks(sha512_256_dma_ctx, msg_buf, msg_words, digest_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = sha512_256_dma_final(sha512_256_dma_ctx, msg_buf+msg_words, remainder_bytes, digest_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(uint32_BigNumCmp((unsigned int *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
	{
		printf("\r\n SHA512_256 dma sample test 1 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 dma sample test 1 success");fflush(stdout);
	}

	/**************** dma test 2 ****************/
	uint32_clear(digest_buf, digest_byte_len/4);
	ret = sha512_256_dma(msg_buf, msg_byte_len, digest_buf, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(uint32_BigNumCmp((unsigned int *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
	{
		printf("\r\n SHA512_256 dma sample test 2 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 dma sample test 2 success");fflush(stdout);
	}

	/**************** dma test 3 ****************/
	uint32_clear(digest_buf, digest_byte_len/4);
	ret = hash_dma_init(hash_dma_ctx, HASH_SHA512_256, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_dma_update_blocks(hash_dma_ctx, msg_buf, msg_words, digest_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	ret = hash_dma_final(hash_dma_ctx, msg_buf+msg_words, remainder_bytes, digest_buf);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(uint32_BigNumCmp((unsigned int *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
	{
		printf("\r\n SHA512_256 dma sample test 3 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 dma sample test 3 success");fflush(stdout);
	}

	/**************** dma test 4 ****************/
	uint32_clear(digest_buf, digest_byte_len/4);
	ret = hash_dma(HASH_SHA512_256, msg_buf, msg_byte_len, digest_buf, hash_call_manage);
	if(HASH_SUCCESS != ret)
	{
		return 1;
	}

	if(uint32_BigNumCmp((unsigned int *)std_digest, digest_byte_len/4, digest_buf, digest_byte_len/4))
	{
		printf("\r\n SHA512_256 dma sample test 4 error");fflush(stdout);
		return 1;
	}
	else
	{
		printf("\r\n SHA512_256 dma sample test 4 success");fflush(stdout);
	}

#endif

	return 0;
}


#endif

#endif

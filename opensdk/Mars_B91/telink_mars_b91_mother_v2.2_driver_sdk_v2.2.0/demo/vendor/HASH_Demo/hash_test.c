/********************************************************************************************************
 * @file    hash_test.c
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
#if(HASH_TEST_MODE == HASH_TEST)


//get random value in [1,max_number-1]
unsigned int hash_get_rand_number(unsigned int max_number)
{
	static unsigned int i=34;

#if 0
	get_rand((unsigned char *)&i, 4);
#else
	//simple random number generator
	i *= i<<1;
	i -= i*3;
	i ^= 0x78345bc;
	i += 89 | i>>1;
#endif

	i = i%max_number;
	if(0 == i)
	{
		i=1;
	}

	return i;
}

//callback function
void hash_call_manage(void)
{
	;
}

unsigned char hash_test(HASH_ALG hash_alg, unsigned char *std_digest1,unsigned char *std_digest2,unsigned char *std_digest3)
{
	unsigned char message[500]={0x61,0x62,0x63};
	unsigned char digest[64];
	HASH_CTX hash_ctx[1];
	char *name[]={"SM3","MD5","SHA256","SHA384","SHA512","SHA1","SHA224","SHA512_224","SHA512_256"};
	unsigned char outLen[]={32,16,32,48,64,20,28,28,32};
	unsigned int i, j, total;
	unsigned int len[5][15]=     //sum of each line is 500
	{
		{4, 91, 173, 23, 56, 79, 2, 11, 61, 0},
		{84, 37, 68, 293,13,  1, 4, 0},
		{33, 20, 208, 3, 7, 67, 33, 88, 19, 22, 0},
		{59, 111, 29, 34,27, 89, 9, 6,  79, 22, 35, 0},
		{333, 2, 5, 39, 77, 16, 24, 4, 0},
	};
	unsigned char ret;
	
	
	printf("\r\n -------------- %s test -------------- ", name[hash_alg]);
	
	/*************************** test1 ***************************/
	ret = hash_init(hash_ctx, hash_alg);
	ret |= hash_update(hash_ctx, message, 3);
	ret |= hash_final(hash_ctx, digest);

#if 0
		print_buf_U8(message, 3, "message");	
	    print_buf_U8(digest, outLen[hash_alg], "digest");
#endif
	if(HASH_SUCCESS != ret || memcmp(digest, std_digest1, outLen[hash_alg]))
	{
		printf("\r\n %s test 1 failure, ret=%d\r\n", name[hash_alg], ret);
	    return 1;
	}
	else
	{
		printf("\r\n %s test 1 success ", name[hash_alg]);
	}

#if 1
	/*************************** test2 ***************************/
	for(i=0;i<500;i++)
		message[i] = i;

	//test2-1
#if 1
	ret = hash(hash_alg, message, 500, digest);
#else
	ret = hash_init(hash_ctx, hash_alg);
	ret |= hash_update(hash_ctx, message, 500);
	ret |= hash_finish(hash_ctx, digest);
#endif
	if(HASH_SUCCESS != ret || memcmp(digest, std_digest2, outLen[hash_alg]))
	{
		printf("\r\n %s test 2-%d failure, ret=%d \r\n", name[hash_alg], 1, ret);
		return 1;
	}
	else
	{
		printf("\r\n %s test 2-%d success ", name[hash_alg], 1);
	}

	//test2-2...2-6
	for(i=0;i<5;i++)
	{
		ret = hash_init(hash_ctx, hash_alg);
				
		j=0;
		total=0;
		while(len[i][j])
		{
			ret |= hash_update(hash_ctx, message+total, len[i][j]);
			total+=len[i][j];
			j++;
		}
		
		memset(digest, 0, 64);
		ret |= hash_final(hash_ctx, digest);
		if(HASH_SUCCESS != ret || memcmp(digest, std_digest2, outLen[hash_alg]))
		{
			printf("\r\n %s test 2-%d failure, ret=%d \r\n", name[hash_alg], i+2, ret);
			return 1;
		}
		else 
		{
			printf("\r\n %s test 2-%d success ", name[hash_alg], i+2);
		}
	}

	//test2-7...
	printf("\r\n");
	for(; i<100; i++)
	{
		ret = hash_init(hash_ctx, hash_alg);

		total = 0;
		while(total < 500)
		{
			j = hash_get_rand_number(500);
			if(j%10 < 3)
				j = j%10;
			else if(j%10 < 5)
				j = j%70;
			else if(j%10 < 8)
				j = j%128;

			if(j > 500-total)
				j = 500-total;

			ret |= hash_update(hash_ctx, message+total, j);
			total += j;
		}

		memset(digest, 0, 64);
		ret |= hash_final(hash_ctx, digest);
		if(HASH_SUCCESS != ret || memcmp(digest, std_digest2, outLen[hash_alg]))
		{
			printf("\r\n %s test 2-%d failure, ret=%d \r\n", name[hash_alg], i+2, ret);fflush(stdout);
			return 1;
		}
		else
		{
			printf("\r\n %s test 2-%d success ", name[hash_alg], i+2);
//			printf("2-%d ", i+2);
			fflush(stdout);
		}
	}


	/*************************** test3 ***************************/
	for(i=0;i<500;i++)
		message[i] = i;
	ret = 0;
	for(i=0;i<=500; i++)
	{
		j = hash_get_rand_number(2);
		if(j)
		{
		ret |= hash_init(hash_ctx, hash_alg);
		ret |= hash_update(hash_ctx, message, i);
		ret |= hash_final(hash_ctx, message);
		}
		else
		{
			hash(hash_alg, message, i, message);
		}
    }
	if(HASH_SUCCESS != ret || memcmp(message, std_digest3, outLen[hash_alg]))
	{
		printf("\r\n %s test 3 failure, ret=%d \r\n", name[hash_alg], ret);
		return 1;
	}
	else
	{
		printf("\r\n %s test 3 success ", name[hash_alg]);
	}
#endif

	printf("\r\n");
	return 0;
}


#if (defined(CONFIG_HASH_SUPPORT_MUL_THREAD) && defined(SUPPORT_HASH_SHA256) && defined(SUPPORT_HASH_SHA512))
unsigned char SHA256_SHA512_cross_test()
{
	unsigned char message[500];
	unsigned char digest1[32];
	unsigned char digest2[64];
	unsigned char std_sha256_digest[32] =  {
		0x6A,0x25,0x9D,0xA4,0xDA,0xCD,0xFB,0x0F,0x51,0x36,0x96,0x49,0xCB,0xF8,0x86,0x4D,
		0x8E,0x2D,0x67,0x54,0x62,0xC8,0x62,0x5A,0x70,0x33,0x4B,0xFC,0x2C,0x50,0xD1,0xAF,};
	unsigned char std_sha512_digest[64] = {
		0xF8,0xD1,0xC9,0x17,0xDA,0x2E,0x66,0xBE,0x92,0x02,0x2D,0x6A,0x49,0xEC,0xEE,0x0F,
		0x0D,0x89,0x7C,0x6D,0x97,0x92,0x7A,0xDC,0x65,0x4C,0x73,0xD7,0x35,0xDC,0xA4,0x20,
		0xDD,0x80,0x8B,0xE0,0x67,0x94,0xF5,0x14,0xD5,0xA8,0x8F,0xA2,0xE2,0x4C,0x0C,0x0F,
		0x08,0x17,0xC9,0x5A,0xD4,0x48,0x37,0x1B,0x4C,0xD6,0xA8,0xCB,0x6A,0xA6,0x56,0xB3,};
	HASH_CTX sha256_ctx[1];
	HASH_CTX sha512_ctx[1];
	unsigned int i;
	unsigned char ret;
	
	printf("\r\n -------------- SHA256 & SHA512 cross test -------------- ");
	
	for(i=0;i<500;i++)
		message[i] = i;		

    ret = hash_init(sha256_ctx, HASH_SHA256);
    ret |= hash_init(sha512_ctx, HASH_SHA512);
	
    ret |= hash_update(sha256_ctx, message, 25);
	
    ret |= hash_update(sha512_ctx, message, 220);

    ret |= hash_update(sha256_ctx, message+25, 251);

    ret |= hash_update(sha512_ctx, message+220, 41);

    ret |= hash_update(sha256_ctx, message+276, 115);
	
    ret |= hash_update(sha512_ctx, message+261, 137);
	
    ret |= hash_update(sha256_ctx, message+391, 109);
    ret |= hash_final(sha256_ctx, digest1);
	if(HASH_SUCCESS != ret || memcmp(digest1, std_sha256_digest, 32))
	{
		printf("\r\n SHA256 test failure \r\n");
		return 1;
	}
	else 
	{
		printf("\r\n SHA256 test success ");
	}	

	ret |= hash_update(sha512_ctx, message+398, 38+64);
	ret |= hash_final(sha512_ctx, digest2);

	if(HASH_SUCCESS != ret || memcmp(digest2, std_sha512_digest, 64))
	{
		printf("\r\n SHA512 test failure \r\n");
		return 1;
	}
	else 
	{
		printf("\r\n SHA512 test success ");
	}		
	
	return 0;
}
#endif


#define HASH_SPEED_TEST_ROUND             (409600)
#define HASH_SPEED_TEST_BYTE_LEN          (1024)
#define HASH_DMA_SPEED_TEST_ROUND         (1000)
#define HASH_DMA_SPEED_TEST_WORD_LEN      (262144)    //1024*16*SM3_BLOCK_WORD_LEN

void hash_speed_test()
{
	unsigned char message[HASH_SPEED_TEST_BYTE_LEN];
	unsigned char digest[64];
	unsigned int i;
	HASH_CTX ctx[1];
	HASH_ALG hash_alg = HASH_SHA512_256;//HASH_SM3;//HASH_SHA512;//HASH_SHA384;//HASH_SHA256;//HASH_MD5;//HASH_SHA1;//HASH_SHA224;//HASH_SHA512_224;//

	printf("\r\n hash begin");fflush(stdout);
	hash_init(ctx, hash_alg);
	for(i=0;i<HASH_SPEED_TEST_ROUND;i++)
		hash_update(ctx, message, HASH_SPEED_TEST_BYTE_LEN);
	hash_final(ctx, digest);
	printf("\r\n hash finished");fflush(stdout);
}

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
void hash_dma_speed_test()
{
	HASH_DMA_CTX ctx[1];
	unsigned int i;
	unsigned int *msg=(unsigned int *)(DMA_RAM_BASE);
	unsigned int *digest = msg+512;
	HASH_ALG hash_alg = HASH_MD5;//HASH_SHA384;//HASH_SHA256;//HASH_SM3;//HASH_SHA512;//HASH_SHA512_224;//HASH_SHA512_256;//HASH_SHA224;//HASH_SHA1;//

	//dma speed test
	printf("\r\n hash dma begin");fflush(stdout);
	hash_dma_init(ctx, hash_alg, hash_call_manage);
	for(i=0;i<HASH_DMA_SPEED_TEST_ROUND;i++)
		hash_dma_update_blocks(ctx, msg, HASH_DMA_SPEED_TEST_WORD_LEN, digest);
	hash_dma_final(ctx, msg, 0, digest);
	printf("\r\n hash dma finished");fflush(stdout);
}


unsigned char hash_dma_test(HASH_ALG hash_alg, unsigned char *std_digest1, unsigned char *std_digest2, unsigned char *std_digest3)
{
	unsigned char message[500]={0x61,0x62,0x63,0x00};
	unsigned char tmp_buf[500];

	HASH_DMA_CTX ctx[1];
	unsigned int i;
	unsigned int block_word_len, block_byte_len, output_word_len, digest_byte_len;
	char *name[]={"SM3","MD5","SHA_256","SHA_384","SHA_512","SHA_1","SHA_224","SHA_512_224","SHA_512_256"};
	unsigned int *msg=(unsigned int *)(DMA_RAM_BASE);
	unsigned int *digest = msg+512;
	unsigned char ret;

	printf("\r\n ------------ %s DMA test ------------ ", name[hash_alg]);

	block_word_len = hash_get_block_word_len(hash_alg);
	block_byte_len = block_word_len<<2;
	digest_byte_len = hash_get_digest_word_len(hash_alg)<<2;
	output_word_len = ((digest_byte_len+15)/16)*4;  //must be a multiple of 4(namely 128bits)


	//test1  message: 0x616263, 3 bytes.
	uint32_clear(msg, 512/4);

	uint32_copy(msg, (unsigned int *)message, 1);

	//the two styles are equivalent
#if 1
	ret = hash_dma(hash_alg, msg, 3, digest, hash_call_manage);
#else
	ret = hash_dma_init(ctx, hash_alg);
	ret |= hash_dma_last_process(ctx, msg, 3, digest);
#endif

	if(HASH_SUCCESS != ret || memcmp(std_digest1, digest, digest_byte_len))
	{
		printf("\r\n %s DMA test 1 failure, ret=%d \r\n", name[hash_alg], ret);
		print_buf_U8(message, 3, "message");
		print_buf_U8((unsigned char *)digest, output_word_len<<2, "digest");
		print_buf_U8(std_digest1, digest_byte_len, "std digest");
		return 1;
	}
	else
	{
		printf("\r\n %s DMA test 1 success ", name[hash_alg]); fflush(stdout);
	}


	//test2  message: 0x000102..., 500 bytes.
	for(i=0;i<500;i++)
	{
		message[i] = i;
	}

	uint32_clear(msg, 512/4);
	uint32_copy(msg, (unsigned int *)message, 500);

	//the two styles are equivalent
#if 0
	ret = hash_dma(hash_alg, msg, 500, digest, hash_call_manage);
#else
	ret = hash_dma_init(ctx, hash_alg, hash_call_manage);
	if(16 == block_word_len)         //for sm3,sha256,etc.
	{
		#if 0
		ret |= hash_dma_update_blocks(ctx, msg, 7*block_word_len, digest);
		#else
		ret |= hash_dma_update_blocks(ctx, msg, 2*block_word_len, digest);
		ret |= hash_dma_update_blocks(ctx, msg+2*block_word_len, block_word_len, digest);
		ret |= hash_dma_update_blocks(ctx, msg+3*block_word_len, 3*block_word_len, digest);
		ret |= hash_dma_update_blocks(ctx, msg+6*block_word_len, block_word_len, digest);
		#endif
		ret |= hash_dma_final(ctx, msg+7*block_word_len, 500-7*block_byte_len, digest);
	}
	else if(32 == block_word_len)  //for sha384,sha512,etc.
	{
		#if 0
		ret |= hash_dma_update_blocks(ctx, msg, 3*block_word_len, digest);
		#else
		ret |= hash_dma_update_blocks(ctx, msg, 2*block_word_len, digest);
		ret |= hash_dma_update_blocks(ctx, msg+2*block_word_len, block_word_len, digest);
		#endif
		ret |= hash_dma_final(ctx, msg+3*block_word_len, 500-3*block_byte_len, digest);
	}
#endif

	if(HASH_SUCCESS != ret || memcmp(std_digest2, digest, digest_byte_len))
	{
		printf("\r\n %s DMA test 2 failure, ret=%d \r\n", name[hash_alg], ret);fflush(stdout);
		print_buf_U8(message, 500, "message");
		print_buf_U8((unsigned char *)digest, output_word_len<<2, "digest");
		print_buf_U8(std_digest1, digest_byte_len, "std digest");
		return 1;
	}
	else
	{
		printf("\r\n %s DMA test 2 success ", name[hash_alg]);fflush(stdout);
	}


	//test3  message: 0x000102..., 500 bytes.
	for(i=0;i<500;i++)
	{
		message[i] = i;
	}

	ret = 0;
	for(i=0;i<=500;i++)
	{
		memset(tmp_buf, 0, 500);
		memcpy(tmp_buf, message, i);

		uint32_clear(msg, 512/4);
		uint32_copy(msg, (unsigned int *)tmp_buf, (i+3)/4);

		ret |= hash_dma(hash_alg, msg, i, digest, hash_call_manage);

		uint32_copy((unsigned int *)message, digest, digest_byte_len/4);
	}

	if(HASH_SUCCESS != ret || memcmp(std_digest3, digest, digest_byte_len))
	{
		printf("\r\n %s DMA test 3 failure, ret=%d \r\n", name[hash_alg], ret);fflush(stdout);
		print_buf_U8(message, 500, "message");
		print_buf_U8((unsigned char *)digest, digest_byte_len, "digest");
		return 1;
	}
	else
	{
		printf("\r\n %s DMA test 3 success \r\n", name[hash_alg]);fflush(stdout);
	}

	return 0;
}
#endif


#ifdef SUPPORT_HASH_MD5
unsigned char MD5_test()
{
	unsigned char std_digest1[32] =  {0x90,0x01,0x50,0x98,0x3C,0xD2,0x4F,0xB0,0xD6,0x96,0x3F,0x7D,0x28,0xE1,0x7F,0x72,};
	unsigned char std_digest2[32] =  {0xDE,0xC0,0x59,0x23,0x45,0xAC,0x3D,0xFA,0x17,0x2C,0x9E,0xC4,0x76,0xA2,0x4C,0x75,};
	unsigned char std_digest3[32] =  {0x1B,0x6B,0xF6,0x9F,0x9A,0x78,0x01,0x84,0xD8,0xF7,0x47,0x8C,0x27,0x96,0x22,0x77,};
	unsigned char ret;

	ret = hash_test(HASH_MD5, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_MD5, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

//SM3 mode is not supported.
#ifdef SUPPORT_HASH_SM3
unsigned char SM3_test()
{
	unsigned char std_digest1[32] =  {0x66,0xC7,0xF0,0xF4,0x62,0xEE,0xED,0xD9,0xD1,0xF2,0xD4,0x6B,0xDC,0x10,0xE4,0xE2,
								0x41,0x67,0xC4,0x87,0x5C,0xF2,0xF7,0xA2,0x29,0x7D,0xA0,0x2B,0x8F,0x4B,0xA8,0xE0};
	unsigned char std_digest2[32] =  {0xda,0xd1,0x46,0x5a,0x3f,0x51,0x12,0x8d,0x86,0xf8,0xda,0xaa,0x4f,0x4e,0x7e,0x1b,
								0x3d,0x2e,0x70,0x4c,0x29,0x45,0x12,0x47,0xaf,0xd4,0x9d,0x7d,0x96,0xa5,0xac,0x31,};
	unsigned char std_digest3[32] =  {0xe8,0xb9,0x9c,0xdf,0x8c,0x46,0xa6,0x72,0xce,0xc9,0x9d,0x97,0x6e,0x31,0xa1,0x5c,
								0x83,0x63,0x1f,0x57,0x47,0x05,0x29,0x5f,0xd3,0x0f,0x92,0x3a,0xe7,0xeb,0xbd,0x5e,};
	unsigned char ret;

	ret = hash_test(HASH_SM3, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SM3, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA1
unsigned char SHA1_test()
{
	unsigned char std_digest1[20] =  {0xA9,0x99,0x3E,0x36,0x47,0x06,0x81,0x6A,0xBA,0x3E,0x25,0x71,0x78,0x50,0xC2,0x6C,
								0x9C,0xD0,0xD8,0x9D,};
	unsigned char std_digest2[20] =  {0x50,0x75,0x0F,0x6E,0xC4,0xBD,0xAA,0x13,0x43,0x69,0x29,0x9D,0xE5,0x3D,0x8D,0x8B,
								0x87,0xD1,0xBA,0x63,};
	unsigned char std_digest3[20] =  {0x9F,0x14,0x31,0x91,0x13,0x17,0xD7,0x4D,0x2D,0xF1,0xBD,0xD2,0x52,0x82,0xA4,0xD8,
								0x0E,0x66,0x42,0x7F,};
	unsigned char ret;

	ret = hash_test(HASH_SHA1, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA1, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA256
unsigned char SHA256_test()
{
	unsigned char std_digest1[32] =  {0xBA,0x78,0x16,0xBF,0x8F,0x01,0xCF,0xEA,0x41,0x41,0x40,0xDE,0x5D,0xAE,0x22,0x23,
								0xB0,0x03,0x61,0xA3,0x96,0x17,0x7A,0x9C,0xB4,0x10,0xFF,0x61,0xF2,0x00,0x15,0xAD};
	unsigned char std_digest2[32] =  {0x6A,0x25,0x9D,0xA4,0xDA,0xCD,0xFB,0x0F,0x51,0x36,0x96,0x49,0xCB,0xF8,0x86,0x4D,
								0x8E,0x2D,0x67,0x54,0x62,0xC8,0x62,0x5A,0x70,0x33,0x4B,0xFC,0x2C,0x50,0xD1,0xAF};
	unsigned char std_digest3[32] =  {0xC3,0xFB,0x71,0x00,0xFE,0x3F,0x3F,0x9F,0xAF,0x19,0xD2,0x8E,0xE4,0x4B,0x5A,0x87,
								0xCF,0x6F,0xB6,0xD1,0x55,0x6C,0x5A,0x04,0x45,0xE5,0x84,0xF6,0xAE,0x9D,0xB8,0x6A};
	unsigned char ret;

	ret = hash_test(HASH_SHA256, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA256, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA224
unsigned char SHA224_test()
{
	unsigned char std_digest1[28] =  {0x23,0x09,0x7D,0x22,0x34,0x05,0xD8,0x22,0x86,0x42,0xA4,0x77,0xBD,0xA2,0x55,0xB3,
								0x2A,0xAD,0xBC,0xE4,0xBD,0xA0,0xB3,0xF7,0xE3,0x6C,0x9D,0xA7};
	unsigned char std_digest2[28] =  {0xDE,0xFC,0x2F,0x15,0x71,0x32,0x50,0xE4,0xEF,0x6B,0xCC,0xEB,0x4F,0xAC,0x74,0x42,
								0xDF,0x56,0x51,0xFC,0x17,0xA0,0xBC,0x56,0xA9,0xEF,0x67,0xF2};
	unsigned char std_digest3[28] =  {0xFE,0x56,0x7D,0xC1,0x15,0xA9,0xA1,0x62,0xA4,0xB7,0x92,0x1A,0x76,0x6E,0x4F,0x35,
								0x67,0x65,0x74,0xE7,0xCD,0x00,0x4E,0x2E,0xEA,0xE2,0x73,0x99};
	unsigned char ret;

	ret = hash_test(HASH_SHA224, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA224, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA384
unsigned char SHA384_test()
{
	unsigned char std_digest1[48] =  {0xCB,0x00,0x75,0x3F,0x45,0xA3,0x5E,0x8B,0xB5,0xA0,0x3D,0x69,0x9A,0xC6,0x50,0x07,
								0x27,0x2C,0x32,0xAB,0x0E,0xDE,0xD1,0x63,0x1A,0x8B,0x60,0x5A,0x43,0xFF,0x5B,0xED,
								0x80,0x86,0x07,0x2B,0xA1,0xE7,0xCC,0x23,0x58,0xBA,0xEC,0xA1,0x34,0xC8,0x25,0xA7};
	unsigned char std_digest2[48] =  {0x32,0x1E,0x84,0x33,0xE8,0xE1,0xE4,0x61,0xBD,0xC5,0xB1,0x49,0x58,0x2F,0x4A,0x80,
								0xB3,0xA8,0x8D,0x75,0x10,0x42,0x08,0xA2,0xC4,0xF8,0xA5,0x35,0xB4,0x5D,0xDB,0x27,
								0x34,0xEB,0xAC,0x7C,0x89,0x96,0xC8,0x60,0x4F,0xF0,0x3A,0x5A,0xEE,0x3D,0xA5,0xCB};
	unsigned char std_digest3[48] =  {0xCC,0x2B,0x35,0xE0,0x00,0xE8,0x54,0x8B,0x17,0xAD,0x89,0xC1,0xA9,0x15,0xBB,0x36,
								0x5A,0xE8,0xBF,0xFF,0x89,0xBA,0x12,0xD1,0x9B,0x5E,0x3F,0xE8,0x56,0xC5,0x1A,0xAE,
								0xC0,0x45,0x9B,0x80,0x25,0xD8,0x99,0x96,0xE1,0x2F,0x44,0x8F,0xD0,0x2E,0x46,0xF9};
	unsigned char ret;

	ret = hash_test(HASH_SHA384, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA384, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA512
unsigned char SHA512_test()
{
	unsigned char std_digest1[64] =  {0xDD,0xAF,0x35,0xA1,0x93,0x61,0x7A,0xBA,0xCC,0x41,0x73,0x49,0xAE,0x20,0x41,0x31,
								0x12,0xE6,0xFA,0x4E,0x89,0xA9,0x7E,0xA2,0x0A,0x9E,0xEE,0xE6,0x4B,0x55,0xD3,0x9A,
								0x21,0x92,0x99,0x2A,0x27,0x4F,0xC1,0xA8,0x36,0xBA,0x3C,0x23,0xA3,0xFE,0xEB,0xBD,
								0x45,0x4D,0x44,0x23,0x64,0x3C,0xE8,0x0E,0x2A,0x9A,0xC9,0x4F,0xA5,0x4C,0xA4,0x9F};
	unsigned char std_digest2[64] =  {0xF8,0xD1,0xC9,0x17,0xDA,0x2E,0x66,0xBE,0x92,0x02,0x2D,0x6A,0x49,0xEC,0xEE,0x0F,
								0x0D,0x89,0x7C,0x6D,0x97,0x92,0x7A,0xDC,0x65,0x4C,0x73,0xD7,0x35,0xDC,0xA4,0x20,
								0xDD,0x80,0x8B,0xE0,0x67,0x94,0xF5,0x14,0xD5,0xA8,0x8F,0xA2,0xE2,0x4C,0x0C,0x0F,
								0x08,0x17,0xC9,0x5A,0xD4,0x48,0x37,0x1B,0x4C,0xD6,0xA8,0xCB,0x6A,0xA6,0x56,0xB3};
	unsigned char std_digest3[64] =  {0xB5,0xA7,0xB2,0x22,0x2E,0x91,0x6B,0x74,0xA7,0xDD,0x59,0x3A,0x07,0x8A,0x33,0xAC,
								0xF5,0x3D,0xD9,0x55,0xF7,0x66,0x10,0x89,0x80,0x49,0xC3,0xB3,0x1D,0x16,0x50,0xBB,
								0x5D,0x63,0x53,0xC0,0x35,0x70,0xFB,0x30,0xBC,0x1F,0xBA,0x0C,0x02,0xDF,0x4E,0xB9,
								0x37,0x87,0xED,0x79,0x0B,0x1E,0x74,0x8A,0x3A,0xA6,0xA7,0x15,0xEC,0x41,0x02,0xFA};
	unsigned char ret;

	ret = hash_test(HASH_SHA512, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA512, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA512_224
unsigned char SHA512_224_test()
{
	unsigned char std_digest1[28] =  {0x46,0x34,0x27,0x0F,0x70,0x7B,0x6A,0x54,0xDA,0xAE,0x75,0x30,0x46,0x08,0x42,0xE2,
								0x0E,0x37,0xED,0x26,0x5C,0xEE,0xE9,0xA4,0x3E,0x89,0x24,0xAA};
	unsigned char std_digest2[28] =  {0xEB,0x41,0x69,0x3D,0x78,0x4B,0x87,0x25,0xC1,0xFB,0x06,0xA6,0x92,0x2E,0x86,0x9B,
								0x1C,0xF0,0x34,0x64,0x04,0x8C,0xCF,0x72,0x0F,0x62,0x34,0xFD};
	unsigned char std_digest3[28] =  {0x8A,0x48,0x1F,0xBD,0x48,0x94,0xD7,0x2C,0x63,0xA7,0x3A,0xF9,0x5C,0xB4,0x4F,0x34,
								0x6E,0xAF,0x0E,0xF0,0x5E,0x25,0x17,0xBB,0x47,0x3A,0xBE,0xF4};
	unsigned char ret;

	ret = hash_test(HASH_SHA512_224, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA512_224, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif

#ifdef SUPPORT_HASH_SHA512_256
unsigned char SHA512_256_test()
{
	unsigned char std_digest1[32] =  {0x53,0x04,0x8E,0x26,0x81,0x94,0x1E,0xF9,0x9B,0x2E,0x29,0xB7,0x6B,0x4C,0x7D,0xAB,
								0xE4,0xC2,0xD0,0xC6,0x34,0xFC,0x6D,0x46,0xE0,0xE2,0xF1,0x31,0x07,0xE7,0xAF,0x23};
	unsigned char std_digest2[32] =  {0x81,0xDC,0x4F,0x56,0x1A,0x2A,0x4C,0x29,0x6B,0xD3,0xAA,0x3F,0x67,0xC1,0x73,0x5F,
								0x86,0xDA,0xBE,0xB6,0x2D,0xBB,0xCE,0xC3,0x90,0xBD,0x58,0x16,0xE5,0x78,0x10,0xEA};
	unsigned char std_digest3[32] =  {0xFE,0xB9,0x98,0x10,0xCC,0xE3,0xB3,0x8F,0x3E,0x12,0xC1,0xF6,0xA6,0x5B,0x5A,0x1C,
								0xE4,0x9A,0x5E,0xBD,0xD7,0x35,0x70,0x9F,0x7F,0x4E,0xD4,0x16,0x34,0x25,0x4C,0x74};
	unsigned char ret;

	ret = hash_test(HASH_SHA512_256, std_digest1, std_digest2, std_digest3);
	if(ret)
		return 1;

//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	ret = hash_dma_test(HASH_SHA512_256, std_digest1, std_digest2, std_digest3);
#endif

	return ret;
}
#endif


unsigned char HASH_all_test()
{
	printf("\r\n\r\n =================== HASH test =================== ");
#if 0
	hash_speed_test();
#endif

#if 0
//DMA mode is not supported.
#ifdef HASH_DMA_FUNCTION
	hash_dma_speed_test();
#endif
#endif

#if 1
//SM3 mode is not supported.
#ifdef SUPPORT_HASH_SM3
	if(SM3_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_MD5
	if(MD5_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA256
	if(SHA256_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA384
	if(SHA384_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512
	if(SHA512_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA1
	if(SHA1_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA224
	if(SHA224_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_224
	if(SHA512_224_test())
		return 1;
#endif

#ifdef SUPPORT_HASH_SHA512_256
	if(SHA512_256_test())
		return 1;
#endif


#if (defined(CONFIG_HASH_SUPPORT_MUL_THREAD) && defined(SUPPORT_HASH_SHA256) && defined(SUPPORT_HASH_SHA512))
	if(SHA256_SHA512_cross_test())
		return 1;
#endif

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

	HASH_all_test();

}

void main_loop (void)
{
	delay_ms(500);
	gpio_toggle(LED1);
}

#endif

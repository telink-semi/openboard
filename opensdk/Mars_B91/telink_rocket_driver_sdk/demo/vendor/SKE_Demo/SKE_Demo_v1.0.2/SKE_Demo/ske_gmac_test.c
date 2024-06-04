/********************************************************************************************************
 * @file    ske_gmac_test.c
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
#if (SKE_TEST_MODE == SKE_GMAC_TEST)


#include <stdio.h>


//#define GMAC_SPEED_TEST_BY_TIMER

#ifdef GMAC_SPEED_TEST_BY_TIMER
extern unsigned int startP();
extern unsigned int endP(unsigned char mode, unsigned int once_bytes, unsigned int round);
#endif

static unsigned int speed_get_round_by_alg(SKE_ALG alg)
{
	unsigned int round;

	switch(alg)
	{
#ifdef SUPPORT_SKE_TDES_128
	case SKE_ALG_TDES_128:
#endif

#ifdef SUPPORT_SKE_TDES_192
	case SKE_ALG_TDES_192:
#endif

#ifdef SUPPORT_SKE_TDES_EEE_128
	case SKE_ALG_TDES_EEE_128:
#endif

#ifdef SUPPORT_SKE_TDES_EEE_192
	case SKE_ALG_TDES_EEE_192:
#endif

#if (defined(SUPPORT_SKE_TDES_128) || defined(SUPPORT_SKE_TDES_192) || defined(SUPPORT_SKE_TDES_EEE_128) || defined(SUPPORT_SKE_TDES_EEE_192))
		round = 20;  //since haredware does not support 3DES.
		break;
#endif

	default:         //hardware supports other ske alg
		round = 100;
	}

	return round;
}

void get_rand_(unsigned char *rand, unsigned int bytes)
{
#if 0
	memset_(rand, 0x43, bytes);
#else
	get_rand(rand, bytes);
#endif
}

//get random value in [1,max_number-1]
unsigned int ske_get_rand_number(unsigned int max_number)
{
	unsigned int i;

	get_rand_((unsigned char *)&i, 4);

	i = i%max_number;
	if(0 == i)
	{
		i=1;
	}

	return i;
}

//GMAC mode is only available for AES and SM4(block length is 128bits)

#ifdef SUPPORT_SKE_MODE_GMAC


unsigned int ske_gmac_test(SKE_ALG alg, unsigned char wordAlign, unsigned char *std_msg, unsigned int byteLen, unsigned char *key,
		unsigned short sp_key_idx, unsigned char *iv, unsigned char ivByteLen, unsigned char *aad, unsigned char aadByteLen,
		unsigned char *std_mac, unsigned int macByteLen)
{
	unsigned char key_buf[32+4];
	unsigned char iv_buf[64+4];
	unsigned char aad_buf[64+4];
	unsigned char std_msg_buf[256+4];
	unsigned char std_mac_buf[16+4];
	unsigned char mac_buf[16+4];
	unsigned char *mac_, *std_msg_, *std_mac_, *std_key_, *std_iv_, *std_aad_;
	unsigned int i,j;
	unsigned int block_byteLen, key_byteLen;
	unsigned int ret;

	char *name[]={"","","","","","AES_128","AES_192","AES_256","SM4"};

	SKE_GMAC_CTX ctx[1];

	key_byteLen = ske_lp_get_key_byte_len(alg);
	block_byteLen = ske_lp_get_block_byte_len(alg); //16;

	printf("\r\n %s GMAC test(iv=%u bytes,AAD=%u bytes,P=%u bytes, mac=%u bytes)",
			name[alg],ivByteLen,aadByteLen,byteLen,macByteLen);

	if(wordAlign)
	{
		mac_        = mac_buf;
		std_msg_    = std_msg_buf;
		std_mac_    = std_mac_buf;
		std_key_    = key_buf;
		std_iv_     = iv_buf;
		std_aad_    = aad_buf;
	}
	else
	{
		mac_        = mac_buf + 1;
		std_msg_    = std_msg_buf + 1;
		std_mac_    = std_mac_buf + 1;
		std_key_    = key_buf + 1;
		std_iv_     = iv_buf + 1;
		std_aad_    = aad_buf + 1;
	}
	memcpy_(std_msg_, std_msg, byteLen);
	memcpy_(std_mac_, std_mac, macByteLen);
	memcpy_(std_key_, key, key_byteLen);
	memcpy_(std_iv_, iv, ivByteLen);
	memcpy_(std_aad_, aad, aadByteLen);


	/**************** test 1: one-time style ******************/
#if 1
	//generate mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_GENERATE_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 0 error, ret=%u", ret);
		return 1;
	}

	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 0 error");
		return 1;
	}

	ret = ske_lp_gmac_update(ctx, std_msg_, byteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac update 0 error, ret=%u", ret);
		print_buf_U8(std_msg_, byteLen, "std_msg");
		return 1;
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if((SKE_SUCCESS != ret) || (memcmp_(mac_, std_mac_, macByteLen)))
	{
		printf("\r\n ske gmac final 0 error");
		return 1;
	}


	//verify mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_VERIFY_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 1 error, ret=%u", ret);
		return 1;
	}

	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 1 error");
		return 1;
	}

	ret = ske_lp_gmac_update(ctx, std_msg_, byteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac update 1 error, ret=%u", ret);
		print_buf_U8(std_msg_, byteLen, "std_msg");
		return 1;
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac final 1 error");
		return 1;
	}

#else

	//generate mac
	ret = ske_lp_gmac_crypto(alg, SKE_GENERATE_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, std_aad_, aadByteLen,
			std_msg_, byteLen, mac_, macByteLen);
	if((SKE_SUCCESS != ret) || (memcmp_(mac_, std_mac_, macByteLen)))
	{
		printf("\r\n encrypt 1 error, ret=%u", ret);
		return 1;
	}

	//verify mac
	ret = ske_lp_gmac_crypto(alg, SKE_VERIFY_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, std_aad_, aadByteLen,
			std_msg_, byteLen, mac_, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n decrypt 1 error, ret=%u", ret);
		return 1;
	}
#endif

	printf("\r\n one time input test success!!");//fflush(stdout);


	/**************** test 2: multiple style(one block every time) ******************/
	memset_(mac_, 0, 16);

	//generate mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_GENERATE_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 2 error");
		return 1;
	}

	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 2 error");
		return 1;
	}

	for(i=0; i<byteLen; i+=block_byteLen)
	{
		ret = ske_lp_gmac_update(ctx, std_msg_+i, GET_MIN_LEN(block_byteLen, byteLen-i));
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac update 2 error");
			return 1;
		}
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if((SKE_SUCCESS != ret) || (memcmp_(mac_, std_mac_, macByteLen)))
	{
		printf("\r\n ske gmac final 2 error");
		return 1;
	}

	//verify mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_VERIFY_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 3 error, ret=%u", ret);
		return 1;
	}

	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 3 error");
		return 1;
	}

	for(i=0;i<byteLen;i+=block_byteLen)
	{
		ret = ske_lp_gmac_update(ctx, std_msg_+i, GET_MIN_LEN(block_byteLen, byteLen-i));
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac update 3 error");
			return 1;
		}
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske final 3 error");
		return 1;
	}

	printf("\r\n multiple input test success(one block every time)!!");


	/**************** test 3: multiple style(random bytes every time) ******************/
	memset_(mac_, 0, 16);

	//generate mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_GENERATE_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 5 error");
		return 1;
	}

	//input aad
#ifdef SKE_LP_GCM_CPU_UPDATE_AAD_BY_STEP
	i=0;
	while(i < aadByteLen)
	{
		j = ske_get_rand_number(aadByteLen);
		j = GET_MIN_LEN(j, aadByteLen-i);

		ret = ske_lp_gmac_update_aad(ctx, std_aad_+i, j);
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac aad 5 error");
			return 1;
		}
		i+=j;
	}
#else
	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 5 error");
		return 1;
	}
#endif

	//input msg
	i=0;
	while(i < byteLen)
	{
		j = ske_get_rand_number(byteLen);
		j = GET_MIN_LEN(j, byteLen-i);

		ret = ske_lp_gmac_update(ctx, std_msg_+i, j);
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac update 5 error");
			return 1;
		}
		i+=j;
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac final 5 error");
		return 1;
	}

	//verify mac
	ret = ske_lp_gmac_init(ctx, alg, SKE_VERIFY_MAC, std_key_, sp_key_idx, std_iv_, ivByteLen, aadByteLen, byteLen, macByteLen);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac init 6 error");
		return 1;
	}

	//input aad
#ifdef SKE_LP_GCM_CPU_UPDATE_AAD_BY_STEP
	i=0;
	while(i < aadByteLen)
	{
		j = ske_get_rand_number(aadByteLen);
		j = GET_MIN_LEN(j, aadByteLen-i);

		ret = ske_lp_gmac_update_aad(ctx, std_aad_+i, j);
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac aad 6 error");
			return 1;
		}
		i+=j;
	}
#else
	ret = ske_lp_gmac_aad(ctx, std_aad_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac aad 6 error");
			return 1;
	}
#endif

	//input msg
	i=0;
	while(i < byteLen)
	{
		j = ske_get_rand_number(byteLen);
		j = GET_MIN_LEN(j, byteLen-i);

		ret = ske_lp_gmac_update(ctx, std_msg_+i, j);
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n ske gmac update 6 error");
			return 1;
		}
		i+=j;
	}

	ret = ske_lp_gmac_final(ctx, mac_);
	if(SKE_SUCCESS != ret)
	{
		printf("\r\n ske gmac final 6 error");
		return 1;
	}

	printf("\r\n multiple input test success(random blocks every time)!!");	fflush(stdout);

	return 0;
}


#ifdef SKE_LP_DMA_FUNCTION
#endif



typedef struct {
	SKE_ALG alg;
	const unsigned char *std_msg;
	unsigned int c_bytes;    //byte length of msg
	const unsigned char *std_key;
	const unsigned char *std_iv;
	unsigned int iv_bytes;
	const unsigned char *std_aad;
	unsigned int aad_bytes;
	const unsigned char *std_mac;
	unsigned int mac_bytes;
}SKE_GMAC_TEST_VECTOR;

unsigned int ske_gmac_test_internal(SKE_GMAC_TEST_VECTOR *vector, unsigned int num)
{
	char *name[]={"","","","","","AES_128","AES_192","AES_256","SM4"};
	unsigned int i, ret;

	printf("\r\n\r\n  =================== %s GMAC test ==================== ", name[vector[0].alg]);

	for(i=0; i<num; i++)
	{
		ret = ske_gmac_test(vector[i].alg, 1, (unsigned char *)vector[i].std_msg, vector[i].c_bytes, (unsigned char *)vector[i].std_key, 0,
				(unsigned char *)vector[i].std_iv, vector[i].iv_bytes, (unsigned char *)vector[i].std_aad, vector[i].aad_bytes, (unsigned char *)vector[i].std_mac,
				16);
		if(ret)
			return 1;

		ret = ske_gmac_test(vector[i].alg, 0, (unsigned char *)vector[i].std_msg, vector[i].c_bytes, (unsigned char *)vector[i].std_key, 0,
				(unsigned char *)vector[i].std_iv, vector[i].iv_bytes, (unsigned char *)vector[i].std_aad, vector[i].aad_bytes, (unsigned char *)vector[i].std_mac,
				vector[i].mac_bytes);
		if(ret)
			return 1;

#ifdef SKE_LP_DMA_FUNCTION
#endif
	}

	return 0;
}


#if  (defined(SUPPORT_SKE_AES_128) || defined(SUPPORT_SKE_AES_192) || defined(SUPPORT_SKE_AES_256) || defined(SUPPORT_SKE_SM4))
const unsigned char std_gmac_msg[256] =  {0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
									0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
									0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
									0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39,0x1a,0xaf,0xd2,0x55,
									0xB4,0xD1,0x4B,0x82,0xE0,0x03,0xCA,0x48,0x4C,0x28,0x1B,0x5D,0x4D,0x9B,0xBC,0x29,
									0xF3,0x61,0xD1,0x61,0x79,0x36,0xF5,0x3E,0x37,0xEF,0x97,0x73,0xB3,0x31,0x8C,0x8C,
									0x4E,0xCB,0x31,0x9E,0x26,0x4D,0xF6,0x29,0xBE,0x8C,0x41,0x1B,0xF5,0xC4,0xBE,0x45,
									0x03,0x62,0x56,0xE4,0xD8,0x2F,0x64,0x5A,0x4D,0x0A,0xDF,0x44,0x55,0xB1,0x31,0xA1,
									0x7B,0xC6,0x74,0x21,0x86,0xC4,0x2E,0x28,0x3A,0xA5,0xBA,0xD5,0x4A,0x18,0xEC,0x14,
									0x3B,0xAF,0x1E,0x6C,0x85,0x7F,0x32,0xEC,0x5D,0x92,0xB1,0x96,0xCF,0x7E,0x31,0x13,
									0x71,0xBD,0xA2,0xEA,0xE8,0xDF,0xD0,0x82,0xCF,0xEB,0x58,0x3A,0x41,0xCF,0xCE,0xAA,
									0x78,0x0F,0xD1,0x7F,0xBA,0xE9,0x31,0xDE,0xF7,0x17,0xB4,0x5B,0xB8,0x6F,0x15,0xD6,
									0x01,0xF5,0x44,0xFB,0xCC,0xB2,0x43,0x5F,0x80,0xE8,0x9D,0x6F,0x21,0x87,0x06,0x37,
									0x86,0x01,0xEA,0x99,0x15,0x6E,0x8A,0x88,0xAA,0xED,0x8C,0xE6,0x14,0x2F,0xB6,0x65,
									0xB6,0xEE,0xEB,0x6F,0x06,0xEA,0x9D,0x45,0xB0,0x14,0x04,0x00,0x59,0xB4,0x26,0x77,
									0x32,0xD7,0x33,0xB2,0xF0,0x92,0xE8,0x5E,0x40,0xF2,0x21,0x04,0xC4,0xF0,0x9C,0x80,};
const unsigned char std_gmac_key[32]  =  {0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08,
									0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08};
const unsigned char std_gmac_iv[12]   =  {0xca,0xfe,0xba,0xbe,0xfa,0xce,0xdb,0xad,0xde,0xca,0xf8,0x88};
const unsigned char std_gmac_aad[33]  =  {0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,
									0xab,0xad,0xda,0xd2,0x7C,0xA4,0x8E,0x82,0x99,0xFC,0x5A,0xD3,0xE9,0x08,0xB7,0x65,
									0x1B,};
#endif



#ifdef SUPPORT_SKE_AES_128
const unsigned char std_aes128_gmac_aad0_p256[]  = {0x97,0x5A,0x7B,0x3E,0x2E,0x76,0x19,0x59,0x4E,0xFB,0x4A,0x12,0xB3,0x88,0xB3,0xD0};
const unsigned char std_aes128_gmac_aad3_p201[]  = {0xF0,0x0A,0x05,0xCF,0xD8,0x11,0x2B,0x6C,0x08,0x4B,0xAA,0xAC,0x06,0xEB,0xDB,0xAF};
const unsigned char std_aes128_gmac_aad16_p120[] = {0x97,0x79,0x9F,0xA3,0xFC,0x35,0x41,0x33,0x41,0xA6,0x09,0xBC,0xCF,0x42,0x89,0xAF};
const unsigned char std_aes128_gmac_aad17_p65[]  = {0xD9,0x89,0xF7,0xFF,0xD8,0xED,0xD4,0x99,0x4B,0x1D,0x1D,0x54,0x00,0x7F,0x3F,0x7B};
const unsigned char std_aes128_gmac_aad20_p32[]  = {0xE1,0x3E,0x14,0x34,0x28,0x5A,0x94,0x26,0xAD,0xDF,0xBF,0xC2,0x70,0xD2,0x7F,0x16};
const unsigned char std_aes128_gmac_aad32_p15[]  = {0x09,0x24,0xCA,0x52,0xC7,0x81,0x92,0x2E,0xC5,0x3B,0x9F,0x55,0x8F,0xDB,0x6C,0x80};
const unsigned char std_aes128_gmac_aad33_p0[]   = {0x3C,0xBA,0x19,0x87,0x0D,0xFD,0x4D,0xDD,0x35,0x0F,0x6D,0xFB,0xF1,0x90,0xD7,0x4E};

unsigned int aes_128_gmac_test()
{
	SKE_GMAC_TEST_VECTOR vector[7] = {
		{SKE_ALG_AES_128, std_gmac_msg, 256, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 0,  std_aes128_gmac_aad0_p256,  16},
		{SKE_ALG_AES_128, std_gmac_msg, 201, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 3,  std_aes128_gmac_aad3_p201,  13},
		{SKE_ALG_AES_128, std_gmac_msg, 120, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 16, std_aes128_gmac_aad16_p120, 10},
		{SKE_ALG_AES_128, std_gmac_msg, 65,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 17, std_aes128_gmac_aad17_p65,  9},
		{SKE_ALG_AES_128, std_gmac_msg, 32,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 20, std_aes128_gmac_aad20_p32,  7},
		{SKE_ALG_AES_128, std_gmac_msg, 15,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 32, std_aes128_gmac_aad32_p15,  4},
		{SKE_ALG_AES_128, std_gmac_msg, 0,   std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 33, std_aes128_gmac_aad33_p0,   1},
	};

	return ske_gmac_test_internal(vector, 7);
}
#endif


#ifdef SUPPORT_SKE_AES_192
const unsigned char std_aes192_gmac_aad0_p256[]  = {0x36,0x21,0x7A,0xB6,0x75,0x39,0x55,0x13,0x44,0x16,0x28,0xB8,0xCB,0x94,0x45,0x16};
const unsigned char std_aes192_gmac_aad3_p201[]  = {0x16,0x19,0x79,0x41,0xF7,0x31,0x5D,0x4E,0x42,0x64,0xA5,0xEE,0x45,0x52,0x60,0xB2};
const unsigned char std_aes192_gmac_aad16_p120[] = {0x7F,0x6C,0x15,0xD0,0x46,0x5E,0xF5,0xC4,0x75,0x06,0xE6,0x7F,0x68,0xEC,0xA6,0xA8};
const unsigned char std_aes192_gmac_aad17_p65[]  = {0x3F,0x55,0x20,0xDA,0x76,0x8B,0x45,0x99,0xF9,0x45,0xED,0xF8,0xC4,0x2D,0x37,0x75};
const unsigned char std_aes192_gmac_aad20_p32[]  = {0x9C,0x95,0x3E,0x3F,0xDE,0xB9,0xDF,0x33,0xE4,0x8E,0xA6,0xC9,0xCC,0x3C,0x0C,0x8E};
const unsigned char std_aes192_gmac_aad32_p15[]  = {0xAD,0xDC,0xEC,0xF8,0x05,0x6C,0x1B,0xEE,0xB3,0x87,0x2F,0xFE,0xBD,0x74,0x22,0x23};
const unsigned char std_aes192_gmac_aad33_p0[]   = {0xCD,0x15,0xAB,0xBC,0xBC,0x4A,0xDA,0x30,0xB7,0x36,0x03,0xAC,0xD7,0x0F,0x65,0xB2};

unsigned int aes_192_gmac_test()
{
	SKE_GMAC_TEST_VECTOR vector[7] = {
		{SKE_ALG_AES_192, std_gmac_msg, 256, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 0,  std_aes192_gmac_aad0_p256,  16},
		{SKE_ALG_AES_192, std_gmac_msg, 201, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 3,  std_aes192_gmac_aad3_p201,  13},
		{SKE_ALG_AES_192, std_gmac_msg, 120, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 16, std_aes192_gmac_aad16_p120, 10},
		{SKE_ALG_AES_192, std_gmac_msg, 65,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 17, std_aes192_gmac_aad17_p65,  9},
		{SKE_ALG_AES_192, std_gmac_msg, 32,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 20, std_aes192_gmac_aad20_p32,  7},
		{SKE_ALG_AES_192, std_gmac_msg, 15,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 32, std_aes192_gmac_aad32_p15,  4},
		{SKE_ALG_AES_192, std_gmac_msg, 0,   std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 33, std_aes192_gmac_aad33_p0,   1},
	};

	return ske_gmac_test_internal(vector, 7);
}
#endif



#ifdef SUPPORT_SKE_AES_256
const unsigned char std_aes256_gmac_aad0_p256[]  = {0xA0,0x6D,0x50,0x80,0xEA,0xA8,0x51,0x7B,0x5F,0x4C,0xBC,0x2B,0x6A,0x2A,0x19,0x25};
const unsigned char std_aes256_gmac_aad3_p201[]  = {0x19,0xE1,0x84,0x49,0xF5,0x71,0x9F,0xB4,0x90,0x28,0x86,0x17,0x20,0xD9,0x0F,0xAD};
const unsigned char std_aes256_gmac_aad16_p120[] = {0xAC,0x9B,0x6E,0x77,0x01,0xBE,0xBA,0x2D,0x5D,0x9B,0xC2,0x91,0xF0,0x83,0x49,0xAF};
const unsigned char std_aes256_gmac_aad17_p65[]  = {0xBC,0x52,0x87,0x18,0x08,0x72,0xCF,0x53,0x08,0x9E,0xB3,0x48,0x66,0x41,0xB9,0xFA};
const unsigned char std_aes256_gmac_aad20_p32[]  = {0xCF,0x9B,0xFF,0xA9,0xBD,0x33,0x4D,0x82,0x40,0xC8,0x68,0x34,0x2B,0x36,0xB5,0x06};
const unsigned char std_aes256_gmac_aad32_p15[]  = {0x8A,0x86,0xAA,0x09,0xB5,0x10,0x91,0x76,0xEC,0x4B,0x6D,0xE7,0x69,0x6D,0x5B,0x89};
const unsigned char std_aes256_gmac_aad33_p0[]   = {0x53,0x1B,0x3B,0x5F,0xC4,0xED,0x0F,0x01,0xDD,0x67,0x8C,0x38,0x4F,0x32,0xCE,0x87};

unsigned int aes_256_gmac_test()
{
	SKE_GMAC_TEST_VECTOR vector[7] = {
		{SKE_ALG_AES_256, std_gmac_msg, 256, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 0,  std_aes256_gmac_aad0_p256,  16},
		{SKE_ALG_AES_256, std_gmac_msg, 201, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 3,  std_aes256_gmac_aad3_p201,  13},
		{SKE_ALG_AES_256, std_gmac_msg, 120, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 16, std_aes256_gmac_aad16_p120, 10},
		{SKE_ALG_AES_256, std_gmac_msg, 65,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 17, std_aes256_gmac_aad17_p65,  9},
		{SKE_ALG_AES_256, std_gmac_msg, 32,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 20, std_aes256_gmac_aad20_p32,  7},
		{SKE_ALG_AES_256, std_gmac_msg, 15,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 32, std_aes256_gmac_aad32_p15,  4},
		{SKE_ALG_AES_256, std_gmac_msg, 0,   std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 33, std_aes256_gmac_aad33_p0,   1},
	};

	return ske_gmac_test_internal(vector, 7);
}
#endif


#ifdef SUPPORT_SKE_SM4
const unsigned char std_sm4_gmac_aad0_p256[]  = {0x87,0x60,0x91,0xAC,0x80,0x6C,0x53,0x62,0x04,0x64,0x16,0x62,0xDD,0xC2,0x17,0xDE};
const unsigned char std_sm4_gmac_aad3_p201[]  = {0x0B,0xA4,0xC7,0xA8,0x89,0xB4,0xF9,0xCC,0x9B,0x54,0x76,0x35,0x22,0x78,0xCE,0x4F};
const unsigned char std_sm4_gmac_aad16_p120[] = {0xFC,0xA6,0xC2,0x34,0xF7,0xFE,0x8A,0x73,0xF1,0x96,0x93,0xB2,0x42,0x3A,0x44,0x56};
const unsigned char std_sm4_gmac_aad17_p65[]  = {0xC1,0xC7,0xC5,0x2B,0xB7,0x1A,0x2A,0xF9,0x68,0x24,0x3C,0x3C,0xDB,0x10,0x6B,0x21};
const unsigned char std_sm4_gmac_aad20_p32[]  = {0x3E,0x1F,0x43,0xCD,0xE8,0x00,0xA9,0xBE,0x5A,0xA5,0xA1,0x08,0x2C,0xF7,0x74,0x46};
const unsigned char std_sm4_gmac_aad32_p15[]  = {0x3E,0xCE,0x23,0x5C,0x9F,0xE4,0x3F,0x6C,0x46,0xC2,0xC9,0xFF,0x8F,0x94,0x7E,0xBA};
const unsigned char std_sm4_gmac_aad33_p0[]   = {0xC8,0xEB,0x62,0xC2,0xAE,0x2C,0x9F,0x9C,0x9D,0x87,0x0C,0x4C,0x14,0x89,0xF7,0xE9};

unsigned int sm4_gmac_test()
{
	SKE_GMAC_TEST_VECTOR vector[7] = {
		{SKE_ALG_SM4, std_gmac_msg, 256, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 0,  std_sm4_gmac_aad0_p256,  16},
		{SKE_ALG_SM4, std_gmac_msg, 201, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 3,  std_sm4_gmac_aad3_p201,  13},
		{SKE_ALG_SM4, std_gmac_msg, 120, std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 16, std_sm4_gmac_aad16_p120, 10},
		{SKE_ALG_SM4, std_gmac_msg, 65,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 17, std_sm4_gmac_aad17_p65,  9},
		{SKE_ALG_SM4, std_gmac_msg, 32,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 20, std_sm4_gmac_aad20_p32,  7},
		{SKE_ALG_SM4, std_gmac_msg, 15,  std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 32, std_sm4_gmac_aad32_p15,  4},
		{SKE_ALG_SM4, std_gmac_msg, 0,   std_gmac_key, std_gmac_iv, 12, std_gmac_aad, 33, std_sm4_gmac_aad33_p0,   1},
	};

	return ske_gmac_test_internal(vector, 7);
}
#endif


//*
unsigned int ske_gmac_speed_test_internal(SKE_ALG alg, char *info)
{
	unsigned char in[4096];
	unsigned char key[32];
	unsigned char aad[100];
	unsigned char iv[16];
	unsigned char mac[16];
	unsigned int iv_bytes = 12;
	unsigned int aad_bytes = 100;
	unsigned int c_bytes = 4096;
	unsigned int mac_bytes = 15;
	unsigned int i;
	unsigned int round = 100;
	unsigned int ret;

	uint32_sleep(0xFFFF, 1);

	round = speed_get_round_by_alg(alg);

#ifndef GMAC_SPEED_TEST_BY_TIMER
	round *= 100;
#endif

	printf("\r\n %s begin", info); fflush(stdout);

#ifdef GMAC_SPEED_TEST_BY_TIMER
	startP();
#endif

	for(i=0;i<round;i++)
	{
		ret = ske_lp_gmac_crypto(alg, SKE_GENERATE_MAC, key, 0, iv,
				iv_bytes, aad, aad_bytes, in, c_bytes, mac, mac_bytes);
		if(SKE_SUCCESS != ret)
		{
			printf("\r\n gmac crypto error ret=%u", ret); fflush(stdout);
			return 1;
		}
	}

#ifdef GMAC_SPEED_TEST_BY_TIMER
	endP(0,c_bytes,round);
#else
	printf("\r\n finished"); fflush(stdout);
#endif

	return 0;
}


unsigned int ske_gmac_speed_test()
{
	ske_gmac_speed_test_internal(SKE_ALG_AES_128, "SKE_ALG_AES_128 GMAC");
	ske_gmac_speed_test_internal(SKE_ALG_AES_192, "SKE_ALG_AES_192 GMAC");
	ske_gmac_speed_test_internal(SKE_ALG_AES_256, "SKE_ALG_AES_256 GMAC");
	ske_gmac_speed_test_internal(SKE_ALG_SM4, "SKE_ALG_SM4 GMAC");

	return 0;
}


#ifdef SKE_LP_DMA_FUNCTION
#endif
//*/


unsigned int SKE_LP_GMAC_all_test()
{
	unsigned int ret;

#if 0
	if(SKE_SUCCESS != ske_gmac_speed_test())
		return 1;
#endif

#ifdef SKE_LP_DMA_FUNCTION
#endif

//*
#ifdef SUPPORT_SKE_AES_128
	ret = aes_128_gmac_test();
	if(ret)
		return 1;
#endif

#ifdef SUPPORT_SKE_AES_192
	ret = aes_192_gmac_test();
	if(ret)
		return 1;
#endif

#ifdef SUPPORT_SKE_AES_256
	ret = aes_256_gmac_test();
	if(ret)
		return 1;
#endif

#ifdef SUPPORT_SKE_SM4
	ret = sm4_gmac_test();
	if(ret)
		return 1;
#endif
//*/

	printf("\r\n\r\n SKE_LP_GMAC_all_test test over! \r\n ");

	return 0;
}


#endif


void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);

#ifdef SKE_LP_DMA_FUNCTION
    telink_ske_dma_init(DMA0, DMA1, DMA_BURST_1_WORD);
#endif /* SKE_LP_DMA_FUNCTION */
    SKE_LP_GMAC_all_test();
}

void main_loop(void)
{
    gpio_toggle(LED1);
    delay_ms(50);
}

#endif

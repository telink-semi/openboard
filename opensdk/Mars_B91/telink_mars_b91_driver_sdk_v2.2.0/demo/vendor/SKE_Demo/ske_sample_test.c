/********************************************************************************************************
 * @file    ske_sample_test.c
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
#if (SKE_TEST_MODE == SKE_SAMPLE_TEST)
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#ifdef SUPPORT_SKE_AES_128

//extern void ske_call_manage(void);

//callback function
void ske_call_manage(void)
{
	;
}


static int hexchar2int(unsigned char c)
{

    switch (c) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
          return 4;
    case '5':
          return 5;
    case '6':
          return 6;
    case '7':
          return 7;
    case '8':
          return 8;
    case '9':
          return 9;
    case 'a': case 'A':
          return 0x0A;
    case 'b': case 'B':
          return 0x0B;
    case 'c': case 'C':
          return 0x0C;
    case 'd': case 'D':
          return 0x0D;
    case 'e': case 'E':
          return 0x0E;
    case 'f': case 'F':
          return 0x0F;
    }
    return -1;
}

int set_hex(const char *in, unsigned char *out, int size)
{
    int i, n;
    unsigned char j;

    i = size * 2;
    n = strlen(in);
    if (n > i) {
        printf("hex string is too long, ignoring excess\n");
        n = i; /* ignore exceeding part */
    } else if (n < i) {
    	printf("hex string is too short, padding with zero bytes to length\n");
    }

    memset(out, 0, size);
    for (i = 0; i < n; i++) {
        j = (unsigned char)*in++;
        if (!isxdigit(j)) {
            printf("non-hex digit\n");
            return 0;
        }
        j = (unsigned char)hexchar2int(j);
        if (i & 1)
            out[i / 2] |= j;
        else
            out[i / 2] = (j << 4);
    }
    return 1;
}

#if defined(SUPPORT_SKE_MODE_ECB)
unsigned int ske_lp_aes128_ecb_sample()
{

	unsigned char std_cipher_ascii[] = "0B54E59F47144FD8EE4356CA0B2D7A4B84D81726E28F59AD95568C52DA983F8D65EDDD875890ECE7D803669DE2F35795";
	unsigned char std_in_ascii[] = "81709944E0CB2E1DB5B0A477D1A8539B0A8786E34EAAED99303EA6975595B2454D5D7F91EBBD4ACD726C0E0E5E3EB55E";
	unsigned char std_key_ascii[] = "E07099F1BFAFFD7F240CD790CA4FE134";

	unsigned char std_cipher[128] 	= {0};
	unsigned char std_in[128] 		= {0};
	unsigned char std_key[64] 		= {0};
	unsigned char tmp[128] 		= {0};

	memset(std_cipher, 	0, sizeof(std_cipher));
	memset(std_in, 	0, sizeof(std_in));
	memset(std_key, 0, sizeof(std_key));
	memset(tmp,  0, sizeof(tmp));
	set_hex((char *)std_cipher_ascii,std_cipher, sizeof(std_cipher));
	set_hex((char *)std_in_ascii, std_in,sizeof(std_in_ascii));
	set_hex((char *)std_key_ascii, std_key,sizeof(std_key_ascii));



	unsigned int ret;
//	unsigned int *std_in_, *std_out_, *in, *out, *out2;

	unsigned int *std_in_, *std_out_, *out, *out2;


	/************** CPU style ***************/
	//----------------------------encrypt-decrypt--------------------------------------------
	 ret = ske_lp_ecb_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_in, tmp, 48);
	 if(SKE_SUCCESS != ret ||memcmp(std_cipher, tmp,48))
	 {
	 	printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
	 	return 1;
	 }
	 else
	 {
	 	printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	 }

	 ret = ske_lp_ecb_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_cipher, tmp, 48);
	 if(SKE_SUCCESS != ret ||memcmp(std_in, tmp, 48))
	 {
	 	printf("%s line-%04d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
	 	return 1;
	 }
	 else
	 {
	 	printf("%s line-%0d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	 }

//	exit(0);
	//----------------------------encrypt--------------------------------------------
	ret = ske_lp_ecb_init(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0);
	if(SKE_SUCCESS != ret)
	{
		printf("%s line-%d init FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	ret = ske_lp_ecb_update_blocks(std_in, tmp, 16);
	ret |= ske_lp_ecb_update_blocks(std_in+16, tmp+16, 32);

	if(SKE_SUCCESS != ret ||memcmp(std_cipher, tmp, 48))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}


	//-----------------------------DECRYPT-------------------------------------------
	ret = ske_lp_ecb_init(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0);
	if(SKE_SUCCESS != ret)
	{
		printf("%s line-%d init FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	ret = ske_lp_ecb_update_blocks(std_cipher, tmp, 16);
	ret |= ske_lp_ecb_update_blocks(std_cipher+16, tmp+16, 32);

	if(SKE_SUCCESS != ret ||memcmp(tmp, tmp, 48))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}


#ifdef SKE_LP_DMA_FUNCTION

	/************** DMA style ***************/
	std_in_ = (unsigned int *)DMA_SKE_BASE;
	std_out_ = std_in_+64;

	out = std_out_+64;
	out2 = out+64;

	uint32_copy(std_in_, (unsigned int *)std_in, 48/4);
	uint32_copy(std_out_, (unsigned int *)std_cipher, 48/4);
//	uint32_copy(std_in_, std_in, 48/4);
//	uint32_copy(std_out_, std_cipher, 48/4);

	//ENCRYPT
	ret = ske_lp_dma_ecb_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_in_, out, 48/4, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma encryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	//DECRYPT
	ret = ske_lp_dma_ecb_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, out, out2, 48/4, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma decryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}


	if(uint32_BigNumCmp(out, 48/4, std_out_, 48/4) || uint32_BigNumCmp(out2, 48/4, std_in_, 48/4))
	{
		printf("%s line-%d dma encryption/decryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d dma enc/dec success\r\n", __func__, __LINE__);fflush(stdout);
	}
	fflush(stdout);

#endif

	return 0;
}
#endif

#if defined(SUPPORT_SKE_MODE_CBC)
unsigned int ske_lp_aes128_cbc_sample()
{
	unsigned char std_in[48]  =      {0x81,0x70,0x99,0x44,0xE0,0xCB,0x2E,0x1D,0xB5,0xB0,0xA4,0x77,0xD1,0xA8,0x53,0x9B,
								0x0A,0x87,0x86,0xE3,0x4E,0xAA,0xED,0x99,0x30,0x3E,0xA6,0x97,0x55,0x95,0xB2,0x45,
								0x4D,0x5D,0x7F,0x91,0xEB,0xBD,0x4A,0xCD,0x72,0x6C,0x0E,0x0E,0x5E,0x3E,0xB5,0x5E,};
	unsigned char std_key[16] =      {0xE0,0x70,0x99,0xF1,0xBF,0xAF,0xFD,0x7F,0x24,0x0C,0xD7,0x90,0xCA,0x4F,0xE1,0x34,};
	unsigned char std_iv[16]  =      {0xC7,0x2B,0x65,0x91,0xA0,0xD7,0xDE,0x8F,0x6B,0x40,0x72,0x33,0xAD,0x35,0x81,0xD6};
	unsigned char std_cipher[48]   = {0x2C,0x1E,0xD4,0x56,0x36,0x2E,0x00,0x85,0xA8,0x1D,0x8E,0x61,0x69,0xAD,0x38,0xB7,
								0xB4,0x42,0x60,0xE1,0x56,0x8D,0x9E,0x85,0x0A,0x0C,0x95,0x37,0x44,0x02,0xDE,0x28,
								0x24,0xD5,0x05,0x61,0x30,0x42,0x42,0x86,0x0A,0xE7,0x17,0x3D,0xDD,0x19,0xEC,0x5B,};

	unsigned char cipher[48], replain[48];
	unsigned int ret;
//	unsigned int *std_in_, *std_out_, *in, *out, *out2;
	unsigned int *std_in_, *std_out_, *out, *out2;


	/************** CPU style ***************/
	//----------------------------encrypt---------------------------------------------
	ret = ske_lp_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, std_in, cipher, 48);
	if(SKE_SUCCESS != ret ||memcmp(cipher, std_cipher, 48))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}


	//----------------------------decrypt---------------------------------------------
	ret = ske_lp_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, std_cipher, replain, 48);
	if(SKE_SUCCESS != ret ||memcmp(replain, std_in, 48))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}

	//--------------------------encrypt-----------------------------------------------
	ret = ske_lp_cbc_init(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv);
	if(SKE_SUCCESS != ret)
	{
		printf("%s line-%d init  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	ret = ske_lp_cbc_update_blocks(std_in, cipher, 16);
	ret |= ske_lp_cbc_update_blocks(std_in+16, cipher+16, 32);

	if(SKE_SUCCESS != ret ||memcmp(cipher, std_cipher, 48))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}

	//--------------------------decrypt-----------------------------------------------
	ret = ske_lp_cbc_init(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv);
	if(SKE_SUCCESS != ret)
	{
		printf("%s line-%d init  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	ret = ske_lp_cbc_update_blocks(std_cipher, replain, 16);
	ret |= ske_lp_cbc_update_blocks(std_cipher+16, replain+16, 32);

	if(SKE_SUCCESS != ret ||memcmp(std_in, replain, 48))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}


#ifdef SKE_LP_DMA_FUNCTION


	/************** DMA style ***************/
	std_in_ = (unsigned int *)DMA_SKE_BASE;
	std_out_ = std_in_+64;

	out = std_out_+64;
	out2 = out+64;

	uint32_copy(std_in_, (unsigned int *)std_in, 48/4);
	uint32_copy(std_out_, (unsigned int *)std_cipher, 48/4);

//	uint32_copy(std_in_, std_in, 48/4);
//	uint32_copy(std_out_, std_cipher, 48/4);


	//--------------------------DMA mult api ENCRYPT-----------------------------------------------
	ret = ske_lp_dma_cbc_init(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv);
	if(ret)
	{
		printf("%s line-%d dma encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	ret = ske_lp_dma_cbc_update_blocks(std_in_, out, 4, ske_call_manage);
	ret |= ske_lp_dma_cbc_update_blocks(std_in_+4, out+4, 8, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma encryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	//--------------------------DMA mult api DECRYPT-----------------------------------------------
	ret = ske_lp_dma_cbc_init(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv);
	if(ret)
	{
		printf("%s line-%d dma decryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	ret = ske_lp_dma_cbc_update_blocks(out, out2, 8, ske_call_manage);
	ret |= ske_lp_dma_cbc_update_blocks(out+8, out2+8, 4, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma decryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	//--------------------------DMA one api ENCRYPT-----------------------------------------------
	ret = ske_lp_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, std_key, 0, std_iv, std_in_, out, 48/4, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma encryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}


	//--------------------------DMA one api DECRYPT-----------------------------------------------
	ret = ske_lp_dma_cbc_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, std_key, 0, std_iv, out, out2, 48/4, ske_call_manage);
	if(ret)
	{
		printf("%s line-%d dma decryption FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}


	if(uint32_BigNumCmp(out, 48/4, std_out_, 48/4) || uint32_BigNumCmp(out2, 48/4, std_in_, 48/4))
	{
		printf("%s line-%d dma enc/dec FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d dma enc/dec success\r\n", __func__, __LINE__);fflush(stdout);
	}
	fflush(stdout);

#endif

	return 0;
}
#endif




#if defined(SUPPORT_SKE_MODE_GCM)
unsigned int ske_lp_aes128_gcm_sample()
{
	unsigned char std_plain[64]={0xd9,0x31,0x32,0x25,0xf8,0x84,0x06,0xe5,0xa5,0x59,0x09,0xc5,0xaf,0xf5,0x26,0x9a,
							0x86,0xa7,0xa9,0x53,0x15,0x34,0xf7,0xda,0x2e,0x4c,0x30,0x3d,0x8a,0x31,0x8a,0x72,
							0x1c,0x3c,0x0c,0x95,0x95,0x68,0x09,0x53,0x2f,0xcf,0x0e,0x24,0x49,0xa6,0xb5,0x25,
							0xb1,0x6a,0xed,0xf5,0xaa,0x0d,0xe6,0x57,0xba,0x63,0x7b,0x39,0x1a,0xaf,0xd2,0x55,};
	unsigned char key[16]      = {0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08};
	unsigned char iv[12]       = {0xca,0xfe,0xba,0xbe,0xfa,0xce,0xdb,0xad,0xde,0xca,0xf8,0x88};
	unsigned char aad[20]      = {0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,
							0xab,0xad,0xda,0xd2};

	unsigned char std_cipher[] = {0x42,0x83,0x1e,0xc2,0x21,0x77,0x74,0x24,0x4b,0x72,0x21,0xb7,0x84,0xd0,0xd4,0x9c,
							0xe3,0xaa,0x21,0x2f,0x2c,0x02,0xa4,0xe0,0x35,0xc1,0x7e,0x23,0x29,0xac,0xa1,0x2e,
							0x21,0xd5,0x14,0xb2,0x54,0x66,0x93,0x1c,0x7d,0x8f,0x6a,0x5a,0xac,0x84,0xaa,0x05,
							0x1b,0xa3,0x0b,0x39,0x6a,0x0a,0xac,0x97,0x3d,0x58,0xe0,0x91,0x47,0x3f,0x59,0x85};
	unsigned char std_mac[]	 = {0xDA,0x80,0xCE,0x83,0x0C,0xFD,0xA0,0x2D,0xA2,0xA2,0x18,0xA1,0x74,0x4F,0x4C,0x76};

	unsigned char cipher[64], replain[64];
	unsigned char mac[16];
	unsigned int ret;
	unsigned int *in, *out, *out2;

	SKE_GCM_CTX ctx[1];


	/************** CPU style ***************/
	//--------------------------encrypt-----------------------------------------------
	ret = ske_lp_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, key, 0, iv,
			12, aad, 20, std_plain, cipher, 64, mac, 16);

	if(SKE_SUCCESS != ret || memcmp(cipher, std_cipher, 64) || memcmp(mac, std_mac, 16))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}
	//--------------------------encrypt-----------------------------------------------
	ret = ske_lp_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, key, 0, iv,
			12, aad, 20, cipher, replain, 64, mac, 16);

	if(SKE_SUCCESS != ret || memcmp(replain, std_plain, 64))
	{

		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}

	//--------------------------encrypt-----------------------------------------------
	ret = ske_lp_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, key, 0,
			iv, 12, 20, 64);

	ret |= ske_lp_gcm_aad(ctx, aad);

	ret |= ske_lp_gcm_update(ctx, std_plain, cipher, 32);
	ret |= ske_lp_gcm_update(ctx, std_plain+32, cipher+32, 32);

	ret |= ske_lp_gcm_final(ctx, mac, 16);
	if(SKE_SUCCESS != ret || memcmp(cipher, std_cipher, 64) || memcmp(mac, std_mac, 16))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d encryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}


	//--------------------------decrypt-----------------------------------------------
	ret = ske_lp_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, key, 0,
			iv, 12, 20, 64);

	ret |= ske_lp_gcm_aad(ctx, aad);

	ret |= ske_lp_gcm_update(ctx, cipher, replain, 16);
	ret |= ske_lp_gcm_update(ctx, cipher+16, replain+16, 32);
	ret |= ske_lp_gcm_update(ctx, cipher+48, replain+48, 16);

	ret |= ske_lp_gcm_final(ctx, mac, 16);
	if(SKE_SUCCESS != ret || memcmp(replain, std_plain, 64))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d decryption  success\r\n", __func__, __LINE__);fflush(stdout);
	}



#ifdef SKE_LP_DMA_FUNCTION
	/************** DMA style ***************/
	in = (unsigned int *)DMA_SKE_BASE;
	out = in+0x100;
	out2 = out+0x200;

	//copy aad
	uint32_copy(in, (unsigned int *)aad, 20/4);
	uint32_clear(in+5, 3);

	//copy plain
	uint32_copy(in+8, (unsigned int *)std_plain, 64/4);

	//encrypt
	ret = ske_lp_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, key, 0, iv, 12, NULL, 20,
			in, out, 64, 16, ske_call_manage);
	if(SKE_SUCCESS != ret || uint32_BigNumCmp(out, 64/4, (unsigned int *)std_cipher, 64/4) ||
			uint32_BigNumCmp(out+64/4, 4, (unsigned int *)std_mac, 4))
	{
		printf("%s line-%d encryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}

	//copy aad
	uint32_copy(in, (unsigned int *)aad, 20/4);
	uint32_clear(in+5, 3);

	//copy cipher
	uint32_copy(in+8, (unsigned int *)out, 64/4);

	//decrypt
	ret = ske_lp_dma_gcm_crypto(SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, key, 0, iv, 12, NULL, 20,
			in, out2, 64, 16, ske_call_manage);

	if(SKE_SUCCESS != ret || uint32_BigNumCmp(out2, 64/4, (unsigned int *)std_plain, 64/4) ||
			uint32_BigNumCmp(out2+64/4, 4, (unsigned int *)std_mac, 4))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d dma enc/dec success\r\n", __func__, __LINE__);fflush(stdout);
	}




	//--------------------------DMA encrypt-----------------------------------------------
	in = (unsigned int *)DMA_SKE_BASE;
	out = in+0x100;
	out2 = out+0x200;
	ret = 0;
	//copy aad
	uint32_copy(in, (unsigned int *)aad, 20/4);
	uint32_clear(in+5, 3);

	//copy plain
	uint32_copy(in+8, (unsigned int *)std_plain, 64/4);

	//encrypt
	ret = ske_lp_dma_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_ENCRYPT, key, 0, iv, 12, 20, 64);
	ret = ske_lp_dma_gcm_aad_blocks(ctx, in,ske_call_manage);
	ret = ske_lp_dma_gcm_update_blocks(ctx, in+8, out, 16, ske_call_manage);
	ske_lp_dma_gcm_final(ctx);


	//copy aad
	uint32_copy(in, (unsigned int *)aad, 20/4);
	uint32_clear(in+5, 3);
	//copy cipher
	uint32_copy(in+8, (unsigned int *)out, 64/4);
	ret = ske_lp_dma_gcm_init(ctx, SKE_ALG_AES_128, SKE_CRYPTO_DECRYPT, key, 0, iv, 12, 20, 64);
	ret = ske_lp_dma_gcm_aad_blocks(ctx, in,ske_call_manage);
	ret = ske_lp_dma_gcm_update_blocks(ctx, in+8, out2, 16, ske_call_manage);
	ske_lp_dma_gcm_final(ctx);


	if(SKE_SUCCESS != ret || uint32_BigNumCmp(out2, 64/4, (unsigned int *)std_plain, 64/4) ||
			uint32_BigNumCmp(out2+64/4, 4, (unsigned int *)std_mac, 4))
	{
		printf("%s line-%d decryption  FAIL\r\n", __func__, __LINE__);fflush(stdout);
		return 1;
	}
	else
	{
		printf("%s line-%d dma enc/dec success\r\n", __func__, __LINE__);fflush(stdout);
	}



#endif

	return 0;
}




unsigned char SKE_sample_all_test()
{
	printf("\r\n\r\n SKE sample test start! \r\n\r\n");
#if 0
	if(ske_speed_test())
		return 1;
#endif

#ifdef SKE_LP_DMA_FUNCTION
#if 0
	if(ske_dma_speed_test())
		return 1;
#endif
#endif

#ifdef SUPPORT_SKE_MODE_CBC
	if(ske_lp_aes128_cbc_sample())
		return 1;
#endif

#ifdef SUPPORT_SKE_MODE_GCM
	if(ske_lp_aes128_gcm_sample())
		return 1;
#endif

#ifdef SUPPORT_SKE_SM4
	if(ske_lp_aes128_ecb_sample())
		return 1;
#endif



#if 0
#ifdef SUPPORT_SKE_DES
	if(DES_Test())
		return 1;
#endif
#endif

	printf("\r\n\r\n SKE sample test over! \r\n");

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

	SKE_sample_all_test();

}

void main_loop (void)
{
	delay_ms(500);
	gpio_toggle(LED1);
}

#endif


#endif
#endif



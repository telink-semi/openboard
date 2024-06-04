/********************************************************************************************************
 * @file    chacha20_poly1305_test.c
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
#include <stdio.h>
#include <stdlib.h>
#include "app_config.h"

unsigned int chacha20_poly1305_rand_test(void);

#define CHACHA20_POLY1305_SPEED_TEST_BY_TIMER
#define CHACHA20_SPEED_TEST_BY_TIMER

#ifdef CHACHA20_SPEED_TEST_BY_TIMER
extern unsigned int startP(void);
extern unsigned int endP(unsigned char mode, unsigned int once_bytes, unsigned int round);
#endif

//the following is for speed test by using Timer(on ARM-CM3/Keil5), if you don't need, you can disable this part.
//and if you need, you may need to rewrite this on your platform.
#if 0

#include "CM3DS_MPS2.h"

#define TIMER_MAX_COUNTER   0xFFFFFF

#define DEFAULT_CPU_Hz      200u   //MHz
#define CPU_Hz              10u    //MHz
#define STDCNT              1000000u  

unsigned int volatile g_irq_timer_counter = 0;
unsigned int g_starl = 0;
unsigned int g_endl = 0;

unsigned int calcP(unsigned char mode, unsigned int once_bytes, unsigned int round)
{
    unsigned int counterH,counterL;
    unsigned int msg_bits;
    float ts;
    float res;

    counterL = g_irq_timer_counter*TIMER_MAX_COUNTER;
    if(g_starl > g_endl)
    {
        counterL += g_starl - g_endl;
    }
    printf("\r\n startL = %u cycle, endL = %u cycle, counterL = %u cycle", g_starl, g_endl, counterL);//fflush(stdout);
    ts = counterL/(float)CPU_Hz;
    printf("\r\n ts = %.6f s", ts / (float)1000 / (float)1000);//fflush(stdout);

    switch(mode)
    {
        case 0:// test mbps/s
            msg_bits = once_bytes*round*8;
            printf("\r\n meslen = %u bit", msg_bits);//fflush(stdout);
            res = msg_bits/ts;
            res = res*1000/1024;
            res = res*1000/1024;
            printf("\r\n res = %.6fMBps = %.3fMbps;CPU_Hz = %uMhz", res/8,res,CPU_Hz);//fflush(stdout);
            res = res*DEFAULT_CPU_Hz/CPU_Hz;
            printf("\r\n exres = %.3fMBps = %.3fMbps;exCPU_Hz = %uMhz", res/8,res,DEFAULT_CPU_Hz);//fflush(stdout);
            break;

        case 1:// test cnt/s
            res = round*STDCNT/ts;
            printf("\r\nres = %.3fXXX/s;%.4fms,CPU_Hz = %uMhz", res,1000/res,CPU_Hz);//fflush(stdout);
            res = res*DEFAULT_CPU_Hz/CPU_Hz;
            printf("\r\nexres = %.3f/s;exCPU_Hz = %uMhz", res,DEFAULT_CPU_Hz);//fflush(stdout);
            break;

        default:
            break;
    }

    return 0;
}

void SysTick_Handler(void)
{
    g_irq_timer_counter ++;
}

unsigned int startP(void)
{
    g_irq_timer_counter = 0;
    SysTick->CTRL |= ~SysTick_CTRL_ENABLE_Msk;  //disable SysTick, and this could be ignored. please don't use SysTick->CTRL = 0;
    SysTick_Config(TIMER_MAX_COUNTER);
    g_starl = SysTick->VAL;

    return 0;
}

unsigned int endP(unsigned char mode, unsigned int once_bytes, unsigned int round)
{
    g_endl = SysTick->VAL;
    SysTick->CTRL |= ~SysTick_CTRL_ENABLE_Msk;  //disable SysTick, and this could be ignored. please don't use SysTick->CTRL = 0;
    calcP(mode,once_bytes,round);

    return 0;
}

#endif


unsigned int chacha20_poly1305_speed_test()
{
	unsigned char in[4096];
	unsigned char key[16];
	unsigned int constant = 0;
	unsigned int iv[2];
	unsigned int i;
	unsigned int ret;
    unsigned int round = 20000;

	CHACHA20_POLY1305_CTX ctx[1];

    uint32_sleep(0x1FFFF, 1);

	printf("chacha20_poly1305_speed_test begin\r\n");

#ifdef CHACHA20_POLY1305_SPEED_TEST_BY_TIMER
    round /= 10;
    startP();
#endif

	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, (unsigned char *)iv, NULL, 0);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 init error ret=%x", ret);
		return 1;
	}

	for(i=0;i<round-1;i++)
	{
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, in, in, 4096);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n chacha20_poly1305 crypto error ret=%x", ret);
			return 1;
		}
	}

	ret |= chacha20_poly1305_update_including_last_data(ctx, in, in, 4096, in);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 final error ret=%x", ret);
		return 1;
	}

#ifdef CHACHA20_POLY1305_SPEED_TEST_BY_TIMER
    endP(0, 4096, round);
#else
	printf("\r\n finished");
#endif

	return 0;
}


#ifdef CHACHA20_POLY1305_DMA_FUNCTION
unsigned int chacha20_poly1305_dma_speed_test()
{
	unsigned int *in = (unsigned int *)DMA_RAM_BASE;
	unsigned char key[16];
	unsigned int constant = 0;
	unsigned int iv[2];
	unsigned int i;
	unsigned int ret;
    unsigned int round = 10000;
	CHACHA20_POLY1305_DMA_CTX ctx[1];

	printf("\r\nchacha20_poly1305_dma_speed_test begin\r\n");

#ifdef CHACHA20_POLY1305_SPEED_TEST_BY_TIMER
        round /= 10;
        startP();
#endif

	ret = chacha20_poly1305_dma_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, (unsigned char *)iv, NULL, 0);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 init dma error ret=%x", ret);
		return 1;
	}

	for(i=0;i<round-1;i++)
	{
		ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx, in, in, 16384/4);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n chacha20_poly1305 crypto dma error ret=%x", ret);
			return 1;
		}
	}

	ret |= chacha20_poly1305_dma_update_including_last_data(ctx, in, in, 16384/4, (unsigned char *)in); 
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 final dma error ret=%x", ret);
		return 1;
	}

#ifdef CHACHA20_POLY1305_SPEED_TEST_BY_TIMER
    endP(0, 16384, round);
#else
    printf("\r\n finished");
#endif

	return 0;
}

#endif


typedef struct {
	const unsigned char *std_plain;
	unsigned int c_bytes;    //byte length of plaintext or ciphertext
	const unsigned char *key;
	const unsigned char *iv;
	unsigned int constant;
	const unsigned char *aad;
	unsigned int aad_bytes;
	const unsigned char *std_cipher;
	const unsigned char *std_tag;
}CHACHA20_POLY1305_TEST_VECTOR;


static const unsigned char std_cipher[512] =  {
		0x31,0x20,0x5F,0x1F,0x86,0x46,0xDD,0xF5,0xFC,0x0B,0x4C,0x22,0xB6,0x3E,0xB6,0x40,
		0x6E,0x26,0x22,0xEB,0x08,0xFC,0x6C,0xBE,0x87,0x23,0xEB,0xD9,0x81,0x37,0x2F,0x6C,
		0xBE,0x2E,0xC4,0x8A,0xC8,0x93,0x49,0xFD,0x3F,0xA6,0x31,0x52,0x75,0xC1,0x51,0x00,
		0x45,0x10,0xBA,0xF7,0x4F,0x03,0xAB,0xF4,0xD0,0xC2,0x4E,0x13,0x48,0x30,0x3B,0x9D,
		0x06,0x89,0x26,0x74,0x90,0x8B,0xD3,0x98,0xD1,0x42,0x82,0x43,0x91,0xBF,0xDA,0xEB,
		0xFB,0x00,0x0D,0xDD,0x1D,0xD3,0x39,0x3F,0xEE,0x23,0x3B,0x64,0x5B,0x78,0xE3,0xF8,
		0x71,0xEF,0x9D,0xC0,0xFD,0x04,0x30,0x13,0x87,0x92,0xA8,0x2C,0x17,0xD7,0xE3,0x98,
		0xF8,0x84,0x20,0x47,0x42,0x05,0x5D,0x63,0x66,0x85,0x61,0xB8,0x6C,0x39,0x46,0x85,
		0xAE,0xEA,0xE2,0x11,0xAA,0x70,0x88,0x92,0x59,0xB9,0x06,0x7C,0x7E,0xE8,0x4F,0x4B,
		0x09,0x63,0x5B,0xA8,0xB1,0x1C,0x6E,0x28,0x9C,0xAF,0xCC,0x7D,0x2D,0x18,0xB6,0xD7,
		0xC3,0x88,0x69,0x89,0x66,0xA0,0xB7,0x7F,0xB2,0x9E,0x26,0x0C,0x2A,0xF5,0x1B,0xC8,
		0xFB,0x94,0xCA,0x2F,0x24,0x8A,0x5A,0x38,0xFA,0x50,0xCD,0x5D,0xAC,0x44,0xF8,0xEB,
		0x7A,0xF4,0xDB,0x32,0x5A,0x85,0x74,0xC7,0x85,0x31,0xD8,0x59,0x37,0x3F,0x00,0xCB,
		0x54,0x58,0xD5,0x8C,0xD7,0xA2,0xFE,0x45,0x86,0x66,0x0F,0x66,0x53,0xD4,0x63,0x72,
		0x20,0x33,0x5A,0x68,0x67,0xA3,0xC2,0xA6,0xA2,0xF7,0xAD,0xEE,0x66,0xB0,0xA8,0x9F,
		0x38,0x21,0x53,0x3E,0xBD,0xE4,0x93,0x55,0xE4,0xC8,0xEB,0x2E,0x41,0xB0,0x09,0x80,
		0xC2,0x2F,0x92,0xD2,0xB0,0x17,0x9C,0x92,0x97,0x01,0x99,0x76,0x70,0x86,0xFB,0x08,
		0xAE,0xAF,0xDE,0x20,0x40,0x04,0xD9,0x4F,0x07,0x45,0x6A,0x7B,0x39,0xA8,0x7C,0x72,
		0x31,0x10,0xA5,0x31,0x76,0x29,0x43,0x70,0xB9,0xE1,0x16,0xB1,0xA3,0x0E,0x03,0xFA,
		0x5B,0xCC,0xFA,0x4F,0xB2,0x7B,0xD3,0xD5,0x7D,0x2D,0x5E,0x79,0x4B,0x9F,0xD6,0xF0,
		0x74,0xE8,0xF8,0x53,0x6C,0x1C,0x2D,0x92,0x2E,0x78,0x8D,0xFD,0x6B,0x17,0x79,0x61,
		0xBE,0xDB,0xDD,0x0F,0x61,0x41,0x0B,0x17,0x11,0xD4,0x58,0x1C,0x80,0x4E,0xA0,0xAA,
		0x76,0xD0,0x43,0xA4,0xE4,0x74,0xDE,0xE3,0x1C,0x09,0x69,0xAA,0x2F,0x8F,0x4F,0xEB,
		0x11,0x21,0x4E,0xA5,0x7A,0x1A,0x54,0x26,0x37,0x1D,0x38,0xCA,0xE8,0x2C,0x39,0x48,
		0x62,0xCC,0xB7,0x60,0xC3,0x49,0xF6,0xF5,0x8B,0xFD,0xD4,0xA5,0xE8,0xA0,0xC8,0x7B,
		0x4F,0x67,0xA6,0xD7,0x50,0x12,0xFD,0x96,0x55,0x2D,0x61,0x41,0x4B,0x96,0x76,0x38,
		0xCE,0x6E,0x04,0xCA,0x80,0x1A,0x76,0x38,0x35,0xA6,0x0B,0xEE,0x51,0x9E,0xDF,0x82,
		0x6C,0x0D,0xEE,0x5B,0x03,0x47,0x1A,0x43,0x1B,0x9F,0x3E,0x46,0xAD,0x37,0x25,0x7B,
		0xC1,0x7E,0xB4,0x52,0x5B,0xDE,0x32,0x12,0xA0,0x41,0xF8,0x54,0xAE,0x2A,0xEE,0x59,
		0xF3,0xBF,0x88,0xA5,0x4E,0x22,0xD8,0x74,0xF2,0x4D,0xAA,0x0D,0x5F,0x8C,0xAF,0x4A,
		0xE5,0x06,0x2A,0xD2,0x97,0xCB,0xB9,0xC4,0x69,0x82,0x43,0xED,0xA5,0x74,0x3B,0x0D,
		0xB5,0x87,0x1B,0xCD,0xC7,0x0A,0x3C,0x96,0x71,0xAE,0x57,0xA0,0x5B,0x0B,0x10,0x04};
static const unsigned char std_plain[512] =  {
		0x42,0x83,0x1E,0xC2,0x21,0x77,0x74,0x24,0x4B,0x72,0x21,0xB7,0x84,0xD0,0xD4,0x9C,
		0xE3,0xAA,0x21,0x2F,0x2C,0x02,0xA4,0xE0,0x35,0xC1,0x7E,0x23,0x29,0xAC,0xA1,0x2E,
		0x21,0xD5,0x14,0xB2,0x54,0x66,0x93,0x1C,0x7D,0x8F,0x6A,0x5A,0xAC,0x84,0xAA,0x05,
		0x1B,0xA3,0x0B,0x39,0x6A,0x0A,0xAC,0x97,0x3D,0x58,0xE0,0x91,0x47,0x3F,0x59,0x85,
		0x69,0x79,0x2C,0x46,0x2E,0xF1,0x7B,0xB9,0xF4,0x81,0xE4,0x14,0x87,0xBB,0x37,0x48,
		0xE5,0xB9,0x25,0x90,0xBA,0xFF,0xD2,0x01,0x14,0x38,0xCF,0x6F,0x70,0x42,0x68,0xED,
		0xBE,0xD5,0x6D,0x21,0x83,0x62,0xB6,0xDB,0xEE,0x80,0x46,0xDF,0xEA,0xF3,0xCF,0x34,
		0xC2,0xDF,0xA2,0x51,0xB8,0x49,0xE7,0x2A,0xC9,0x80,0x75,0x72,0x53,0xAB,0xA6,0xF9,
		0x85,0x10,0x89,0x90,0x6E,0xFF,0x8A,0x37,0x19,0xDC,0x8C,0x7E,0x0C,0x78,0xC9,0xD4,
		0xFB,0x19,0xC2,0xEA,0xA7,0x3A,0xBA,0xD6,0xCF,0x41,0x49,0x42,0xF4,0x47,0x97,0x38,
		0x03,0xD0,0xE4,0xFC,0x45,0x76,0x34,0x33,0xCE,0xAC,0xC6,0xBB,0x75,0xCD,0xE8,0x15,
		0xF9,0xB3,0xAA,0xBE,0x1F,0x20,0x06,0xF7,0x84,0xFB,0x0A,0xD9,0x22,0x90,0x42,0xF6,
		0xA5,0x46,0x40,0x05,0x56,0x23,0xEA,0xF1,0x2F,0x2C,0x59,0xCE,0x8E,0xD9,0xA8,0x85,
		0xA8,0x93,0x54,0xBA,0x6F,0x1D,0x39,0x9F,0xE1,0x0F,0x69,0xDE,0x94,0xB6,0x13,0x5F,
		0x06,0x73,0x95,0x9E,0x8F,0xFC,0x47,0xE3,0x47,0x95,0x79,0x33,0x1C,0xF7,0x0C,0xE4,
		0x83,0x47,0x09,0x6B,0x67,0xF5,0x1E,0x4E,0x86,0x63,0x31,0x2E,0xCE,0xD5,0x38,0xEC,
		0x39,0x80,0xCA,0x0B,0x3C,0x00,0xE8,0x41,0xEB,0x06,0xFA,0xC4,0x87,0x2A,0x27,0x57,
		0x85,0x9E,0x1C,0xEA,0xA6,0xEF,0xD9,0x84,0x62,0x85,0x93,0xB4,0x0C,0xA1,0xE1,0x9C,
		0x7D,0x77,0x3D,0x00,0xC1,0x44,0xC5,0x25,0xAC,0x61,0x9D,0x18,0xC8,0x4A,0x3F,0x47,
		0x18,0xE2,0x44,0x8B,0x2F,0xE3,0x24,0xD9,0xCC,0xDA,0x27,0x10,0xAC,0xAD,0xE2,0x56,
		0x4E,0xEF,0x84,0xD7,0x8C,0x6B,0x94,0xD0,0x0A,0x1B,0xD2,0xA1,0xDB,0xB0,0xF1,0x5F,
		0x79,0xCF,0xE8,0x1D,0xD4,0x84,0x4E,0x34,0x5B,0x09,0xA5,0x34,0x1D,0x5C,0x2B,0xB1,
		0xF1,0xC2,0xF6,0xD9,0xBF,0x11,0x12,0xDF,0xCA,0x93,0xC3,0x2D,0x28,0x90,0xCB,0x8A,
		0xF5,0x93,0x61,0xE5,0x36,0x9A,0xB9,0xDA,0x39,0xC8,0xBC,0xA3,0x9A,0x71,0x30,0x37,
		0xAF,0xA3,0x30,0x5C,0x5B,0x0A,0x9D,0x80,0xAF,0x0D,0x99,0x1F,0x0F,0xB8,0x6A,0x21,
		0x85,0x4A,0x8E,0x78,0x08,0x04,0x32,0x2B,0xA3,0xEA,0xB9,0x22,0x8C,0xFE,0x70,0x21,
		0x9C,0x2E,0x08,0x48,0x6A,0xB5,0x9F,0x9D,0xE4,0xC8,0x87,0x55,0xD7,0xFD,0x85,0x23,
		0xDF,0x11,0xDA,0xCB,0x80,0xD4,0xE7,0x88,0x74,0x07,0x17,0x15,0x86,0x09,0x28,0xF2,
		0x14,0x17,0xFD,0x60,0xF3,0x03,0x88,0xE3,0x31,0x75,0xC7,0x9D,0x73,0xD9,0xEE,0x56,
		0x09,0x6F,0xE1,0xF8,0x13,0x14,0x51,0x6F,0x20,0xA4,0x6F,0x7D,0x19,0x3D,0xF9,0x24,
		0xF8,0x5C,0x97,0xE9,0xC5,0x1F,0x50,0xD5,0x6B,0x8B,0x82,0x0F,0x96,0xE1,0x2C,0x7E,
		0x78,0x14,0xA3,0x84,0x8D,0x81,0xDA,0x1C,0x1C,0xB5,0x55,0x29,0x15,0x6E,0x3E,0x8E};
static const unsigned char std_aad[65]    = {
		0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,
		0xab,0xad,0xda,0xd2,0x7C,0xA4,0x8E,0x82,0x99,0xFC,0x5A,0xD3,0xE9,0x08,0xB7,0x65,
		0x1B,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,0xef,0xfe,0xed,0xfa,0xce,0xde,0xad,0xbe,
		0xef,0xab,0xad,0xda,0xd2,0x7C,0xA4,0x8E,0x82,0x99,0xFC,0x5A,0xD3,0xE9,0x08,0xB7,
		0x65,};
static const unsigned char std_key[32]= {
		0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08,
		0xfe,0xff,0xe9,0x92,0x86,0x65,0x73,0x1c,0x6d,0x6a,0x8f,0x94,0x67,0x30,0x83,0x08};
static const unsigned char std_iv[8] = {
		0xca,0xfe,0xba,0xbe,0xfa,0xce,0xdb,0xad};

const unsigned char std_tag_c7_aad0_p256[]  = {0x23,0x6A,0xB3,0xE4,0xEC,0xCD,0xE1,0xF7,0x52,0x52,0xFF,0xF8,0x48,0xF2,0x94,0x0E};
const unsigned char std_tag_c7_aad3_p201[]  = {0x13,0x3A,0xB8,0x28,0x5D,0x98,0x8F,0x0D,0x3C,0xF3,0x50,0x0A,0xF9,0x77,0xA9,0xF3};
const unsigned char std_tag_c7_aad16_p120[] = {0x18,0xA7,0x28,0xA1,0x29,0xA2,0x1F,0xEE,0x52,0x78,0x7D,0x60,0x65,0xBA,0x0C,0x92};
const unsigned char std_tag_c7_aad17_p65[]  = {0xCF,0x02,0x1B,0x1D,0x13,0x72,0xAF,0x92,0x00,0xA4,0x9F,0xA4,0x50,0xE3,0x7F,0xB3};
const unsigned char std_tag_c7_aad20_p32[]  = {0x49,0xCD,0xA0,0x23,0x2C,0x13,0x32,0x23,0x93,0xD9,0x78,0x28,0x80,0x97,0x99,0x9B};
const unsigned char std_tag_c7_aad32_p15[]  = {0x93,0x2E,0x53,0x95,0xE8,0x14,0x3B,0x32,0x2F,0xC9,0x62,0xA6,0x9E,0x82,0x9D,0x74};
const unsigned char std_tag_c7_aad33_p0[]   = {0x34,0x35,0x75,0x4D,0x47,0x9B,0xA5,0xDD,0x7F,0x13,0x42,0xFA,0x1F,0x7F,0xFE,0x36};
const unsigned char std_tag_c7_aad65_p256[] = {0x03,0xBF,0x09,0xF1,0x06,0xF4,0x91,0xE7,0x82,0xCA,0x26,0x84,0xA0,0x0C,0xB6,0x69};
const unsigned char std_tag_c7_aad50_p512[] = {0xD1,0x08,0xEE,0xF4,0x86,0x39,0xB8,0xC1,0x50,0xA2,0x10,0xFF,0x73,0x2B,0x2D,0x1C};


unsigned int chacha20_poly1305_test_internal(unsigned char wordAlign, unsigned char *std_plain__, unsigned int byteLen, unsigned char *key,
		unsigned char *iv, unsigned int constant, unsigned char *aad, unsigned char aadByteLen, unsigned char *std_cipher__, unsigned char *std_tag)
{
	unsigned char key_buf[32+4];
	unsigned char iv_buf[8+4];
	unsigned char aad_buf[65+4];
	unsigned char std_plain_buf[512+4];
	unsigned char std_cipher_buf[512+4];
	unsigned char std_tag_buf[16+4];
	unsigned char cipher_buf[512+4];
	unsigned char replain_buf[512+4];
	unsigned char tag_buf[16+4];
	unsigned char *cipher_, *replain_, *tag_, *std_plain_, *std_cipher_, *std_tag_, *key_, *iv_, *aad_;
	unsigned int i,j;
	unsigned int ret;
	unsigned int round = byteLen / 64;
	unsigned int remainder = byteLen & 63;

	CHACHA20_POLY1305_CTX ctx[1];

	printf("\r\nchacha20_poly1305 test(AAD=%u bytes,P/C=%u bytes)", aadByteLen, byteLen);

	if(0 == remainder && 0 < round)
	{
		round--;
		remainder = 64;
	}

	if(wordAlign)
	{
		cipher_     = cipher_buf;
		replain_    = replain_buf;
		tag_        = tag_buf;
		std_plain_  = std_plain_buf;
		std_cipher_ = std_cipher_buf;
		std_tag_    = std_tag_buf;
		key_	    = key_buf;
		iv_         = iv_buf;
		aad_        = aad_buf;
	}
	else
	{
		cipher_     = cipher_buf + 1;
		replain_    = replain_buf + 1;
		tag_        = tag_buf + 1;
		std_plain_  = std_plain_buf + 1;
		std_cipher_ = std_cipher_buf + 1;
		std_tag_    = std_tag_buf + 1;
		key_	    = key_buf + 1;
		iv_         = iv_buf + 1;
		aad_        = aad_buf + 1;
	}
	
	memcpy_(std_plain_, std_plain__, byteLen);
	memcpy_(std_cipher_, std_cipher__, byteLen);
	memcpy_(std_tag_, std_tag, 16);
	memcpy_(key_, key, 32);
	memcpy_(iv_, iv, 8);
	memcpy_(aad_, aad, aadByteLen);

	/**************** test 1: one-time style ******************/
#if 0
#else
	//ENCRYPT
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key_, constant, iv_, aad_, aadByteLen, std_plain_, cipher_, byteLen, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n encrypt 1 error, ret=%u", ret);
		return 1;
	}

	//DECRYPT
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, key_, constant, iv_, aad_, aadByteLen, (unsigned char*)std_cipher, replain_, byteLen, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n decrypt 1 error, ret=%u", ret);
		return 1;
	}
#endif
	if(memcmp_(tag_, std_tag_, 16) || memcmp_(cipher_, std_cipher_, byteLen) || memcmp_(replain_, std_plain_, byteLen))
	{
		printf("\r\n one time input test failure!!");
		print_buf_U8(std_plain_, byteLen, "std_plain");
		print_buf_U8(std_tag_, 16, "std_tag");
		print_buf_U8(std_cipher_, byteLen, "std_cipher");
		print_buf_U8(key_, 32, "key");
		print_buf_U8(iv_, 8, "iv");
		print_buf_U8(aad_, aadByteLen, "aad");
		print_buf_U8(cipher_, byteLen, "cipher");
		print_buf_U8(tag_, 16, "tag");
		print_buf_U8(replain_, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n one time input test success!!");
	}


	/**************** test 2: multiple style(one block every time) ******************/
	memset_(cipher_, 0, byteLen);
	memset_(replain_, 0, byteLen);
	memset_(tag_, 0, 16);

	//ENCRYPT
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, aad, aadByteLen);
	for(i=0; i<round; i++)
	{
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_plain_ + i*64, cipher_ + i*64, 64);
	}
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_plain_ + round*64, cipher_ + round*64, remainder, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 enc test 2 error");
		return 1;
	}
		
	//ENCRYPT
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, aad, aadByteLen);
	for(i=0; i<round; i++)
	{
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher_ + i*64, replain_ + i*64, 64);
	}
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_cipher_ + round*64, replain_ + round*64, remainder, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dec test 2 error");
		return 1;
	}

	if(memcmp_(tag_, std_tag_, 16) || memcmp_(cipher_, std_cipher_, byteLen) || memcmp_(replain_, std_plain_, byteLen))
	{
		printf("\r\n multiple input test failure(one block every time)!!");
		print_buf_U8(std_plain_, byteLen, "std_plain");
		print_buf_U8(std_tag_, 16, "std_tag");
		print_buf_U8(std_cipher_, byteLen, "std_cipher");
		print_buf_U8(cipher_, byteLen, "cipher");
		print_buf_U8(tag_, 16, "tag");
		print_buf_U8(replain_, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n multiple input test success(one block every time)!!");
	}


	/**************** test 3: multiple style(random blocks every time) ******************/
	memset_(cipher_, 0, byteLen);
	memset_(replain_, 0, byteLen);
	memset_(tag_, 0, 16);

	//ENCRYPT
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, aad, aadByteLen);

	i = 0;
	while(i < round * 64)
	{
		j = rand() % ((round*64-i)/64 + 1);
		j = j*64;
		
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_plain_ + i, cipher_ + i, j);
		i += j;
	}
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_plain_ + i, cipher_ + i, remainder, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 enc test 3 error");
		return 1;
	}

	//DECRYPT
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, aad, aadByteLen);

	i = 0;
	while(i < round * 64)
	{
		j = rand() % ((round*64-i)/64 + 1);
		j = j*64;
		
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher_ + i, replain_ + i, j);
		i += j;
	}
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_cipher_ + i, replain_ + i, remainder, tag_);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dec test 3 error");
		return 1;
	}
	
	if(memcmp_(tag_, std_tag_, 16) || memcmp_(cipher_, std_cipher_, byteLen) || memcmp_(replain_, std_plain_, byteLen))
	{
		printf("\r\n multiple input test failure(random blocks every time)!!");
		print_buf_U8(std_plain_, byteLen, "std_plain");
		print_buf_U8(std_tag_, 16, "std_tag");
		print_buf_U8(std_cipher_, byteLen, "std_cipher");
		print_buf_U8(cipher_, byteLen, "cipher");
		print_buf_U8(tag_, 16, "tag");
		print_buf_U8(replain_, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n multiple input test success(random blocks every time)!!");
	}
	return 0;
}


unsigned int chacha20_poly1305_dma_test_internal(unsigned char *std_plain__, unsigned int byteLen, unsigned char *key,
		unsigned char *iv, unsigned int constant, unsigned char *aad, unsigned char aadByteLen, unsigned char *std_cipher__, unsigned char *std_tag)
{
	unsigned int i, j;
	unsigned int ret;
	unsigned int round = byteLen / 64;
	unsigned int remainder = byteLen & 63;
	
	unsigned int *std_aad_buf     = (unsigned int *)(DMA_RAM_BASE);
	unsigned int *std_plain_buf   = std_aad_buf+0x200;
	unsigned int *std_cipher_buf  = std_plain_buf+0x200;
	unsigned int *replain_buf  = std_cipher_buf+0x200;
	unsigned int *cipher_buf = replain_buf+0x200;
	unsigned int *tag_buf = cipher_buf+0x200;
	
	CHACHA20_POLY1305_DMA_CTX ctx[1];

	printf("\r\nchacha20_poly1305 dma test(AAD=%u bytes,P/C=%u bytes)", aadByteLen, byteLen);

	memcpy_(std_plain_buf, std_plain__, byteLen);
	memcpy_(std_cipher_buf, std_cipher__, byteLen);
	memcpy_(std_aad_buf, aad, aadByteLen);

	/**************** test 1: one-time style ******************/
#if 0
#else
	//ENCRYPT
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, std_aad_buf, aadByteLen, std_plain_buf, cipher_buf, byteLen, (unsigned char *)tag_buf);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n encrypt 1 error, ret=%u", ret);
		return 1;
	}

	//DECRYPT
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, std_aad_buf, aadByteLen, std_cipher_buf, replain_buf, byteLen, (unsigned char *)tag_buf);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n decrypt 1 error, ret=%u", ret);
		return 1;
	}
#endif
	if(memcmp_(tag_buf, std_tag, 16) || memcmp_(cipher_buf, std_cipher_buf, byteLen) || memcmp_(replain_buf, std_plain_buf, byteLen))
	{
		printf("\r\n one time input test failure!!");
		print_buf_U8((unsigned char *)std_plain_buf, byteLen, "std_plain");
		print_buf_U8(std_tag, 16, "std_tag");
		print_buf_U8((unsigned char *)std_cipher_buf, byteLen, "std_cipher");
		print_buf_U8(key, 32, "key");
		print_buf_U8(iv, 8, "iv");
		print_buf_U8((unsigned char *)std_aad_buf, aadByteLen, "aad");
		print_buf_U8((unsigned char *)cipher_buf, byteLen, "cipher");
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		print_buf_U8((unsigned char *)replain_buf, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n one time input test success!!");
	}


	/**************** test 2: multiple style(one block every time) ******************/
	memset_(cipher_buf, 0, byteLen);
	memset_(replain_buf, 0, byteLen);
	memset_(tag_buf, 0, 16);

	if(0 == remainder && 0 != byteLen)
	{
		round--;
		remainder = 64;
	}

	//ENCRYPT
	ret = chacha20_poly1305_dma_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, (unsigned int *)std_aad_buf, aadByteLen);
	for(i=0; i<round; i++)
	{
		ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx, std_plain_buf + i*16, cipher_buf + i*16, 16);
	}
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx, std_plain_buf + round*16, cipher_buf + round*16, remainder, (unsigned char *)tag_buf);	
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dma enc test 2 error");
		return 1;
	}
		
	//ENCRYPT
	ret = chacha20_poly1305_dma_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, (unsigned int *)std_aad_buf, aadByteLen);
	for(i=0; i<round; i++)
	{
		ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx, std_cipher_buf + i*16, replain_buf + i*16, 16);
	}
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx, std_cipher_buf + round*16, replain_buf + round*16, remainder, (unsigned char *)tag_buf);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dma dec test 2 error");
		return 1;
	}

	if(memcmp_(tag_buf, std_tag, 16) || memcmp_(cipher_buf, std_cipher_buf, byteLen) || memcmp_(replain_buf, std_plain_buf, byteLen))
	{
		printf("\r\n multiple input test failure(one block every time)!!");
		print_buf_U8((unsigned char *)std_plain_buf, byteLen, "std_plain");
		print_buf_U8(std_tag, 16, "std_tag");
		print_buf_U8((unsigned char *)std_cipher_buf, byteLen, "std_cipher");
		print_buf_U8((unsigned char *)cipher_buf, byteLen, "cipher");
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		print_buf_U8((unsigned char *)replain_buf, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n multiple input test success(one block every time)!!");
	}


	/**************** test 3: multiple style(random blocks every time) ******************/
	memset_(cipher_buf, 0, byteLen);
	memset_(replain_buf, 0, byteLen);
	memset_(tag_buf, 0, 16);

	//ENCRYPT
	ret = chacha20_poly1305_dma_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, (unsigned int *)std_aad_buf, aadByteLen);

	i = 0;
	while(i < round * 64)
	{
		j = rand() % ((round*64-i)/64 + 1);
		j = j*64;
		ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx, std_plain_buf + i/4, cipher_buf + i/4, j/4);
		i += j;
	}
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx, std_plain_buf + i/4, cipher_buf + i/4, remainder, (unsigned char *)tag_buf);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dma enc test 3 error");
		return 1;
	}

	//DECRYPT
	ret = chacha20_poly1305_dma_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, (unsigned int *)std_aad_buf, aadByteLen);

	i = 0;
	while(i < round * 64)
	{
		j = rand() % ((round*64-i)/64 + 1);
		j = j*64;
		ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx, std_cipher_buf + i/4, replain_buf + i/4, j/4);
		i += j;
	}
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx, std_cipher_buf + i/4, replain_buf + i/4, remainder, (unsigned char *)tag_buf);
	if(CHACHA20_POLY1305_SUCCESS != ret)
	{
		printf("\r\n chacha20_poly1305 dma dec test 3 error");
		return 1;
	}
	
	if(memcmp_(tag_buf, std_tag, 16) || memcmp_(cipher_buf, std_cipher_buf, byteLen) || memcmp_(replain_buf, std_plain_buf, byteLen))
	{
		printf("\r\n multiple input test failure(random blocks every time)!!");
		print_buf_U8((unsigned char *)std_plain_buf, byteLen, "std_plain");
		print_buf_U8(std_tag, 16, "std_tag");
		print_buf_U8((unsigned char *)std_cipher_buf, byteLen, "std_cipher");
		print_buf_U8((unsigned char *)cipher_buf, byteLen, "cipher");
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		print_buf_U8((unsigned char *)replain_buf, byteLen, "replain");
		return 1;
	}
	else
	{
		printf("\r\n multiple input test success(random blocks every time)!!");
	}
	
	return 0;
}


unsigned int chacha20_poly1305_standard_data_test()
{
	unsigned int i;
	unsigned int ret;
	
	CHACHA20_POLY1305_TEST_VECTOR vector[9] = {
		{std_plain, 256, std_key, std_iv, 7, std_aad, 0,  std_cipher, std_tag_c7_aad0_p256},
		{std_plain, 201, std_key, std_iv, 7, std_aad, 3,  std_cipher, std_tag_c7_aad3_p201},
		{std_plain, 120, std_key, std_iv, 7, std_aad, 16, std_cipher, std_tag_c7_aad16_p120},
		{std_plain, 65,  std_key, std_iv, 7, std_aad, 17, std_cipher, std_tag_c7_aad17_p65},
		{std_plain, 32,  std_key, std_iv, 7, std_aad, 20, std_cipher, std_tag_c7_aad20_p32},
		{std_plain, 15,  std_key, std_iv, 7, std_aad, 32, std_cipher, std_tag_c7_aad32_p15},
		{std_plain, 0,   std_key, std_iv, 7, std_aad, 33, std_cipher, std_tag_c7_aad33_p0},
		{std_plain, 256, std_key, std_iv, 7, std_aad, 65, std_cipher, std_tag_c7_aad65_p256},
		{std_plain, 512, std_key, std_iv, 7, std_aad, 50, std_cipher, std_tag_c7_aad50_p512},
	};

	printf("\r\n\r\n  =================== chacha20_poly1305 standard_data test ==================== ");

	for(i=0; i<9; i++)
	{
		ret = chacha20_poly1305_test_internal(1, (unsigned char *)vector[i].std_plain, vector[i].c_bytes, (unsigned char *)vector[i].key,
				(unsigned char *)vector[i].iv, vector[i].constant, (unsigned char *)vector[i].aad, vector[i].aad_bytes, (unsigned char *)vector[i].std_cipher,
				(unsigned char *)vector[i].std_tag);
		if(ret)
			return 1;

		ret = chacha20_poly1305_test_internal(0, (unsigned char *)vector[i].std_plain, vector[i].c_bytes, (unsigned char *)vector[i].key,
				(unsigned char *)vector[i].iv, vector[i].constant, (unsigned char *)vector[i].aad, vector[i].aad_bytes, (unsigned char *)vector[i].std_cipher,
				(unsigned char *)vector[i].std_tag);
		if(ret)
			return 1;

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
		ret = chacha20_poly1305_dma_test_internal((unsigned char *)vector[i].std_plain, vector[i].c_bytes, (unsigned char *)vector[i].key,
				(unsigned char *)vector[i].iv, vector[i].constant, (unsigned char *)vector[i].aad, vector[i].aad_bytes, (unsigned char *)vector[i].std_cipher,
				(unsigned char *)vector[i].std_tag);
		if(ret)
			return 1;
#endif
	}

	return 0;
}


unsigned int chacha20_poly1305_sample_test()
{	
	unsigned int ret;
	unsigned char tmp_out[265] = {0};
	unsigned char tmp_tag[16] = {0};
	unsigned char tmp_out2[265] = {0};
	unsigned char tmp_tag2[16] = {0};
	
	//ChaCha20-Poly1305 AEAD Decryption
	unsigned char std_key1[32] = {
		0x1c,0x92,0x40,0xa5,0xeb,0x55,0xd3,0x8a,0xf3,0x33,0x88,0x86,0x04,0xf6,0xb5,0xf0,
		0x47,0x39,0x17,0xc1,0x40,0x2b,0x80,0x09,0x9d,0xca,0x5c,0xbc,0x20,0x70,0x75,0xc0 };
	unsigned char std_cipher1[265] = {
		0x64,0xa0,0x86,0x15,0x75,0x86,0x1a,0xf4,0x60,0xf0,0x62,0xc7,0x9b,0xe6,0x43,0xbd,
		0x5e,0x80,0x5c,0xfd,0x34,0x5c,0xf3,0x89,0xf1,0x08,0x67,0x0a,0xc7,0x6c,0x8c,0xb2,
		0x4c,0x6c,0xfc,0x18,0x75,0x5d,0x43,0xee,0xa0,0x9e,0xe9,0x4e,0x38,0x2d,0x26,0xb0,
		0xbd,0xb7,0xb7,0x3c,0x32,0x1b,0x01,0x00,0xd4,0xf0,0x3b,0x7f,0x35,0x58,0x94,0xcf,
		0x33,0x2f,0x83,0x0e,0x71,0x0b,0x97,0xce,0x98,0xc8,0xa8,0x4a,0xbd,0x0b,0x94,0x81,
		0x14,0xad,0x17,0x6e,0x00,0x8d,0x33,0xbd,0x60,0xf9,0x82,0xb1,0xff,0x37,0xc8,0x55,
		0x97,0x97,0xa0,0x6e,0xf4,0xf0,0xef,0x61,0xc1,0x86,0x32,0x4e,0x2b,0x35,0x06,0x38,
		0x36,0x06,0x90,0x7b,0x6a,0x7c,0x02,0xb0,0xf9,0xf6,0x15,0x7b,0x53,0xc8,0x67,0xe4,
		0xb9,0x16,0x6c,0x76,0x7b,0x80,0x4d,0x46,0xa5,0x9b,0x52,0x16,0xcd,0xe7,0xa4,0xe9,
		0x90,0x40,0xc5,0xa4,0x04,0x33,0x22,0x5e,0xe2,0x82,0xa1,0xb0,0xa0,0x6c,0x52,0x3e,
		0xaf,0x45,0x34,0xd7,0xf8,0x3f,0xa1,0x15,0x5b,0x00,0x47,0x71,0x8c,0xbc,0x54,0x6a,
		0x0d,0x07,0x2b,0x04,0xb3,0x56,0x4e,0xea,0x1b,0x42,0x22,0x73,0xf5,0x48,0x27,0x1a,
		0x0b,0xb2,0x31,0x60,0x53,0xfa,0x76,0x99,0x19,0x55,0xeb,0xd6,0x31,0x59,0x43,0x4e,
		0xce,0xbb,0x4e,0x46,0x6d,0xae,0x5a,0x10,0x73,0xa6,0x72,0x76,0x27,0x09,0x7a,0x10,
		0x49,0xe6,0x17,0xd9,0x1d,0x36,0x10,0x94,0xfa,0x68,0xf0,0xff,0x77,0x98,0x71,0x30,
		0x30,0x5b,0xea,0xba,0x2e,0xda,0x04,0xdf,0x99,0x7b,0x71,0x4d,0x6c,0x6f,0x2c,0x29,
		0xa6,0xad,0x5c,0xb4,0x02,0x2b,0x02,0x70,0x9b };
	unsigned char std_plain1[265] = {
		0x49,0x6e,0x74,0x65,0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x73,0x20,
		0x61,0x72,0x65,0x20,0x64,0x72,0x61,0x66,0x74,0x20,0x64,0x6f,0x63,0x75,0x6d,0x65,
		0x6e,0x74,0x73,0x20,0x76,0x61,0x6c,0x69,0x64,0x20,0x66,0x6f,0x72,0x20,0x61,0x20,
		0x6d,0x61,0x78,0x69,0x6d,0x75,0x6d,0x20,0x6f,0x66,0x20,0x73,0x69,0x78,0x20,0x6d,
		0x6f,0x6e,0x74,0x68,0x73,0x20,0x61,0x6e,0x64,0x20,0x6d,0x61,0x79,0x20,0x62,0x65,
		0x20,0x75,0x70,0x64,0x61,0x74,0x65,0x64,0x2c,0x20,0x72,0x65,0x70,0x6c,0x61,0x63,
		0x65,0x64,0x2c,0x20,0x6f,0x72,0x20,0x6f,0x62,0x73,0x6f,0x6c,0x65,0x74,0x65,0x64,
		0x20,0x62,0x79,0x20,0x6f,0x74,0x68,0x65,0x72,0x20,0x64,0x6f,0x63,0x75,0x6d,0x65,
		0x6e,0x74,0x73,0x20,0x61,0x74,0x20,0x61,0x6e,0x79,0x20,0x74,0x69,0x6d,0x65,0x2e,
		0x20,0x49,0x74,0x20,0x69,0x73,0x20,0x69,0x6e,0x61,0x70,0x70,0x72,0x6f,0x70,0x72,
		0x69,0x61,0x74,0x65,0x20,0x74,0x6f,0x20,0x75,0x73,0x65,0x20,0x49,0x6e,0x74,0x65,
		0x72,0x6e,0x65,0x74,0x2d,0x44,0x72,0x61,0x66,0x74,0x73,0x20,0x61,0x73,0x20,0x72,
		0x65,0x66,0x65,0x72,0x65,0x6e,0x63,0x65,0x20,0x6d,0x61,0x74,0x65,0x72,0x69,0x61,
		0x6c,0x20,0x6f,0x72,0x20,0x74,0x6f,0x20,0x63,0x69,0x74,0x65,0x20,0x74,0x68,0x65,
		0x6d,0x20,0x6f,0x74,0x68,0x65,0x72,0x20,0x74,0x68,0x61,0x6e,0x20,0x61,0x73,0x20,
		0x2f,0xe2,0x80,0x9c,0x77,0x6f,0x72,0x6b,0x20,0x69,0x6e,0x20,0x70,0x72,0x6f,0x67,
		0x72,0x65,0x73,0x73,0x2e,0x2f,0xe2,0x80,0x9d };
	unsigned char std_tag1[] = {
		0xee,0xad,0x9d,0x67,0x89,0x0c,0xbb,0x22,0x39,0x23,0x36,0xfe,0xa1,0x85,0x1f,0x38 };
	unsigned char std_aad1[12] = { 
		0xf3,0x33,0x88,0x86,0x00,0x00,0x00,0x00,0x00,0x00,0x4e,0x91 };
	unsigned char std_iv1[8] = {
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	unsigned int std_constant1 = 0;
		

	//ChaCha20-Poly1305 AEAD Encryption
	unsigned char std_key2[32] = {
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, };
	unsigned char std_plain2[114] = {
		0x4C,0x61,0x64,0x69,0x65,0x73,0x20,0x61,0x6E,0x64,0x20,0x47,0x65,0x6E,0x74,0x6C,
		0x65,0x6D,0x65,0x6E,0x20,0x6F,0x66,0x20,0x74,0x68,0x65,0x20,0x63,0x6C,0x61,0x73,
		0x73,0x20,0x6F,0x66,0x20,0x27,0x39,0x39,0x3A,0x20,0x49,0x66,0x20,0x49,0x20,0x63,
		0x6F,0x75,0x6C,0x64,0x20,0x6F,0x66,0x66,0x65,0x72,0x20,0x79,0x6F,0x75,0x20,0x6F,
		0x6E,0x6C,0x79,0x20,0x6F,0x6E,0x65,0x20,0x74,0x69,0x70,0x20,0x66,0x6F,0x72,0x20,
		0x74,0x68,0x65,0x20,0x66,0x75,0x74,0x75,0x72,0x65,0x2C,0x20,0x73,0x75,0x6E,0x73,
		0x63,0x72,0x65,0x65,0x6E,0x20,0x77,0x6F,0x75,0x6C,0x64,0x20,0x62,0x65,0x20,0x69,
		0x74,0x2E };
	unsigned char std_cipher2[114] = {
		0xD3,0x1A,0x8D,0x34,0x64,0x8E,0x60,0xDB,0x7B,0x86,0xAF,0xBC,0x53,0xEF,0x7E,0xC2,
		0xA4,0xAD,0xED,0x51,0x29,0x6E,0x08,0xFE,0xA9,0xE2,0xB5,0xA7,0x36,0xEE,0x62,0xD6,
		0x3D,0xBE,0xA4,0x5E,0x8C,0xA9,0x67,0x12,0x82,0xFA,0xFB,0x69,0xDA,0x92,0x72,0x8B,
		0x1A,0x71,0xDE,0x0A,0x9E,0x06,0x0B,0x29,0x05,0xD6,0xA5,0xB6,0x7E,0xCD,0x3B,0x36,
		0x92,0xDD,0xBD,0x7F,0x2D,0x77,0x8B,0x8C,0x98,0x03,0xAE,0xE3,0x28,0x09,0x1B,0x58,
		0xFA,0xB3,0x24,0xE4,0xFA,0xD6,0x75,0x94,0x55,0x85,0x80,0x8B,0x48,0x31,0xD7,0xBC,
		0x3F,0xF4,0xDE,0xF0,0x8E,0x4B,0x7A,0x9D,0xE5,0x76,0xD2,0x65,0x86,0xCE,0xC6,0x4B,
		0x61,0x16 };
	unsigned char std_tag2[] = {
		0x1A,0xE1,0x0B,0x59,0x4F,0x09,0xE2,0x6A,0x7E,0x90,0x2E,0xCB,0xD0,0x60,0x06,0x91 };
	unsigned char std_aad2[] = {
		0x50,0x51,0x52,0x53,0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7 };
	unsigned char std_iv2[8] = {
		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47};
	unsigned int std_constant2 = 7;

	CHACHA20_POLY1305_CTX ctx[2];
	CHACHA20_POLY1305_DMA_CTX ctx2[2];

	printf("\r\n\r\n =================== CHACHA20_POLY1305 simple test (from RFC8439) =================== \r\n");

#if 0
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, std_aad1, 12, std_cipher1, tmp_out, 265, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, std_aad1, 12);
#if 1
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher1, tmp_out, 64);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher1+1*64, tmp_out+1*64, 128);
#else
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher1, tmp_msg, 3*64);
#endif
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_cipher1+3*64, tmp_out+3*64, 73, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_plain1, tmp_out, 265) || memcmp_(std_tag1, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 test 1 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_out, 265, "payload_out");
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 test 1 success\r\n");
	}

#if 0
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, std_aad2, 12, std_plain2, tmp_out, 114, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, std_aad2, 12);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_plain2, tmp_out, 64);
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_plain2+64, tmp_out+64, 50, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_cipher2, tmp_out, 114) || memcmp_(std_tag2, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 test 2 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_out, 114, "payload_out");
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 test 2 success\r\n");
	}


#ifdef CHACHA20_POLY1305_DMA_FUNCTION
	/************** DMA style ***************/
	unsigned int *aad_buf = (unsigned int *)DMA_RAM_BASE;
	unsigned int *payload_buf = aad_buf + 0x100;
	unsigned int *out_buf = payload_buf + 0x200;
	unsigned int *aad2_buf = out_buf + 0x300;
	unsigned int *payload2_buf = aad2_buf + 0x400;
	unsigned int *out2_buf = payload2_buf + 0x500;

	memcpy_(aad_buf, std_aad1, 12);
	memcpy_(payload_buf, std_cipher1, 265);
	
#if 0
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, aad_buf, 12, payload_buf, out_buf, 265, (unsigned int *)tmp_tag);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, aad_buf, 12);
#if 0
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf, out_buf, 16);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf+16, out_buf+16, 16);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf+2*16, out_buf+2*16, 16);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf+3*16, out_buf+3*16, 16);
#else
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf, out_buf, 4*16);
#endif
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, payload_buf+4*16, out_buf+4*16, 9, (unsigned char *)tmp_tag);	
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(tmp_tag, std_tag1, 16) || memcmp_(out_buf, std_plain1, 265))
	{
		printf("\r\n generate chacha20_poly1305 dma test 1 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)out_buf, 265, "payload_out");
		print_buf_U8((unsigned char *)tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma test 1 success\r\n");
	}

	memcpy_(aad_buf, std_aad2, 12);
	memcpy_(payload_buf, std_plain2, 114);

#if 0
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, aad_buf, 12, payload_buf, out_buf, 114, (unsigned int *)tmp_tag);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, aad_buf, 12);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf, out_buf, 1*16);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, payload_buf+1*16, out_buf+1*16, 50, (unsigned char *)tmp_tag);	
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(tmp_tag, std_tag2, 16) || memcmp_(out_buf, std_cipher2, 114))
	{
		printf("\r\n generate chacha20_poly1305 dma test 2 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)out_buf, 114, "payload_out");
		print_buf_U8((unsigned char *)tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma test 2 success\r\n");
	}
#endif

#ifdef CONFIG_CHACHA20_POLY1305_SUPPORT_MUL_THREAD
	/*data interlace test CPU*/
	ret = chacha20_poly1305_init(&ctx[0], CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, std_aad1, 12);
	ret |= chacha20_poly1305_init(&ctx[1], CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, std_aad2, 12);

	ret |= chacha20_poly1305_update_excluding_last_data(&ctx[0], std_cipher1, tmp_out, 256);
	ret |= chacha20_poly1305_update_excluding_last_data(&ctx[1], std_plain2, tmp_out2, 64);
	ret |= chacha20_poly1305_update_including_last_data(&ctx[0], std_cipher1+256, tmp_out+256, 9, tmp_tag);
	ret |= chacha20_poly1305_update_including_last_data(&ctx[1], std_plain2+64, tmp_out2+64, 50, tmp_tag2);

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(tmp_tag, std_tag1, 16) || memcmp_(tmp_tag2, std_tag2, 16)
		|| memcmp_(tmp_out, std_plain1, 265) || memcmp_(tmp_out2, std_cipher2, 114))
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 1 failure, ret=%x \r\n", ret);
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 1 success\r\n");
	}

	/*data interlace test DMA*/
	memcpy_(aad_buf, std_aad1, 12);
	memcpy_(payload_buf, std_cipher1, 265);
	memcpy_(aad2_buf, std_aad2, 12);
	memcpy_(payload2_buf, std_plain2, 114);

	memset_(tmp_tag, 0, 16);
	memset_(tmp_tag2, 0, 16);
	
	ret = chacha20_poly1305_dma_init(&ctx2[0], CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, aad_buf, 12);
	ret |= chacha20_poly1305_dma_init(&ctx2[1], CHACHA20_POLY1305_CRYPTO_ENCRYPT, std_key2, std_constant2, std_iv2, aad2_buf, 12);

	ret |= chacha20_poly1305_dma_update_excluding_last_data(&ctx2[0], (unsigned int *)std_cipher1, out_buf, 256/4);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(&ctx2[1], (unsigned int *)std_plain2, out2_buf, 64/4);
	ret |= chacha20_poly1305_dma_update_including_last_data(&ctx2[0], (unsigned int *)std_cipher1+256/4, out_buf+256/4, 9, tmp_tag);
	ret |= chacha20_poly1305_dma_update_including_last_data(&ctx2[1], (unsigned int *)std_plain2+64/4, out2_buf+64/4, 50, tmp_tag2);

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(tmp_tag, std_tag1, 16) || memcmp_(tmp_tag2, std_tag2, 16)
		|| memcmp_(out_buf, std_plain1, 265) || memcmp_(out2_buf, std_cipher2, 114))
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 2 failure, ret=%x \r\n", ret);
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 2 success\r\n");
	}

	/*data interlace test CPU and DMA*/
	memset_(tmp_tag, 0, 16);
	memset_(tmp_tag2, 0, 16);
	memset_(tmp_out, 0, 265);
	memset_(out_buf, 0, 265);

	ret = chacha20_poly1305_init(&ctx[0], CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, std_aad1, 12);
	ret |= chacha20_poly1305_dma_init(&ctx2[0], CHACHA20_POLY1305_CRYPTO_DECRYPT, std_key1, std_constant1, std_iv1, aad_buf, 12);

	ret |= chacha20_poly1305_dma_update_excluding_last_data(&ctx2[0], (unsigned int *)std_cipher1, out_buf, 256/4);
	ret |= chacha20_poly1305_update_excluding_last_data(&ctx[0], std_cipher1, tmp_out, 256);
	ret |= chacha20_poly1305_dma_update_including_last_data(&ctx2[0], (unsigned int *)std_cipher1+256/4, out_buf+256/4, 9, tmp_tag);
	ret |= chacha20_poly1305_update_including_last_data(&ctx[0], std_cipher1+256, tmp_out+256, 9, tmp_tag2);

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(tmp_tag, std_tag1, 16) || memcmp_(tmp_tag2, std_tag1, 16)
		|| memcmp_(out_buf, std_plain1, 265) || memcmp_(tmp_out, std_plain1, 265))
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 3 failure, ret=%x \r\n", ret);
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 data interlace test 3 success\r\n");
	}
#endif

	return CHACHA20_POLY1305_SUCCESS;
}


unsigned int chacha20_poly1305_corner_test()
{
	//aad null, payload not null
	unsigned char std_cipher1[100] = {
		0x9F,0x07,0xE7,0xBE,0x55,0x51,0x38,0x7A,0x98,0xBA,0x97,0x7C,0x73,0x2D,0x08,0x0D,
		0xCB,0x0F,0x29,0xA0,0x48,0xE3,0x65,0x69,0x12,0xC6,0x53,0x3E,0x32,0xEE,0x7A,0xED,
		0x29,0xB7,0x21,0x76,0x9C,0xE6,0x4E,0x43,0xD5,0x71,0x33,0xB0,0x74,0xD8,0x39,0xD5,
		0x31,0xED,0x1F,0x28,0x51,0x0A,0xFB,0x45,0xAC,0xE1,0x0A,0x1F,0x4B,0x79,0x4D,0x6F,
		0x2D,0x09,0xA0,0xE6,0x63,0x26,0x6C,0xE1,0xAE,0x7E,0xD1,0x08,0x19,0x68,0xA0,0x75,
		0x8E,0x71,0x8E,0x99,0x7B,0xD3,0x62,0xC6,0xB0,0xC3,0x46,0x34,0xA9,0xA0,0xB3,0x5D,
		0x01,0x27,0x37,0x68 };
	unsigned char std_tag1[16] = {
		0xE0,0x1A,0x70,0x68,0xCC,0xA1,0xB1,0xB3,0x08,0xF5,0x83,0xA9,0xFA,0xA9,0xEB,0x34 };
	//aad not null, payload null
	unsigned char std_tag2[16] = {
		0x12,0x3F,0x86,0x99,0x59,0x29,0xE4,0x64,0xE2,0x41,0x8F,0x46,0x3C,0xE4,0x4D,0xD9 };
	//aad null, payload null
	unsigned char std_tag3[16] = {
		0x4E,0xB9,0x72,0xC9,0xA8,0xFB,0x3A,0x1B,0x38,0x2B,0xB4,0xD3,0x6F,0x5F,0xFA,0xD1 };

	unsigned char key[32] = {0};
	unsigned char iv[8] = {0};
	unsigned char aad[100] = {0};
	unsigned char plain[100] = {0};
	unsigned int constant = 0;

	unsigned char tmp_tag[16];
	unsigned char tmp_out[100];
	unsigned int ret;
	
	CHACHA20_POLY1305_CTX ctx[1];
	CHACHA20_POLY1305_DMA_CTX ctx2[1];

	/************** DMA style ***************/
	unsigned int *aad_buf = (unsigned int *)DMA_RAM_BASE;
	unsigned int *payload_buf = aad_buf + 0x400;
	unsigned int *out_buf = payload_buf + 0x800;
	unsigned int *tag_buf = payload_buf + 0xC00;

	memcpy_(aad_buf, aad, 100);

	printf("\r\n\r\n =================== CHACHA20_POLY1305 corner test =================== \r\n");
	
	//1. aad null, payload not null 
#if 0
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0, plain, tmp_out, 100, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, plain, tmp_out, 64);
	ret |= chacha20_poly1305_update_including_last_data(ctx, plain+64, tmp_out+64, 36, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_cipher1, tmp_out, 100) || memcmp_(std_tag1, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload not null) 1 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_out, 100, "payload_out");
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload not null) 1 success\r\n");
	}

	tmp_tag[0] = 0;
#if 0
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0, std_cipher1, tmp_out, 100, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher1, tmp_out, 64);
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_cipher1+64, tmp_out+64, 36, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(plain, tmp_out, 100) || memcmp_(std_tag1, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 dec corner test(aad null, payload not null) 2 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_out, 100, "payload_out");
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dec corner test(aad null, payload not null) 2 success\r\n");
	}

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
	memcpy_(payload_buf, plain, 100);
#if 1
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0, payload_buf, out_buf, 100, (unsigned char *)tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf, out_buf, 1*16);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, payload_buf+1*16, out_buf+1*16, 36, tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_cipher1, out_buf, 100) || memcmp_(std_tag1, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad null, payload not null) 1 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)out_buf, 100, "payload_out");
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad null, payload not null) 1 success\r\n");
	}

	tag_buf[0] = 0;
	memcpy_(payload_buf, std_cipher1, 100);
#if 0
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0, payload_buf, out_buf, 100, tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, payload_buf, out_buf, 1*16);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, payload_buf+1*16, out_buf+1*16, 36, (unsigned char *)tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(plain, out_buf, 100) || memcmp_(std_tag1, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad null, payload not null) 2 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)out_buf, 100, "payload_out");
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad null, payload not null) 2 success\r\n");
	}
	
#endif


	//2. aad not null, payload null
#if 1
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, aad, 100, NULL, NULL, 0, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, aad, 100);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, NULL, NULL, 0);
	ret |= chacha20_poly1305_update_including_last_data(ctx, NULL, NULL, 0, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag2, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad not null, payload null) 3 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad not null, payload null) 3 success\r\n");
	}
	
	tmp_tag[0] = 0;
#if 1
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, aad, 100, std_cipher1, NULL, 0, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, aad, 100);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, std_cipher1, tmp_out, 0);
	ret |= chacha20_poly1305_update_including_last_data(ctx, std_cipher1+0, tmp_out+0, 0, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag2, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 dec corner test(aad null, payload not null) 4 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_out, 100, "payload_out");
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dec corner test(aad null, payload not null) 4 success\r\n");
	}

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
	memcpy_(aad_buf, aad, 100);
#if 1
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, (unsigned int *)aad, 100, NULL, NULL, 0, (unsigned char *)tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, aad_buf, 100);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, NULL, NULL, 0);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, NULL, NULL, 0, tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag2, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad not null, payload null) 3 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad not null, payload null) 3 success\r\n");
	}

	tag_buf[0] = 0;
#if 1
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, (unsigned int *)aad, 100, NULL, NULL, 0, (unsigned char *)tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, aad_buf, 100);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, NULL, NULL, 0);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, NULL, NULL, 0, tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag2, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad not null, payload null) 4 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad not null, payload null) 4 success\r\n");
	}
#endif


	//3. aad null, payload null
#if 1
	ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0, NULL, NULL, 0, tmp_tag);
#else
	ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_update_excluding_last_data(ctx, NULL, NULL, 0);
	ret |= chacha20_poly1305_update_including_last_data(ctx, NULL, NULL, 0, tmp_tag);
#endif
	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag3, tmp_tag, 16))
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload null) 5 failure, ret=%x \r\n", ret);
		print_buf_U8(tmp_tag, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload null) 5 success\r\n");
	}

	tmp_tag[0]=0;
#if 0
		ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0, NULL, NULL, 0, tmp_tag);
#else
		ret = chacha20_poly1305_init(ctx, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0);
		ret |= chacha20_poly1305_update_excluding_last_data(ctx, NULL, NULL, 0);
		ret |= chacha20_poly1305_update_including_last_data(ctx, NULL, NULL, 0, tmp_tag);
#endif
		if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag3, tmp_tag, 16))
		{
			printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload null) 6 failure, ret=%x \r\n", ret);
			print_buf_U8(tmp_tag, 16, "tag");
			return 1;
		}
		else
		{
			printf("\r\n generate chacha20_poly1305 enc corner test(aad null, payload null) 6 success\r\n");
		}

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
#if 0
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0, NULL, NULL, 0, tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_ENCRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, NULL, NULL, 0);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, NULL, NULL, 0, (unsigned char *)tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag3, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad not null, payload null) 5 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma enc corner test(aad not null, payload null) 5 success\r\n");
	}

	tag_buf[0] = 0;
#if 0
	ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0, NULL, NULL, 0, tag_buf);
#else
	ret = chacha20_poly1305_dma_init(ctx2, CHACHA20_POLY1305_CRYPTO_DECRYPT, key, constant, iv, NULL, 0);
	ret |= chacha20_poly1305_dma_update_excluding_last_data(ctx2, NULL, NULL, 0);
	ret |= chacha20_poly1305_dma_update_including_last_data(ctx2, NULL, NULL, 0, (unsigned char *)tag_buf); 
#endif

	if(CHACHA20_POLY1305_SUCCESS != ret || memcmp_(std_tag3, tag_buf, 16))
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad not null, payload null) 6 failure, ret=%x \r\n", ret);
		print_buf_U8((unsigned char *)tag_buf, 16, "tag");
		return 1;
	}
	else
	{
		printf("\r\n generate chacha20_poly1305 dma dec corner test(aad not null, payload null) 6 success\r\n");
	}
#endif	
	
	return CHACHA20_POLY1305_SUCCESS;
}


#define TEST_PAYLOAD_MAX_BYTE    (1000) //attention stack size enough
#define TEST_AAD_MAX_BYTE        (1000)
#define TEST_ROUND               (5000)
unsigned int chacha20_poly1305_rand_test()
{
	unsigned int i;
	unsigned int ret;
	unsigned int payload_bytes;
	unsigned int aad_bytes;

	unsigned char local_std_aad[TEST_AAD_MAX_BYTE];
	unsigned char local_std_plain[TEST_PAYLOAD_MAX_BYTE];
	unsigned char local_std_key[32];
	unsigned char local_std_iv[8];
	unsigned int std_constant = 0;

	unsigned char cipher[TEST_PAYLOAD_MAX_BYTE];
	unsigned char replain[TEST_PAYLOAD_MAX_BYTE];
	unsigned char tag[16];
	unsigned char tag2[16];

	printf("\r\n\r\n =================== CHACHA20_POLY1305 rand test =================== \r\n");

	srand(0x87);
	
	for(i=0; i<TEST_ROUND+1; i++)
	{
		payload_bytes = (rand() & TEST_PAYLOAD_MAX_BYTE);
		aad_bytes = (rand() & TEST_AAD_MAX_BYTE);
		std_constant = (unsigned int)rand();

		get_rand(local_std_aad, aad_bytes);
		get_rand(local_std_plain, payload_bytes);
		get_rand(local_std_key, 32);
		get_rand(local_std_iv, 8);


		tag[0] = 0;
		tag2[0] = 1;

		//ENCRYPT
		ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_ENCRYPT, local_std_key, std_constant, local_std_iv, local_std_aad, aad_bytes, local_std_plain, cipher, payload_bytes, tag);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n generate chacha20_poly1305 failure ret=%u \r\n", ret);
			return ret;
		}

		//DECRYPT
		ret = chacha20_poly1305(CHACHA20_POLY1305_CRYPTO_DECRYPT, local_std_key, std_constant, local_std_iv, local_std_aad, aad_bytes, cipher, replain, payload_bytes, tag2);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n generate chacha20_poly1305 failure ret=%u \r\n", ret);
			return ret;
		}
				
		if(memcmp_(tag, tag2, 16) || memcmp_(replain, local_std_plain, payload_bytes))
		{
			printf("\r\n generate chacha20_poly1305 enc rand test %d failure \r\n", i);
			print_buf_U8(local_std_aad, aad_bytes, "aad");
			print_buf_U8(local_std_plain, payload_bytes, "plain");
			print_buf_U8(local_std_key, 32, "key");
			print_buf_U8(local_std_iv, 8, "iv");
			printf("constant=0x%08x\n", std_constant);
			return ret;
		}
		else if(0 == i % 100)
		{
			printf("\r\n generate chacha20_poly1305 rand test %d success\r\n", i);
		}

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
		/************** DMA style ***************/
		unsigned int *aad_buf = (unsigned int *)DMA_RAM_BASE;
		unsigned int *plain_buf = aad_buf + (TEST_AAD_MAX_BYTE>>2)+1;
		unsigned int *cipher_buf = plain_buf + (TEST_AAD_MAX_BYTE>>2)+1;
		unsigned int *replain_buf = cipher_buf + (TEST_AAD_MAX_BYTE>>2)+1;

		memcpy_(aad_buf, local_std_aad, aad_bytes);
		memcpy_(plain_buf, local_std_plain, payload_bytes);

		tag[0] = 0;
		tag2[0] = 1;

		//ENCRYPT
		ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_ENCRYPT, local_std_key, std_constant, local_std_iv, aad_buf, aad_bytes, plain_buf, cipher_buf, payload_bytes, (unsigned char *)tag);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n generate chacha20_poly1305 failure ret=%u \r\n", ret);
			return ret;
		}

		//DECRYPT
		ret = chacha20_poly1305_dma(CHACHA20_POLY1305_CRYPTO_DECRYPT, local_std_key, std_constant, local_std_iv, aad_buf, aad_bytes, cipher_buf, replain_buf, payload_bytes, tag2);
		if(CHACHA20_POLY1305_SUCCESS != ret)
		{
			printf("\r\n generate chacha20_poly1305 failure ret=%u \r\n", ret);
			return ret;
		}
		
		if(memcmp_(tag, tag2, 16) || memcmp_(replain_buf, local_std_plain, payload_bytes))
		{
			printf("\r\n generate chacha20_poly1305 dma rand test %d failure ret=%u\r\n", i, ret);
			print_buf_U8(local_std_aad, aad_bytes, "aad");
			print_buf_U8(local_std_plain, payload_bytes, "plain");
			print_buf_U8(local_std_key, 32, "key");
			print_buf_U8(local_std_iv, 8, "iv");
			printf("constant=0x%08x\n", std_constant);
			return ret;
		}
		else if(0 == i % 100)
		{
			printf("\r\n generate chacha20_poly1305 dma rand test %d success\r\n", i);
		}
#endif
	}

	return 0;
}


unsigned int CHACHA20_POLY1305_all_test()
{
	unsigned int ret;

	printf("\r\n\r\n =================== CHACHA20_POLY1305 all test =================== \r\n");

#if 1
	if(CHACHA20_POLY1305_SUCCESS != chacha20_poly1305_speed_test())
		return 1;
#endif

#if 1
	if(CHACHA20_POLY1305_SUCCESS != chacha20_poly1305_dma_speed_test())
		return 1;
#endif

#if 0
    //test software model speed
    if (chacha20_poly1305_sw_speed_test())
        return 1;
#endif

#if 1
	ret = chacha20_poly1305_sample_test();
	if (ret)
		return 1;
#endif

#if 1
	ret = chacha20_poly1305_corner_test();
	if (ret)
		return 1;
#endif

#if 1
	ret = chacha20_poly1305_standard_data_test();
	if (ret)
		return 1;
#endif

#if 1
	ret = chacha20_poly1305_rand_test();
	if (ret)
		return 1;
#endif

#if 0
	ret = chacha20_poly1305_with_software_rand_test();
	if (ret)
		return 1;
#endif

	printf("\r\n\r\n =================== CHACHA20_POLY1305 all test over =================== \r\n");

	return 0;
}

void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);

#ifdef CHACHA20_POLY1305_DMA_FUNCTION
    telink_chacha20_dma_init(DMA0, DMA1, DMA_BURST_1_WORD);
#endif /* CHACHA20_POLY1305_DMA_FUNCTION */
    CHACHA20_POLY1305_all_test();
}

void main_loop(void)
{
    gpio_toggle(LED1);
    delay_ms(50);
}

/********************************************************************************************************
 * @file	string.h
 *
 * @brief	This is the header file for BTBLE SDK
 *
 * @author	BTBLE GROUP
 * @date	2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#pragma once
#include "compiler.h"
#include "types.h"


#ifndef NULL
#define NULL  0
#endif

void *  memmove(void * dest, const void * src, unsigned int n);
//void *  memset(void * d, int c, unsigned int  n);
// void *  memcpy(void * des_ptr, const void * src_ptr, unsigned int );

// do not return void*,  otherwise, we must use a variable to store the dest porinter, that is not performance 
// void   	memcpy4(void * dest, const void * src, unsigned int);
void *	memchr(const void *_s, int _c, unsigned int _n);
int		memcmp(const void *_s1, const void *_s2, unsigned int _n);

char *	strcat(char *_s1, const char *_s2);
char *	strchr(const char *_s, int _c);
int		strcmp(const char *_s1, const char *_s2);
int		strcoll(const char *_s1, const char *_s2);
char *	strcpy(char *_s1, const char *_s2);
unsigned int	strcspn(const char *_s1, const char *_s2);
char *	strerror(int _errcode);
unsigned int	strlen(const char *_s);
char *	strncat(char *_s1, const char *_s2, unsigned int _n);
int		strncmp(const char *_s1, const char *_s2, unsigned int _n);
char *	strncpy(char *_s1, const char *_s2, unsigned int _n);
char *	strpbrk(const char *_s1, const char *_s2);
char *	strrchr(const char *_s, int _c);
unsigned int	strspn(const char *_s1, const char *_s2);
char *	strstr(const char *_s1, const char *_s2);
char *	strtok(char *_s1, const char *_s2);
unsigned int	strxfrm(char *_s1, const char *_s2, unsigned int _n);
char * 	strchr (register const char *s, int c);
void * 	memchr (register const void * src_void, int c, unsigned int length);
int ismemf4(void *data, unsigned int len);
void * memset4(void * dest, int val, unsigned int len);
void zeromem4(void *data, unsigned int len);

int tstrlen(const char *pStr);
int tmemcmp(const void * m1, const void *m2, u32 len);
int  tmemcmp4(void * m1, void * m2, register unsigned int len);
void * tmemset(void * dest, int val, unsigned int len);
void tmemcpy(void * out, const void * in, unsigned int length);
void tmemcpy4(void * d, const void * s, unsigned int length);
extern volatile uint32_t tdest_addr;
extern volatile uint32_t tdest_addr_end;

#define smemcmp				tmemcmp
#define smemcmp4			tmemcmp4
#define smemset				tmemset
#define smemcpy				tmemcpy
#define smemcpy4			tmemcpy4
#define memcpy 				tmemcpy
#define memcpy4 			tmemcpy4
//#define memset				tmemset

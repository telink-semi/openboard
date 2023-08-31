/********************************************************************************************************
 * @file    assert.h
 *
 * @brief   This is the header file for B91m
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
#ifndef _ASSERT_H_
#define _ASSERT_H_
#include <printf.h>
#if(!DEBUG_MODE)
#define assert(expression) ((void) 0)
#else

static inline void bad_assertion(const char *mess) {
	printf(mess);
	while(1);
}

#define  __str(x)  	# x
#define  __xstr(x)  __str(x)
#define  assert(expr)  ((expr)? (void)0 : \
		 bad_assertion("Assertion failed, file "__xstr(__FILE__)", line" __xstr(__LINE__) "\n"))
#endif
#endif

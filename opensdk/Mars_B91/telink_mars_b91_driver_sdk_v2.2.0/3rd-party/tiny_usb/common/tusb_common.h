/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This file is part of the TinyUSB stack.
 */

/** \ingroup Group_Common
 *  \defgroup Group_CommonH common.h
 *  @{ */
#pragma once
//--------------------------------------------------------------------+
// Includes
//--------------------------------------------------------------------+

// Standard Headers
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>


#define TU_ATTR_WEAK __attribute__((weak))


#define TU_BSWAP16(u16) (__builtin_bswap16(u16))
#define TU_BSWAP32(u32) (__builtin_bswap32(u32))

#define tu_htons(u16)  (TU_BSWAP16(u16))
#define tu_ntohs(u16)  (TU_BSWAP16(u16))

#define tu_htonl(u32)  (TU_BSWAP32(u32))
#define tu_ntohl(u32)  (TU_BSWAP32(u32))

//--------------------------------------------------------------------+
// Macros Helper
//--------------------------------------------------------------------+
#define TU_ARRAY_SIZE(_arr)   ( sizeof(_arr) / sizeof(_arr[0]) )
#define TU_MIN(_x, _y)        ( ( (_x) < (_y) ) ? (_x) : (_y) )
#define TU_MAX(_x, _y)        ( ( (_x) > (_y) ) ? (_x) : (_y) )

#define TU_U16_HIGH(u16)      ((unsigned char) (((u16) >> 8) & 0x00ff))
#define TU_U16_LOW(u16)       ((unsigned char) ((u16)       & 0x00ff))
#define U16_TO_U8S_BE(u16)    TU_U16_HIGH(u16), TU_U16_LOW(u16)
#define U16_TO_U8S_LE(u16)    TU_U16_LOW(u16), TU_U16_HIGH(u16)

#define TU_U32_BYTE3(u32)     ((unsigned char) ((((unsigned int) u32) >> 24) & 0x000000ff)) // MSB
#define TU_U32_BYTE2(u32)     ((unsigned char) ((((unsigned int) u32) >> 16) & 0x000000ff))
#define TU_U32_BYTE1(u32)     ((unsigned char) ((((unsigned int) u32) >>  8) & 0x000000ff))
#define TU_U32_BYTE0(u32)     ((unsigned char) (((unsigned int)  u32)        & 0x000000ff)) // LSB

#define U32_TO_U8S_BE(u32)    TU_U32_BYTE3(u32), TU_U32_BYTE2(u32), TU_U32_BYTE1(u32), TU_U32_BYTE0(u32)
#define U32_TO_U8S_LE(u32)    TU_U32_BYTE0(u32), TU_U32_BYTE1(u32), TU_U32_BYTE2(u32), TU_U32_BYTE3(u32)

#define TU_BIT(n)             (1U << (n))



//------------- Mem -------------//
#define tu_memclr(buffer, size)  memset((buffer), 0, (size))
#define tu_varclr(_var)          tu_memclr(_var, sizeof(*(_var)))

//------------- Bytes -------------//
static inline unsigned int tu_u32(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
  return ( ((unsigned int) b3) << 24) | ( ((unsigned int) b2) << 16) | ( ((unsigned int) b1) << 8) | b0;
}

static inline unsigned short tu_u16(unsigned char high, unsigned char low)
{
  return (unsigned short) ((((unsigned short) high) << 8) | low);
}

static inline unsigned char tu_u32_byte3(unsigned int tu_u32) { return TU_U32_BYTE3(tu_u32); }
static inline unsigned char tu_u32_byte2(unsigned int tu_u32) { return TU_U32_BYTE2(tu_u32); }
static inline unsigned char tu_u32_byte1(unsigned int tu_u32) { return TU_U32_BYTE1(tu_u32); }
static inline unsigned char tu_u32_byte0(unsigned int tu_u32) { return TU_U32_BYTE0(tu_u32); }

static inline unsigned char tu_u16_high(unsigned short tu_u16) { return TU_U16_HIGH(tu_u16); }
static inline unsigned char tu_u16_low (unsigned short tu_u16) { return TU_U16_LOW(tu_u16); }

//------------- Bits -------------//
static inline unsigned int tu_bit_set  (unsigned int value, unsigned char pos) { return value | TU_BIT(pos);                  }
static inline unsigned int tu_bit_clear(unsigned int value, unsigned char pos) { return value & (~TU_BIT(pos));               }
static inline bool     tu_bit_test (unsigned int value, unsigned char pos) { return (value & TU_BIT(pos)) ? true : false; }

//------------- Min -------------//
static inline unsigned char  tu_min8  (unsigned char  x, unsigned char y ) { return (x < y) ? x : y; }
static inline unsigned short tu_min16 (unsigned short x, unsigned short y) { return (x < y) ? x : y; }
static inline unsigned int tu_min32 (unsigned int x, unsigned int y) { return (x < y) ? x : y; }

//------------- Max -------------//
static inline unsigned char  tu_max8  (unsigned char  x, unsigned char y ) { return (x > y) ? x : y; }
static inline unsigned short tu_max16 (unsigned short x, unsigned short y) { return (x > y) ? x : y; }
static inline unsigned int tu_max32 (unsigned int x, unsigned int y) { return (x > y) ? x : y; }

//------------- Align -------------//
static inline unsigned int tu_align(unsigned int value, unsigned int alignment)
{
  return value & ((unsigned int) ~(alignment-1));
}

static inline unsigned int tu_align16 (unsigned int value) { return (value & 0xFFFFFFF0UL); }
static inline unsigned int tu_align32 (unsigned int value) { return (value & 0xFFFFFFE0UL); }
static inline unsigned int tu_align4k (unsigned int value) { return (value & 0xFFFFF000UL); }
static inline unsigned int tu_offset4k(unsigned int value) { return (value & 0xFFFUL); }

//------------- Mathematics -------------//
static inline unsigned int tu_abs(int value) { return (unsigned int)((value < 0) ? (-value) : value); }

/// inclusive range checking TODO remove
static inline bool tu_within(unsigned int lower, unsigned int value, unsigned int upper)
{
  return (lower <= value) && (value <= upper);
}

// log2 of a value is its MSB's position
// TODO use clz TODO remove
static inline unsigned char tu_log2(unsigned int value)
{
  unsigned char result = 0;
  while (value >>= 1) { result++; }
  return result;
}

// MCU that could access unaligned memory natively
static inline unsigned int tu_unaligned_read32  (const void* mem           ) { return *((unsigned int*) mem);  }
static inline unsigned short tu_unaligned_read16  (const void* mem           ) { return *((unsigned short*) mem);  }

static inline void     tu_unaligned_write32 (void* mem, unsigned int value ) { *((unsigned int*) mem) = value; }
static inline void     tu_unaligned_write16 (void* mem, unsigned short value ) { *((unsigned short*) mem) = value; }

// To be removed
//------------- Binary constant -------------//

#define TU_BIN8(x)               ((unsigned char)  (0b##x))
#define TU_BIN16(b1, b2)         ((unsigned short) (0b##b1##b2))
#define TU_BIN32(b1, b2, b3, b4) ((unsigned int) (0b##b1##b2##b3##b4))

//--------------------------------------------------------------------+
// Debug Function
//--------------------------------------------------------------------+

// CFG_TUSB_DEBUG for debugging
// 0 : no debug
// 1 : print when there is error
// 2 : print out log
#define CFG_TUSB_DEBUG 0
void tu_print_mem(void const *buf, unsigned int count, unsigned char indent);

#ifdef CFG_TUSB_DEBUG_PRINTF
  extern int CFG_TUSB_DEBUG_PRINTF(const char *format, ...);
  #define tu_printf    CFG_TUSB_DEBUG_PRINTF
#else
  #define tu_printf printf
#endif

static inline
void tu_print_var(unsigned char const* buf, unsigned int bufsize)
{
  for(unsigned int i=0; i<bufsize; i++) tu_printf("%02X ", buf[i]);
}

// Log with debug level 1
#define TU_LOG1               tu_printf
#define TU_LOG1_VAR(_x)       tu_print_var((unsigned char const*)(_x), sizeof(*(_x)))
#define TU_LOG1_INT(_x)       tu_printf(#_x " = %ld\n", (unsigned int) (_x) )
#define TU_LOG1_HEX(_x)       tu_printf(#_x " = %lX\n", (unsigned int) (_x) )
#define TU_LOG1_LOCATION()    tu_printf("%s: %d:\r\n", __PRETTY_FUNCTION__, __LINE__)
#define TU_LOG1_FAILED()      tu_printf("%s: %d: Failed\r\n", __PRETTY_FUNCTION__, __LINE__)


  #define TU_LOG2             TU_LOG1
  #define TU_LOG2_VAR         TU_LOG1_VAR
  #define TU_LOG2_INT         TU_LOG1_INT
  #define TU_LOG2_HEX         TU_LOG1_HEX
  #define TU_LOG2_LOCATION()  TU_LOG1_LOCATION()



typedef struct
{
  unsigned int key;
  const char* data;
} tu_lookup_entry_t;

typedef struct
{
  unsigned short count;
  tu_lookup_entry_t const* items;
} tu_lookup_table_t;

static inline const char* tu_lookup_find(tu_lookup_table_t const* p_table, unsigned int key)
{
  for(unsigned short i=0; i<p_table->count; i++)
  {
    if (p_table->items[i].key == key) return p_table->items[i].data;
  }

  return NULL;
}
 // CFG_TUSB_DEBUG



/**  @} */

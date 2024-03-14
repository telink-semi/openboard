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

#pragma once

#define TUSB_VERSION_MAJOR     0
#define TUSB_VERSION_MINOR     10
#define TUSB_VERSION_REVISION  0
/** \defgroup group_mcu Supported MCU
 * \ref CFG_TUSB_MCU must be defined to one of these
 *  @{ */

#define OPT_MCU_NONE                0

// Telink TLSR9
#define OPT_MCU_TLSR9x1x       	 1500 ///< Telink TLSR9x1x
/** @} */

/** \defgroup group_supported_os Supported RTOS
 *  \ref CFG_TUSB_OS must be defined to one of these
 *  @{ */
#define OPT_OS_NONE       1  ///< No RTOS

/** @} */

// Allow to use command line to change the config name/location
  #include "tusb_config.h"

/** \addtogroup group_configuration
 *  @{ */


//--------------------------------------------------------------------
// RootHub Mode Configuration
// CFG_TUSB_RHPORTx_MODE contains operation mode and speed for that port
//--------------------------------------------------------------------

// Low byte is operational mode
#define OPT_MODE_NONE           0x0000 ///< Disabled
#define OPT_MODE_DEVICE         0x0001 ///< Device Mode

// High byte is max operational speed (corresponding to tusb_speed_t)
#define OPT_MODE_DEFAULT_SPEED  0x0000 ///< Default (max) speed supported by MCU
#define OPT_MODE_LOW_SPEED      0x0100 ///< Low Speed
#define OPT_MODE_FULL_SPEED     0x0200 ///< Full Speed

/** @} */

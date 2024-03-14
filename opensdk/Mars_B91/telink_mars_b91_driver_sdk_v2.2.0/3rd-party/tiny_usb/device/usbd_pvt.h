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

#include "../common/tusb_fifo.h"
#include "../osal/osal_none.h"


//--------------------------------------------------------------------+
// Class driver
//--------------------------------------------------------------------+

typedef struct
{
  #if CFG_TUSB_DEBUG >= 2
  char const* name;
  #endif

  void           (* init             ) (void);
  void           (* reset            ) (unsigned char rhport);
  unsigned short (* open             ) (unsigned char rhport, tusb_desc_interface_t const * desc_intf, unsigned short max_len);
  bool           (* xfer_cb          ) (unsigned char rhport, unsigned char ep_addr, xfer_result_t result, unsigned int xferred_bytes);
  void           (* sof              ) (unsigned char rhport); /* optional */
} usbd_class_driver_t;

// Invoked when initializing device stack to get additional class driver.
// Can optionally implemented by application to extend/overwrite class driver support.
// Note: The driver array must be accessible at all time when stack is active
usbd_class_driver_t const* usbd_app_driver_get_cb(unsigned char* driver_count) TU_ATTR_WEAK;


//--------------------------------------------------------------------+
// USBD Endpoint API
//--------------------------------------------------------------------+

// Open an endpoint
bool usbd_edpt_open(unsigned char rhport, tusb_desc_endpoint_t const * desc_ep);

// Close an endpoint
void usbd_edpt_close(unsigned char rhport, unsigned char ep_addr);

// Submit a usb transfer
bool usbd_edpt_xfer(unsigned char rhport, unsigned char ep_addr, unsigned char * buffer, unsigned short total_bytes);

// Claim an endpoint before submitting a transfer.
// If caller does not make any transfer, it must release endpoint for others.
bool usbd_edpt_claim(unsigned char rhport, unsigned char ep_addr);

// Release an endpoint without submitting a transfer
bool usbd_edpt_release(unsigned char rhport, unsigned char ep_addr);

// Check if endpoint transferring is complete
bool usbd_edpt_busy(unsigned char rhport, unsigned char ep_addr);

// Stall endpoint
void usbd_edpt_stall(unsigned char rhport, unsigned char ep_addr);

// Clear stalled endpoint
void usbd_edpt_clear_stall(unsigned char rhport, unsigned char ep_addr);

// Check if endpoint is stalled
bool usbd_edpt_stalled(unsigned char rhport, unsigned char ep_addr);
static inline bool usbd_edpt_ready(unsigned char rhport, unsigned char ep_addr)
{
  return !usbd_edpt_busy(rhport, ep_addr) && !usbd_edpt_stalled(rhport, ep_addr);
}

/*------------------------------------------------------------------*/
/* Helper
 *------------------------------------------------------------------*/

bool usbd_open_edpt_pair(unsigned char rhport, unsigned char const* p_desc, unsigned char ep_count, unsigned char xfer_type, unsigned char* ep_out, unsigned char* ep_in);


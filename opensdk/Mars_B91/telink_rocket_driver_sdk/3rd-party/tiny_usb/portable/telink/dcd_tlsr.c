/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2020 William D. Jones
 * Copyright (c) 2019-2020 Ha Thach (tinyusb.org)
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

#include "driver.h"
#include "../../common/tusb_types.h"
#include "../../device/dcd.h"
#include "../../tusb_option.h"

/* Telink USB has nine endpoints, endpoint 0 and endpoint 1 to 8. Except endpoint 0 (support IN and OUT),
each endpoint can only be either an IN endpoint or an OUT endpoint (Endpoint 5 and endpoint 6 can only
be IN endpoints, other endpoints can only be OUT endpoints.). Endpoint 0 can only use the control transfer,
endpoint 1 to 8 supports other three transfer types except the control transfer, endpoint 6 and endpoint 7
uses the isochronous transfer by default */
// 0x01
// 0x02
// 0x03
// 0x04
// 0x85
// 0x86
// 0x07
// 0x08

#define EP_MAX            8

typedef struct
{
  unsigned char *buffer;
  unsigned short total_len;
  unsigned short queued_len;
  unsigned short max_size;
  bool isochronous;
  bool short_packet;
} xfer_ctl_t;

xfer_ctl_t xfer_status[EP_MAX][2];

#define XFER_CTL_BASE(_ep, _dir) &xfer_status[_ep][_dir]

static void transmit_packet(unsigned char ep_num);

/*------------------------------------------------------------------*/
/* Controller API
 *------------------------------------------------------------------*/

// Initialize controller to device mode
void dcd_init(unsigned char rhport)
{
  (void) rhport;
  // do nothing initialized in user_init
}

// Enable device interrupt
void dcd_int_enable(unsigned char rhport)
{
  (void) rhport;
  plic_interrupt_enable(IRQ_USB_ENDPOINT);
}

// Disable device interrupt
void dcd_int_disable(unsigned char rhport)
{
  (void) rhport;
  plic_interrupt_disable(IRQ_USB_ENDPOINT);
}

// Receive Set Address request, mcu port must also include status IN response
void dcd_set_address(unsigned char rhport, unsigned char dev_addr)
{
  (void) rhport;
  (void) dev_addr;
  // TO implement
}

// Wake up host
void dcd_remote_wakeup(unsigned char rhport)
{
  (void) rhport;
  // TO implement
}

// Connect by enabling internal pull-up resistor on D+/D-
void dcd_connect(unsigned char rhport)
{
  (void) rhport;
  usb_dp_pullup_en(1);
}

// Disconnect by disabling internal pull-up resistor on D+/D-
void dcd_disconnect(unsigned char rhport)
{
  (void) rhport;
  usb_dp_pullup_en(0);
}

// Enable/Disable Start-of-frame interrupt. Default is disabled
void dcd_sof_enable(unsigned char rhport, bool en)
{
  (void) rhport;
  (void) en;
  // TO implement
}

//--------------------------------------------------------------------+
// Endpoint API
//--------------------------------------------------------------------+

// Invoked when a control transfer's status stage is complete.
// May help DCD to prepare for next control transfer, this API is optional.
void dcd_edpt0_status_complete(unsigned char rhport, tusb_control_request_t const *request)
{
  (void) rhport;
  (void) request;
  // TO implement
}

// Configure endpoint's registers according to descriptor
bool dcd_edpt_open(unsigned char rhport, tusb_desc_endpoint_t const *desc_edpt)
{
  (void) rhport;

  unsigned char const epnum = tu_edpt_number(desc_edpt->bEndpointAddress);
  unsigned char const dir = tu_edpt_dir(desc_edpt->bEndpointAddress);

  xfer_ctl_t *xfer = XFER_CTL_BASE(epnum, dir);
  xfer->max_size = desc_edpt->wMaxPacketSize.size;

  usbhw_set_ep_en(epnum & 0x07, 1);
  usbhw_set_eps_irq_mask(BIT(epnum & 0x07));

  if (desc_edpt->bmAttributes.xfer == 1)
  {
    /* enable endpoint iso mode */
    BM_SET(reg_usb_iso_mode,BIT(epnum & 0x07));
    xfer->isochronous = 1;
  }
  else
  {
    /* disable endpoint iso mode */
    BM_CLR(reg_usb_iso_mode,BIT(epnum & 0x07));
    xfer->isochronous = 0;
  }

  return true;
}

// Close all non-control endpoints, cancel all pending transfers if any.
// Invoked when switching from a non-zero Configuration by SET_CONFIGURE therefore
// required for multiple configuration support.
void dcd_edpt_close_all(unsigned char rhport)
{
  (void) rhport;

  for(unsigned char i = 0; i < EP_MAX; i++)
  {
    /* disable hw endpoint */
    usbhw_set_ep_en(i & 0x07, 0);
    usbhw_clr_eps_irq_mask(BIT(i & 0x07));

    // disable OUT endpoint
    xfer_status[i][TUSB_DIR_OUT].max_size = 0;
    // disable IN endpoint
    xfer_status[i][TUSB_DIR_IN].max_size = 0;
  }
}

// Close an endpoint.
// Since it is weak, caller must TU_ASSERT this function's existence before calling it.
void dcd_edpt_close(unsigned char rhport, unsigned char ep_addr)
{
  (void) rhport;

  volatile unsigned char const epnum = tu_edpt_number(ep_addr);
  unsigned char const dir = tu_edpt_dir(ep_addr);

  usbhw_set_ep_en(epnum & 0x07, 0);
  usbhw_clr_eps_irq_mask(BIT(epnum & 0x07));

  // Update max_size
  xfer_status[epnum][dir].max_size = 0;  // max_size = 0 marks a disabled EP
}

// Submit a transfer, When complete dcd_event_xfer_complete() is invoked to notify the stack
bool dcd_edpt_xfer(unsigned char rhport, unsigned char ep_addr, unsigned char *buffer, unsigned short total_bytes)
{
  (void) rhport;

  volatile unsigned char const epnum = tu_edpt_number(ep_addr);
  unsigned char const dir = tu_edpt_dir(ep_addr);

  xfer_ctl_t *xfer = XFER_CTL_BASE(epnum, dir);
  xfer->buffer = buffer;
  xfer->total_len = total_bytes;
  xfer->queued_len = 0;
  xfer->short_packet = false;

  if (dir == TUSB_DIR_IN)
  {
    transmit_packet(epnum);
  }
  if (dir == TUSB_DIR_OUT)
  {
    usbhw_data_ep_ack(epnum);
  }
  dcd_int_enable(0);

  return true;
}

// Stall endpoint, any queuing transfer should be removed from endpoint
void dcd_edpt_stall(unsigned char rhport, unsigned char ep_addr)
{
  (void) rhport;

  unsigned char const epnum = tu_edpt_number(ep_addr);
  usbhw_data_ep_stall(epnum);
}

// clear stall, data toggle is also reset to DATA0
// This API never calls with control endpoints, since it is auto cleared when receiving setup packet
void dcd_edpt_clear_stall(unsigned char rhport, unsigned char ep_addr)
{
  (void) rhport;

  unsigned char const epnum = tu_edpt_number(ep_addr);
  usbhw_data_ep_ack(epnum);
}

static void receive_packet(unsigned char ep_num)
{
  xfer_ctl_t *xfer = XFER_CTL_BASE(ep_num, TUSB_DIR_OUT);
  unsigned char xfer_size;

  /* get receive data length */
  xfer_size = reg_usb_ep_ptr(ep_num);

  unsigned short remaining = xfer->total_len - xfer->queued_len;
  unsigned short to_recv_size;

  if (remaining <= xfer->max_size)
  {
    // Avoid buffer overflow.
    to_recv_size = (xfer_size > remaining) ? remaining : xfer_size;
  }
  else
  {
    // Room for full packet, choose recv_size
    to_recv_size = (xfer_size > xfer->max_size) ? xfer->max_size : xfer_size;
  }

  unsigned char *base = (xfer->buffer + xfer->queued_len);

  usbhw_reset_ep_ptr(ep_num);
  for (unsigned short i = 0; i < to_recv_size; i++)
  {
    base[i] = usbhw_read_ep_data(ep_num);
  }

  xfer->queued_len += xfer_size;

  xfer->short_packet = (xfer_size < xfer->max_size);
  if ((xfer->total_len == xfer->queued_len) || xfer->short_packet)
  {
    dcd_event_xfer_complete(0, ep_num, xfer->queued_len, XFER_RESULT_SUCCESS, true);
    dcd_int_disable(0);
  }
  else
  {
    usbhw_data_ep_ack(ep_num);
  }
}

static void transmit_packet(unsigned char ep_num)
{
  if(ep_num >= EP_MAX)  return;
  xfer_ctl_t *xfer = XFER_CTL_BASE(ep_num, TUSB_DIR_IN);

  // First, determine whether we should even send a packet or finish
  // up the xfer.
  bool zlp = (xfer->total_len == 0); // By necessity, xfer->total_len will
                     // equal xfer->queued_len for ZLPs.
                     // Of course a ZLP is a short packet.
  if ((!zlp && (xfer->total_len == xfer->queued_len)) || xfer->short_packet)
  {
    dcd_event_xfer_complete(0, ep_num | TUSB_DIR_IN_MASK, xfer->queued_len, XFER_RESULT_SUCCESS, true);
    dcd_int_disable(0);
    return;
  }

  // Then actually commit to transmit a packet.
  unsigned short remaining = xfer->total_len - xfer->queued_len;
  unsigned char xfer_size = (xfer->max_size < xfer->total_len) ? xfer->max_size : remaining;

  if (xfer_size < xfer->max_size)
  {
    // Next "xfer complete interrupt", the transfer will end.
    xfer->short_packet = true;
  }

  unsigned char *base = (xfer->buffer + xfer->queued_len);
  xfer->queued_len += xfer_size;
  usbhw_reset_ep_ptr(ep_num);
  for (int i = 0; i < xfer_size; i++)
  {
    usbhw_write_ep_data(ep_num, base[i]);
  }
  usbhw_data_ep_ack(ep_num);
}

//--------------------------------------------------------------------+
// Event API (implemented by stack)
//--------------------------------------------------------------------+

// Called by DCD to notify device stack
void dcd_int_handler(unsigned char rhport)
{
  (void) rhport;
  unsigned char irq = usbhw_get_eps_irq();

  if (irq & FLD_USB_EDP5_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP5_IRQ);
    receive_packet(5);
  }
  if (irq & FLD_USB_EDP6_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP6_IRQ);
    receive_packet(6);
  }
  if (irq & FLD_USB_EDP8_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP8_IRQ);
    transmit_packet(8);
  }
  if (irq & FLD_USB_EDP7_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP7_IRQ);
    transmit_packet(7);
  }
  if (irq & FLD_USB_EDP4_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP4_IRQ);
    transmit_packet(4);
  }
  if (irq & FLD_USB_EDP3_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP3_IRQ);
    transmit_packet(3);
  }
  if (irq & FLD_USB_EDP2_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP2_IRQ);
    transmit_packet(2);
  }
  if (irq & FLD_USB_EDP1_IRQ)
  {
    usbhw_clr_eps_irq(FLD_USB_EDP1_IRQ);
    transmit_packet(1);
  }
}

void usb_ctrl_reset_cb(void)
{
  dcd_event_bus_reset(0, TUSB_SPEED_FULL, 1);
}

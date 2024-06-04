/********************************************************************************************************
 * @file    myudb_usb.c
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
#if(1)
#include "fifo.h"
#include "reg_include/usb_reg.h"
#define			HCI_VCD_EN		0
#include "myudb.h"
#include "string.h"
#include "myudb_usbdesc.h"
#ifndef  BUFFUSB_NUM
#define			BUFFUSB_NUM		16
#endif

#ifndef BUFFUSB_SIZE
#define			BUFFUSB_SIZE	280
#endif


static unsigned short	myudb_id = 0x120;
static unsigned char * myudb_g_response = 0;
static unsigned short 	myudb_g_response_len = 0;
static int 	myudb_g_stall = 0;
static USB_Request_Header_t control_request;


unsigned char 		 	myudb_txfifo_b[BUFFUSB_SIZE * BUFFUSB_NUM];
my_fifo_t	myudb_txfifo = {	BUFFUSB_SIZE,
							BUFFUSB_NUM,
							0,
							0,
							myudb_txfifo_b};
my_fifo_t	*myudb_fifo = 0;



//-----------------------------------------------------------------------------------------
enum {
	MYUDB_USB_IRQ_SETUP_REQ = 0,
	MYUDB_USB_IRQ_DATA_REQ,
};

void myudb_set_txfifo_local (void)
{
	myudb_fifo = &myudb_txfifo;
}

void myudb_set_txfifo (void *p)
{
	myudb_fifo = p;
}

//-----------------------------------------------------------------------------------------
void myudb_usb_send_response(void) {
	unsigned short n;
	if (myudb_g_response_len < 8) {
		n = myudb_g_response_len;
	} else {
		n = 8;
	}
	myudb_g_response_len -= n;
	usbhw_reset_ctrl_ep_ptr();
	while (n-- > 0) {
		usbhw_write_ctrl_ep_data(*myudb_g_response);
		++myudb_g_response;
	}
}

void myudb_usb_prepare_desc_data(void) {
	unsigned char value_l = (control_request.wValue) & 0xff;
	unsigned char value_h = (control_request.wValue >> 8) & 0xff;

	myudb_g_response = 0;
	myudb_g_response_len = 0;

	switch (value_h) {

	case DTYPE_Device:
		myudb_g_response = myudb_usbdesc_get_device();
		myudb_g_response_len = sizeof(USB_Descriptor_Device_t);
		break;

	case DTYPE_Configuration:
		myudb_g_response = myudb_usbdesc_get_configuration();
		myudb_g_response_len = sizeof(MYUDB_USB_Descriptor_Configuration_t);
		break;

	case DTYPE_String:
		if (MYUDB_USB_STRING_LANGUAGE == value_l) {
			myudb_g_response = myudb_usbdesc_get_language();
			myudb_g_response_len = sizeof(LANGUAGE_ID_ENG);
		} else if (MYUDB_USB_STRING_VENDOR == value_l) {
			myudb_g_response = myudb_usbdesc_get_vendor();
			myudb_g_response_len = sizeof (MYUDB_STRING_VENDOR);
		} else if (MYUDB_USB_STRING_PRODUCT == value_l) {
			myudb_g_response = myudb_usbdesc_get_product();
			myudb_g_response_len = sizeof(MYUDB_STRING_PRODUCT);
		} else if (MYUDB_USB_STRING_SERIAL == value_l) {
			myudb_g_response = myudb_usbdesc_get_serial();
			myudb_g_response_len = sizeof(MYUDB_STRING_SERIAL);
		} else {
			myudb_g_stall = 1;
		}
		break;

	default:
		myudb_g_stall = 1;
		break;

	}

	if (control_request.wLength < myudb_g_response_len) {
		myudb_g_response_len = control_request.wLength;
	}

	return;
}

void myudb_usb_handle_in_class_intf_req(void) {
	unsigned char property = control_request.bRequest;
	switch (property) {
		case 0x00:
			usbhw_write_ctrl_ep_data(0x00);
			break;
		default:
			myudb_g_stall = 1;
			break;
	}

}

void myudb_usb_handle_request(unsigned char data_request) {
	unsigned char bmRequestType = control_request.bmRequestType;
	unsigned char bRequest = control_request.bRequest;

	usbhw_reset_ctrl_ep_ptr();
	switch (bmRequestType) {
	case (REQDIR_DEVICETOHOST | REQTYPE_STANDARD | REQREC_DEVICE):
		if (REQ_GetDescriptor == bRequest) {
			if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
				myudb_usb_prepare_desc_data();
			}
			myudb_usb_send_response();
		}
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE):
		myudb_usb_handle_in_class_intf_req();
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_INTERFACE):
		if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
			if (0xc0 == bRequest) {					// Get board version
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(myudb_id);
				usbhw_write_ctrl_ep_data(myudb_id >> 8);
			}
			else
			{
				myudb_g_stall = 1;
			}
		}
		break;
	case (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQREC_DEVICE):	// 0xc0
		if (MYUDB_USB_IRQ_SETUP_REQ == data_request) {
			if (0xc0 == bRequest) {					// Get board version
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(0x40);
				usbhw_write_ctrl_ep_data(0x25);
				usbhw_write_ctrl_ep_data(0x40);
				usbhw_write_ctrl_ep_data(0x05);
				usbhw_write_ctrl_ep_data(0x03);
				usbhw_write_ctrl_ep_data(0x00);
				usbhw_write_ctrl_ep_data(0x01);
				usbhw_write_ctrl_ep_data(0x00);
			}
			else if (0xc6 == bRequest) {			//
				usbhw_reset_ctrl_ep_ptr();
				usbhw_write_ctrl_ep_data(0x04);
			}
			else
			{
				myudb_g_stall = 1;
			}
		}
		break;
	case (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQREC_DEVICE):	// 0x40
		myudb_g_stall = 1;
		break;
	default:
		myudb_g_stall = 1;
		break;
	}

	return;
}

void myudb_usb_handle_ctl_ep_setup(void) {
	usbhw_reset_ctrl_ep_ptr();
	control_request.bmRequestType = usbhw_read_ctrl_ep_data();
	control_request.bRequest = usbhw_read_ctrl_ep_data();
	control_request.wValue = usbhw_read_ctrl_ep_u16();
	control_request.wIndex = usbhw_read_ctrl_ep_u16();
	control_request.wLength = usbhw_read_ctrl_ep_u16();
	myudb_g_stall = 0;
	myudb_usb_handle_request(MYUDB_USB_IRQ_SETUP_REQ);
	if (myudb_g_stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void myudb_usb_handle_ctl_ep_data(void) {
	usbhw_reset_ctrl_ep_ptr();
	myudb_g_stall = 0;
	myudb_usb_handle_request(MYUDB_USB_IRQ_DATA_REQ);
	if (myudb_g_stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_DAT_ACK);
}

void myudb_usb_handle_ctl_ep_status(void) {
	if (myudb_g_stall)
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_STALL);
	else
		usbhw_write_ctrl_ep_ctrl(FLD_EP_STA_ACK);
}

_attribute_ram_code_sec_noinline_ void usb_send_status_pkt(unsigned char status, unsigned char buffer_num, unsigned char *pkt, unsigned short len)
{
	if (((myudb_fifo->wptr - myudb_fifo->rptr) & 255) >= myudb_fifo->num ) return;		//skip if overflow

	my_irq_disable ();
	unsigned char *p = myudb_fifo->p + (myudb_fifo->wptr++ & (myudb_fifo->num - 1)) * myudb_fifo->size;
	if (len > 272)
	{
		len = 272;
	}
	*p++ = len + 2;
	*p++ = (len + 2) >> 8;
	p += 2;
	*p++ = status;
	*p++ = buffer_num;
	while (len--)
	{
		*p++ = *pkt++;
	}
	my_irq_restore ();
}

void usb_send_str_data (char *str, unsigned char *ph, int n)
{
	if (((myudb_fifo->wptr - myudb_fifo->rptr) & 255) >= myudb_fifo->num ) return;		//skip if overflow

	my_irq_disable ();
	unsigned char *ps =  myudb_fifo->p + (myudb_fifo->wptr & (myudb_fifo->num - 1)) * myudb_fifo->size;;
	unsigned char *pd = ps;

	int ns = strlen (str);
	if (n > 272)
	{
		n = 272;
		ns = 0;
	}
	if (n + ns > 272)
	{
		ns = 272 - n;
	}

	int len = n + ns + 2 + 3;
	*pd++ = len;
	*pd++ = len >> 8;
	*pd++ = 0;
	*pd++ = 0;
	*pd++ = 0x82;
	*pd++ = 8;

	*pd++ = 0x22;
	*pd++ = n;
	*pd++ = n >> 8;
	while (n--)
	{
		*pd++ = *ph++;
	}
	while (ns--)
	{
		*pd++ = *str++;
	}
	myudb_fifo->wptr++;
	my_irq_restore ();
}

_attribute_ram_code_sec_noinline_ void myudb_to_usb(void)
{
	static unsigned short len = 0;
	static unsigned char *p = 0;

	if(usbhw_is_ep_busy(MYUDB_EDP_IN_HCI)) return;
	if (!p && (myudb_fifo->wptr != myudb_fifo->rptr))		//first packet
	{
		p =  myudb_fifo->p + (myudb_fifo->rptr++ & (myudb_fifo->num - 1)) * myudb_fifo->size;;
		//len = p[1] + 3;
		len = p[0] + p[1] * 256;
		p += 4;
	}
	if (p)
	{
		int n = len < 64 ? len : 64;
		usbhw_reset_ep_ptr(MYUDB_EDP_IN_HCI);
		for (int i=0; i<n; i++)
		{
			usbhw_write_ep_data(MYUDB_EDP_IN_HCI, *p++);
		}
		usbhw_data_ep_ack(MYUDB_EDP_IN_HCI);
		len -= n;
		if (n < 64)
		{
			p = 0;
		}
	}
}

#define			USB_ENDPOINT_BULK_OUT_FLAG		(1 << (MYUDB_EDP_OUT_HCI & 7))

_attribute_ram_code_sec_noinline_ int myudb_usb_get_packet (unsigned char *p)
{
	static	int len = 0;
	if (reg_usb_ep_irq_status & USB_ENDPOINT_BULK_OUT_FLAG)
	{
		//clear interrupt flag
		reg_usb_ep_irq_status = USB_ENDPOINT_BULK_OUT_FLAG;

		// read data
		int n = reg_usb_ep_ptr(MYUDB_EDP_OUT_HCI);
		reg_usb_ep_ptr(MYUDB_EDP_OUT_HCI) = 0;
		for (int i=0; i<n; i++)
		{
			p[len++] = reg_usb_ep_dat(MYUDB_EDP_OUT_HCI);
		}
		reg_usb_ep_ctrl (MYUDB_EDP_OUT_HCI) = 1;		//ACK
		if (n < 64)
		{
			n = len;
			len = 0;
			return n;
		}
	}
	return 0;
}

func_myudb_hci_cmd_cb_t	myudb_hci_cmd_cb = 0;

void myudb_register_hci_cb (void *p)
{
	myudb_hci_cmd_cb = p;
}

#define			MYHCI_FW_DOWNLOAD			0xfe

_attribute_ram_code_sec_noinline_ int myudb_mem_cmd (unsigned char *p, int nbyte)
{
	int len = nbyte;
	int cmd = p[0];
	unsigned char	rsp[280];

	int ret = 0;

	//////////////////////////  Memory Read ////////////////////////////////////
	if (cmd == 0x28 && len >= 8)
	{
		usb_send_status_pkt (0x81, 8, p, 12);
		rsp[0] = 0x29;
		memcpy (rsp + 1, p + 1, 5);
		int type = p[1];
		unsigned int adr = p[2] | (p[3] << 8) | (p[4] << 16) | (p[5] << 24);
		int n = p[6] | (p[7] << 8);
		if (n > 256)
		{
			n = 256;
		}

		if (type == 0)
		{
			memcpy (rsp + 6, (void *)adr, n);
		}
		else if (type == 1)
		{
			for (int i=0; i<n; i++)
			{
				rsp[i + 6] = analog_read_reg8 (adr + i);
			}
		}
		else if (type == 2 || type == 3)					//flash
		{
			flash_read_page (adr, n, rsp + 6);
		}

		usb_send_status_pkt (0x82, 8, rsp, n + 6);
	}
	//////////////////////////  Memory Write ////////////////////////////////////
	else if (cmd == 0x2a && len > 6)
	{
		usb_send_status_pkt (0x81, 8, p, 12);
		rsp[0] = 0x2b;
		memcpy (rsp + 1, p + 1, 16);
		unsigned char type = p[1];
		unsigned int adr = p[2] | (p[3] << 8) | (p[4] << 16) | (p[5] << 24);
		int n = len - 6;

		if (type == 0)				//RAM
		{
			memcpy ((void *)adr, p + 6, n);
		}
		else if (type == 1)			//Analog Register
		{
			for (int i=0; i<n; i++)
			{
				analog_write_reg8(adr + i, p[i + 6]);
			}
		}
		else if (type == 2)			// flash
		{
			flash_write_page (adr, n, p + 6);
		}
		else if (type == 3)
		{
			int nb = p[6];
			if (n > 7)
				nb += p[7] << 8;
			if (n > 8)
				nb += p[8] << 16;
			if (n > 9)
				nb += p[9] << 24;

			if (nb)
			{
				for (int i=0; i<nb; i+=4096)
				{
					flash_erase_sector (adr + i);
				}
			}
			else
			{
//				flash_erase_chip ();
				unsigned int flash_mid = flash_read_mid();
				flash_mid >>= 16;
				flash_mid &= 0xff;
				nb = (1 << (flash_mid+6));
				for (int i=0; i<nb; i+=4096)
				{
					flash_erase_sector (adr + i);
				}
			}
		}
		else if (type == MYHCI_FW_DOWNLOAD)
		{
			core_interrupt_disable();
			ret = MYHCI_FW_DOWNLOAD;
		}
		usb_send_status_pkt (0x82, 8, rsp, 14);
	}
	else
	{
		ret = 1;
	}
	return ret;
}

unsigned char buff[320];
_attribute_ram_code_sec_noinline_ int myudb_hci_cmd_from_usb (void)
{
	int fw_download = 0;
	do
	{
		int n = myudb_usb_get_packet (buff);
		if (n)
		{
			int r = myudb_mem_cmd(buff, n);

			if (r == MYHCI_FW_DOWNLOAD)
			{
				fw_download =  MYHCI_FW_DOWNLOAD;
			}
			else if (r && myudb_hci_cmd_cb)
			{

				myudb_hci_cmd_cb (buff, n);
			}
		}
		if (fw_download)
		{
			myudb_to_usb ();
		}
	} while (fw_download);

	return 0;
}

/////////////////////////////////////////////////////////////////////////
void myudb_usb_handle_irq(void) {
	static unsigned int tick_sync = 0;
	if(1){		//  do nothing when in suspend. Maybe not unnecessary
		unsigned int irq = usbhw_get_ctrl_ep_irq();
		if (irq & FLD_CTRL_EP_IRQ_SETUP) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_SETUP);
			myudb_usb_handle_ctl_ep_setup();
			if (!tick_sync)
			{
				tick_sync = stimer_get_tick () | 1;
			}
		}
		if (irq & FLD_CTRL_EP_IRQ_DATA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_DATA);
			myudb_usb_handle_ctl_ep_data();
		}
		if (irq & FLD_CTRL_EP_IRQ_STA) {
			usbhw_clr_ctrl_ep_irq(FLD_CTRL_EP_IRQ_STA);
			myudb_usb_handle_ctl_ep_status();
		}

		if (usbhw_get_irq_status(USB_IRQ_RESET_STATUS))
		{
			usbhw_clr_irq_status(USB_IRQ_RESET_STATUS); 		//Clear USB reset flag
			myudb_usb_bulkout_ready ();
	    }

		myudb_g_stall = 0;
	}

	myudb_to_usb ();

	myudb_hci_cmd_from_usb ();

	if (tick_sync && clock_time_exceed (tick_sync, 10000))
	{
		tick_sync = stimer_get_tick () | 1;
		log_sync (SL_STACK_EN);
	}

}

void myudb_usb_bulkout_ready (void)
{
	reg_usb_ep_ctrl (MYUDB_EDP_OUT_HCI) = FLD_EP_DAT_ACK;
}

void myudb_usb_init(unsigned short id, void *p)
{
	if (!myudb_fifo && p)
	{
		myudb_fifo = p;
	}
	myudb_id = id;
	//reg_usb_mask = BIT(7);			//audio in interrupt enable
	//reg_irq_mask |= FLD_IRQ_IRQ4_EN;
	reg_usb_ep_max_size = (128 >> 2);
	reg_usb_ep8_send_thres = 0x40;
	reg_usb_ep_buf_addr (MYUDB_EDP_IN_HCI) = 128;
	reg_usb_ep_buf_addr (MYUDB_EDP_OUT_HCI) = 192;
	reg_usb_ep_buf_addr (MYUDB_EDP_IN_VCD) = 0;
	reg_usb_ep8_fifo_mode = 1;
	reg_usb_mdev &= ~BIT(3);			//vendor command: bRequest[7] = 0

	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC);

	myudb_usb_bulkout_ready ();
}

#endif


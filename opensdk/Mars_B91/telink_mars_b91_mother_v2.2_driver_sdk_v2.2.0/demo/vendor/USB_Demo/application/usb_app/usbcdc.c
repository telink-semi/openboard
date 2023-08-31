/********************************************************************************************************
 * @file    usbcdc.c
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
#include "usbcdc.h"
#if(USB_CDC_ENABLE)
#include "../usbstd/usb.h"

unsigned char usb_cdc_data[CDC_TXRX_EPSIZE];
unsigned short usb_cdc_data_len;
unsigned int usb_cdc_tx_cnt;
unsigned char LineCoding[7]={0x00,0xC2,0x01,0x00,0x00,0x00,0x08};
unsigned char g_cdc_length=0;

void usb_cdc_tx_data_to_host(unsigned char * data_ptr, unsigned short data_len)
{

	if(usbhw_is_ep_busy(USB_EDP_CDC_IN)){
		return;
	}
	/*If the length of the data sent is equal to the wMaxPacketSize (CDC_TXRX_EPSIZE),
			 the device must return a zero-length packet to indicate the end of the data stage,
			 The following is the process of sending zero-length packet*/
	if(data_len >CDC_TXRX_EPSIZE) {
		data_len = CDC_TXRX_EPSIZE;
	}
	g_cdc_length=data_len;
	usbhw_reset_ep_ptr(USB_EDP_CDC_IN);

	while(data_len-- > 0) {
		reg_usb_ep_dat(USB_EDP_CDC_IN) = (*data_ptr);

		++data_ptr;
	}

	usbhw_data_ep_ack(USB_EDP_CDC_IN);
	/*If the length of the data sent is equal to the wMaxPacketSize (CDC_TXRX_EPSIZE),
	 the device must return a zero-length packet to indicate the end of the data stage,
	 The following is the process of sending zero-length packet*/
	if(g_cdc_length%CDC_TXRX_EPSIZE==0)
	{
		delay_us(64);//delay for the actual data transfer to complete
		usbhw_reset_ep_ptr(USB_EDP_CDC_IN);
		usbhw_data_ep_ack(USB_EDP_CDC_IN);
	}
	//printf(" ACK \r\n");
}

void usb_cdc_rx_data_from_host(unsigned char* rx_buff)
{
	//receive data from usb and send them to uart
	unsigned char rx_from_usbhost_len = reg_usb_ep_ptr(USB_EDP_CDC_OUT);
	usbhw_reset_ep_ptr(USB_EDP_CDC_OUT);


	if(rx_from_usbhost_len > 0 && rx_from_usbhost_len <= CDC_TXRX_EPSIZE){


		for(int i = 0; i < rx_from_usbhost_len; ++i)
		{
			rx_buff[i] = reg_usb_ep_dat(USB_EDP_CDC_OUT);

		}
		usb_cdc_data_len = rx_from_usbhost_len&0xff;		//uart dma: first 4 bytes is the length of packet


	}
}


   #endif

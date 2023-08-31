/********************************************************************************************************
 * @file    myudb.h
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
#ifndef		__MYUDB_H__
#define		__MYUDB_H__
#pragma once

#include "log_def_stack.h"

#define			SL_STACK_EN				1
#define			SL_BASE_EN				1
#define			SL_SNIFF_EN				0
#define			SL_TIMEOUT_EN			0

#define my_dump_str_data(en,s,p,n)		if(en){usb_send_str_data(s,(unsigned char*)(p),n);}


typedef int (*func_myudb_hci_cmd_cb_t) (unsigned char *, int);

void 	myudb_register_hci_cb (void *p);

void 	myudb_usb_init(unsigned short id, void *p);

void 	myudb_usb_bulkout_ready ();

void 	myudb_usb_handle_irq(void);

int 	myudb_usb_get_packet (unsigned char *p);

void 	usb_send_status_pkt(unsigned char status, unsigned char buffer_num, unsigned char *pkt, unsigned short len);

void 	myudb_capture_enable (int en);

void 	usb_send_str_data (char *str, unsigned char *ph, int n);

void	myudb_set_txfifo_local ();

void	myudb_set_txfifo (void *p);

#define			my_irq_disable()		unsigned int rie = core_interrupt_disable ()
#define			my_irq_restore()		core_restore_interrupt(rie)
#define			LOG_EVENT_TIMESTAMP		0
#define			LOG_DATA_B1_0			0
#define			LOG_DATA_B1_1			1

//#define			get_systemtick()  	    (stimer_get_tick()*3/2)
#define			get_systemtick()  	    stimer_get_tick()

//#define			log_uart(d)				uart_send_byte_dma(0,d)
#define			log_uart(d)				reg_usb_ep8_dat=d

#define         DEBUG_PORT				GPIO_PB2
#define			log_ref_gpio_h()		gpio_set_high_level(DEBUG_PORT)
#define			log_ref_gpio_l()		gpio_set_low_level(DEBUG_PORT)


#define	log_hw_ref()	{my_irq_disable();log_ref_gpio_h();log_uart(0x20);int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);log_ref_gpio_l();my_irq_restore();}

// 4-byte sync word: 00 00 00 00
#define	log_sync(en)	if(en) {my_irq_disable();log_uart(0);log_uart(0);log_uart(0);log_uart(0);my_irq_restore();}
//4-byte (001_id-5bits) id0: timestamp align with hardware gpio output; id1-31: user define
#define	log_tick(en,id)	if(en) {my_irq_disable();log_uart(0x20|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);my_irq_restore();}

//1-byte (000_id-5bits)
#define	log_event(en,id) if(en) {my_irq_disable();log_uart(0x00|(id&31));my_irq_restore();}

//1-byte (01x_id-5bits) 1-bit data: id0 & id1 reserved for hardware
#define	log_task(en,id,b)	if(en) {my_irq_disable();log_uart(((b)?0x60:0x40)|(id&31));int t=get_systemtick();log_uart(t);log_uart(t>>8);log_uart(t>>16);my_irq_restore();}

//2-byte (10-id-6bits) 8-bit data
#define	log_b8(en,id,d)	if(en) {my_irq_disable();log_uart(0x80|(id&63));log_uart(d);my_irq_restore();}

//3-byte (11-id-6bits) 16-bit data
#define	log_b16(en,id,d) if(en) {my_irq_disable();log_uart(0xc0|(id&63));log_uart(d);log_uart((d)>>8);my_irq_restore();}


#endif

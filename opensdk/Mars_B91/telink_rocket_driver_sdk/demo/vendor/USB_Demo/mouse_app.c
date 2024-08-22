/********************************************************************************************************
 * @file    mouse_app.c
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
#include "app_config.h"
#if(USB_DEMO_TYPE==USB_MOUSE)
#include "application/usbstd/usb.h"
#include "application/usb_app/usbmouse.h"
char  mouse[4];

#include "device/oled.h"
#include "device/sht30.h"
#include "device/lis2dh.h"
#include "device/button.h"
#include "ws2812b.h"
#include "device/serial_port.h"
#include "device/photoresistor.h"
#include "printf.h"
volatile unsigned int  g_time0;

int putchar(int ch)
{
	uart_send_byte(UART0, ch);
	uart_rtx_pin_tx_trig(UART0);
    return ch;
}

void user_init()
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_init_interrupt();
	//3.enable global interrupt
	core_interrupt_enable();
	usbhw_set_irq_mask(USB_IRQ_RESET_MASK|USB_IRQ_SUSPEND_MASK);
	//initiate Button for Mouse input
	init_button();
	// binding_button();
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
    i2c_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);
    i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));
	OLED_Init();
	OLED_Show_Arrow(0);
	Lis2dh_Init();
	ws2812b_init2();
	serial_port_init(1000000);
	photoresistor_init();
	core_interrupt_enable();
	g_time0 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		// uart_rec_handle(uart_send_array);
		if(clock_time_exceed(g_time0,500*1000)){
			sht3x_test();
			get_res_value();
			get_acc_value(&g_axis_info);
			gpio_toggle(LED);
			ws2812b_test2();
			printf("====================\r\n\r\n");
			g_time0 = stimer_get_tick();
		}
		/* enum to USB input device and simulate the left click and right click of mouse */
		usb_handle_irq();
		if(g_usb_config != 0 )
		{
			if(gpio_get_level(BUTTON_LFT)==0)
			{
				delay_us(10000);
				if(gpio_get_level(BUTTON_LFT)==0)
				{
					while(gpio_get_level(BUTTON_LFT)==0);
					gpio_set_high_level(LED);
					//printf("Key:Mouse  Click ! \r\n");
					mouse[0] = BIT(1);// BIT(0) - left key; BIT(1) - right key; BIT(2) - middle key; BIT(3) - side key; BIT(4) - external key
					mouse[1] = -20;	  // Displacement relative to x coordinate
					mouse[2] = 20;	  // Displacement relative to y coordinate
					mouse[3] = 0;     // Displacement relative to the roller
					usbmouse_hid_report(USB_HID_MOUSE,(unsigned char*)mouse,4);
				}
			}

			if(gpio_get_level(BUTTON_RHT)==0)
			{
				delay_us(10000);
				if(gpio_get_level(BUTTON_RHT)==0)
				{
					while(gpio_get_level(BUTTON_RHT)==0);
					gpio_set_high_level(LED);
					//printf("Key:Mouse  Click ! \r\n");
					mouse[0] = BIT(1);// BIT(0) - left key; BIT(1) - right key; BIT(2) - middle key; BIT(3) - side key; BIT(4) - external key
					mouse[1] = 20;	  // Displacement relative to x coordinate
					mouse[2] = -20;	  // Displacement relative to y coordinate
					mouse[3] = 0;     // Displacement relative to the roller
					usbmouse_hid_report(USB_HID_MOUSE,(unsigned char*)mouse,4);
				}
			}

			if(gpio_get_level(BUTTON_MID)==0)
			{
				delay_us(10000);
				if(gpio_get_level(BUTTON_MID)==0)
				{
					while(gpio_get_level(BUTTON_MID)==0);
					gpio_set_low_level(LED);
					//printf("Key:release \r\n");
					mouse[0] = 0;
					mouse[1] = 0;
					mouse[2] = 0;
					mouse[3] = 0;
					usbmouse_hid_report(USB_HID_MOUSE,(unsigned char*)mouse,4);
				}
			}
		}
	}
}

#endif

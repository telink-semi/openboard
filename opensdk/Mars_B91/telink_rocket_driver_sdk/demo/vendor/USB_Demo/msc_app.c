/********************************************************************************************************
 * @file    msc_app.c
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
#if(MCU_CORE_B91)
#if((USB_DEMO_TYPE==USB_MASS_STORAGE))
#include "usb_default.h"
#include "application/usbstd/usb.h"
#include "../3rd-party/tiny_usb/tusb.h"
#include "application/mass_storage/spi_sdnand_driver/sdcard.h"

extern void dcd_int_handler(unsigned char rhport);
_attribute_ram_code_sec_ void  usb_endpoint_irq_handler (void)
{
	dcd_int_handler(0);
}
PLIC_ISR_REGISTER(usb_endpoint_irq_handler, IRQ_USB_ENDPOINT)

extern	 unsigned int  sdcardblocknum;

void user_init(void)
{
#if (SD_NAND_FLASH_SUPPORT)
			while(sd_nand_flash_init())
			{
				delay_ms(500);
			}
			sdcardblocknum=SD_GetSectorCount();
#endif


			tusb_init();
			usb_init_interrupt();
			reg_usb_ep_max_size = 8;

			reg_usb_ep_buf_addr(USB_EDP_MS_IN)=0X00;
			reg_usb_ep_buf_addr(USB_EDP_MS_OUT)=0X40;
			usbhw_set_eps_irq_mask(BIT(USB_EDP_MS_IN)|BIT(USB_EDP_MS_OUT));

			usbhw_set_irq_mask(USB_IRQ_RESET_MASK|USB_IRQ_SUSPEND_MASK);
			usb_set_pin_en();
			plic_interrupt_enable(IRQ_USB_ENDPOINT);
			core_interrupt_enable();


}
void main_loop (void)
{
	usb_handle_irq();
	tud_task();
}

#endif
#endif

/********************************************************************************************************
 * @file     common_dbg.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 *******************************************************************************************************/

#include "tl_common.h"
#include "common_dbg.h"


#if (DUMP_STR_EN || APP_DUMP_EN)

MYFIFO_INIT_IRAM(print_fifo, 288, 32);



void app_dmup_debug_init(void)
{
	my_usb_init(0x120, &print_fifo);
	usb_set_pin_en ();
}

#endif





#if (EXCEPT_HANDLE_DEBUG_ENABLE)

volatile u32 dbg_except_handler = 0;

volatile u32 g_mcause;
volatile u32 g_mepc;
volatile u32 g_mtval;
volatile u32 g_mdcause;
volatile u32 g_mscratch;
/**
 * @brief		BLE SDK System except handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void except_handler()
{
	gpio_write(GPIO_LED_WHITE, LED_ON_LEVAL);

	g_mcause = read_csr(NDS_MCAUSE);
	g_mepc = read_csr(NDS_MEPC);
	g_mtval = read_csr(NDS_MTVAL);
	g_mdcause = read_csr(NDS_MDCAUSE);
	g_mscratch = read_csr(NDS_MSCRATCH);
	#if 0		//Enable this if you need to read values of the exception by BDT.
		while(1){
			printf("enter except_handler.\r\n");

			for(volatile unsigned int i = 0; i < 0xffff; i++)
			{
				asm("nop");
				dbg_except_handler++;
			}
		}
	#else		//If the exception may occur and the system need to be rebooted , enable this.
		start_reboot(); //reboot the MCU
	#endif
}
#endif

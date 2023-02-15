/********************************************************************************************************
 * @file     main.c
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
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

#if (FEATURE_TEST_MODE == TEST_BLE_PHY)

extern hci_fifo_t bltHci_rxfifo;
extern hci_fifo_t bltHci_txfifo;

volatile u32 test1,test2,test3,test4,test5,test6,test7,test8,test9;
volatile u8* test_p1;

/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN14_LOW;
}



/**
 * @brief		BLE SDK System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN15_LOW;
}

/**
 * @brief      UART0 irq function
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_
void uart0_irq_handler(void)
{
	u32 rev_data_len = 0;
	if(uart_get_irq_status(UART0,UART_TXDONE))
	{
	    uart_clr_tx_done(UART0);
	}
    if(uart_get_irq_status(UART0,UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	/************************get the length of receive data****************************/
		rev_data_len = uart_get_dma_rev_data_len(UART0,DMA2);
    	/************************clr rx_irq****************************/
    	uart_clr_irq_status(UART0,UART_CLR_RX);
		if(rev_data_len!=0)
		{
			u8* p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & (bltHci_rxfifo.num-1)) * bltHci_rxfifo.size;
			p[0] = rev_data_len;
			bltHci_rxfifo.wptr++;
			p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & (bltHci_rxfifo.num-1)) * bltHci_rxfifo.size;
			uart_receive_dma_set(DMA2, (unsigned char*)(p+4),UART_RX_BUFFER_SIZE-4);
		}

    	if((uart_get_irq_status(UART0,UART_RX_ERR)))
    	{
    		uart_clr_irq_status(UART0,UART_RX_ERR);
    	}
    }
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)   //must on ramcode
{
	DBG_CHN0_LOW;
	blc_pm_select_internal_32k_crystal();

	sys_init(DCDC_1P4_DCDC_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6);

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_16M_HCLK_16M_PCLK_16M;

	rf_drv_ble_init();

//	gpio_init(!deepRetWakeUp);

	if(!deepRetWakeUp){//read flash size
		blc_readFlashSize_autoConfigCustomFlashSector();
	}

	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value

	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}

	test1 = 0;
	while (1) {
		test1 ++;
		main_loop ();
	}
	return 0;
}


#endif  //end of (FEATURE_TEST_MODE == ...)

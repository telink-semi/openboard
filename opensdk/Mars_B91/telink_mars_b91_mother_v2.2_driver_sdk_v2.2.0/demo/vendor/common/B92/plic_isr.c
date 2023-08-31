/********************************************************************************************************
 * @file    plic_isr.c
 *
 * @brief   This is the source file for B92
 *
 * @author  Driver Group
 * @date    2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/*
 * All interrupt entry functions and weak definitions of related processing functions are defined here,If you dont use the relevant interrupt function,
 *  you can delete it to save ram_code space.
 ****************************************************************************************
 */
#include "lib/include/plic.h"

/**
 * @brief     Default irq handler.
 * @return      none
 */
 __attribute__((section(".ram_code"))) void default_irq_handler(void)
{

}

void stimer_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void analog_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void dma_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void bmc_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setup_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_data_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_status_irq_handler(void)  __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setinf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_endpoint_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void rf_bt_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void pwm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pke_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void audio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void i2c_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void lspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_pwdn_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void soft_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void mspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void usb_reset_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_250us_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void qdec_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void gpio_src0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src2_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src3_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src4_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src5_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src6_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_src7_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void pm_wkup_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void dpr_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

/**
 * @brief System timer interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq1(void)
{
	plic_isr(stimer_irq_handler,IRQ1_SYSTIMER);
}

/**
 * @brief Analog register master interface interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq2(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq2(void)
{
	plic_isr(analog_irq_handler,IRQ2_ALG);
}


/**
 * @brief Timer1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	plic_isr(timer1_irq_handler,IRQ3_TIMER1);
}

/**
 * @brief Timer0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq4(void)
{
	plic_isr(timer0_irq_handler,IRQ4_TIMER0);
}


/**
 * @brief DMA interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq5(void)
{
	plic_isr(dma_irq_handler,IRQ5_DMA);
}

/**
 * @brief BMC AHB bus matrix controller interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq6(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq6(void)
{
	plic_isr(bmc_irq_handler,IRQ6_BMC);
}


/**
 * @brief USB control endpoint setup interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq7(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq7(void)
{
	plic_isr(usb_ctrl_ep_setup_irq_handler,IRQ7_USB_CTRL_EP_SETUP);
}

/**
 * @brief USB control endpoint data interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq8(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq8(void)
{
	plic_isr(usb_ctrl_ep_data_irq_handler,IRQ8_USB_CTRL_EP_DATA);
}

/**
 * @brief USB control endpoint status interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq9(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq9(void)
{
	plic_isr(usb_ctrl_ep_status_irq_handler,IRQ9_USB_CTRL_EP_STATUS);
}


/**
 * @brief USB control endpoint setinf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq10(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq10(void)
{
	plic_isr(usb_ctrl_ep_setinf_irq_handler,IRQ10_USB_CTRL_EP_SETINF);
}

/**
 * @brief USB endpoint (1-8) interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq11(void)
{
	plic_isr(usb_endpoint_irq_handler,IRQ11_USB_ENDPOINT);
}

/**
 * @brief BR/EDR sub-system interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq14(void)
{
	plic_isr(rf_bt_irq_handler,IRQ14_ZB_BT);
}

/**
 * @brief BLE (TL) sub-system interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq15(void)
{
	plic_isr(rf_irq_handler,IRQ15_ZB_RT);
}



/**
 * @brief PWM interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq16(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq16(void)
{
	plic_isr(pwm_irq_handler,IRQ16_PWM);
}

/**
 * @brief PKE interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq17(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq17(void)
{
	plic_isr(pke_irq_handler,IRQ17_PKE);
}



/**
 * @brief UART1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq18(void)
{
	plic_isr(uart1_irq_handler,IRQ18_UART1);
}



/**
 * @brief UART0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq19(void)
{
	plic_isr(uart0_irq_handler,IRQ19_UART0);
}


/**
 * @brief Audio DMA FIFO interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq20(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq20(void)
{
	plic_isr(audio_irq_handler,IRQ20_DFIFO);
}

/**
 * @brief I2C interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq21(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq21(void)
{
	plic_isr(i2c_irq_handler,IRQ21_I2C);
}


/**
 * @brief LSPI interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq22(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq22(void)
{
	plic_isr(lspi_irq_handler,IRQ22_LSPI);
}


/**
 * @brief GSPI interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq23(void)
{
	plic_isr(gspi_irq_handler,IRQ23_GSPI);

}

/**
 * @brief USB suspend interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq24(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq24(void)
{
	plic_isr(usb_pwdn_irq_handler,IRQ24_USB_PWDN);
}

/**
 * @brief GPIO interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq25(void)
{
	plic_isr(gpio_irq_handler,IRQ25_GPIO);
}

/**
 * @brief GPIO2RISC0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq26(void)
{
	plic_isr(gpio_risc0_irq_handler,IRQ26_GPIO2RISC0);
}


/**
 * @brief GPIO2RISC1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq27(void)
{
	plic_isr(gpio_risc1_irq_handler,IRQ27_GPIO2RISC1);
}

/**
 * @brief Soft interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq28(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq28(void)
{
	plic_isr(soft_irq_handler,IRQ28_SOFT);
}

/**
 * @brief MSPI interrupt handler.
 * @return none
 */

_attribute_ram_code_sec_noinline_ void  entry_irq29(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq29(void)
{
	plic_isr(mspi_irq_handler,IRQ29_MSPI);
}
/**
 * @brief USB reset interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq30(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq30(void)
{
	plic_isr(usb_reset_irq_handler,IRQ30_USB_RESET);
}
/**
 * @brief USB 250us or sof interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq31(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq31(void)
{
	plic_isr(usb_250us_irq_handler,IRQ31_USB_250US);
}

/**
 * @brief QDEC interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq33(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq33(void)
{
	plic_isr(qdec_irq_handler,IRQ33_QDEC);
}

/**
 * @brief GPIO group0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq34(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq34(void)
{
	plic_isr(gpio_src0_irq_handler,IRQ34_GPIO_SRC0);
}

/**
 * @brief GPIO group1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq35(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq35(void)
{
	plic_isr(gpio_src1_irq_handler,IRQ35_GPIO_SRC1);
}

/**
 * @brief GPIO group2 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq36(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq36(void)
{
	plic_isr(gpio_src2_irq_handler,IRQ36_GPIO_SRC2);
}

/**
 * @brief GPIO group3 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq37(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq37(void)
{
	plic_isr(gpio_src3_irq_handler,IRQ37_GPIO_SRC3);
}

/**
 * @brief GPIO group4 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq38(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq38(void)
{
	plic_isr(gpio_src4_irq_handler,IRQ38_GPIO_SRC4);
}

/**
 * @brief GPIO group5 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq39(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq39(void)
{
	plic_isr(gpio_src5_irq_handler,IRQ39_GPIO_SRC5);
}

/**
 * @brief GPIO group6 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq40(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq40(void)
{
	plic_isr(gpio_src6_irq_handler,IRQ40_GPIO_SRC6);
}

/**
 * @brief GPIO group7 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq41(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq41(void)
{
	plic_isr(gpio_src7_irq_handler,IRQ41_GPIO_SRC7);
}

/**
 * @brief PM wakeup interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq44(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq44(void)
{
	plic_isr(pm_wkup_irq_handler,IRQ44_PM_WKUP);
}
/**
 * @brief PM mixed interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq45(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq45(void)
{
	plic_isr(pm_irq_handler,IRQ45_PM_IRQ);
}
/**
 * @brief DPR interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq46(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq46(void)
{
	plic_isr(dpr_irq_handler,IRQ46_DPR_IRQ);
}


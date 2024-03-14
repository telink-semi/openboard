/********************************************************************************************************
 * @file    app_bqb.c
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
#include "BQB/bqb.h"

#if(TEST_DEMO==BQB_DEMO)

#if SUPPORT_CONFIGURATION
static unsigned char read_config(unsigned long addr)
{
	unsigned char tmp;
#if MCU_STARTUP_FLASH
	flash_read_page(addr, 1, &tmp);
#else
	tmp = read_sram8(addr);
#endif
	return tmp;
}

//volatile unsigned char flash_mid;
void rd_usr_definition(unsigned char _s)
{
	if(_s)
	{
		usr_config.uart_tx = read_config(CONFIGURATION_ADDR_UART_TX);
		usr_config.uart_rx = read_config(CONFIGURATION_ADDR_UART_RX);
		usr_config.pa_tx = read_config(CONFIGURATION_ADDR_PA_TX);
		usr_config.pa_rx = read_config(CONFIGURATION_ADDR_PA_RX);
		unsigned char tmp = read_config(CONFIGURATION_ADDR_CAP);
		usr_config.cap = (tmp >> 7) & 0x01;
		usr_config.cal_pos = tmp & 0x03;
		usr_config.power_mode = (tmp >> 5) & 0x03;
		usr_config.power = read_config(CONFIGURATION_ADDR_POWER);
	}
	else
	{
		usr_config.flash = (flash_read_mid() >> 16) & 0xff;
	}
}
#if(MCU_CORE_B91)
void get_uart_port(uart_tx_pin_e* bqb_uart_tx_port, uart_rx_pin_e* bqb_uart_rx_port)
{
	if(usr_config.uart_tx == usr_config.uart_rx) return;
	*bqb_uart_tx_port = get_pin(usr_config.uart_tx);
	*bqb_uart_rx_port = get_pin(usr_config.uart_rx);
}
#elif(MCU_CORE_B92)
void get_uart_port(gpio_func_pin_e* bqb_uart_tx_port, gpio_func_pin_e* bqb_uart_rx_port)
{
	if(usr_config.uart_tx == usr_config.uart_rx) return;
	*bqb_uart_tx_port = get_pin(usr_config.uart_tx);
	*bqb_uart_rx_port = get_pin(usr_config.uart_rx);
}
#endif
#endif

#if	(MCU_CORE_B91)
uart_num_redef_e get_uart_num(uart_tx_pin_e tx_pin,uart_rx_pin_e rx_pin)
{
	uart_num_redef_e tx_flag = UART_NONE, rx_flag = UART_NONE;
	if(tx_pin == UART0_TX_PA3 || tx_pin == UART0_TX_PB2 || tx_pin == UART0_TX_PD2)
	{
		tx_flag = UART_NUM0;
	}
	else if(tx_pin == UART1_TX_PC6 || tx_pin == UART1_TX_PD6 || tx_pin == UART1_TX_PE0)
	{
		tx_flag = UART_NUM1;
	}
	else
	{
		return UART_NONE;
	}

	if(rx_pin == UART0_RX_PA4 || rx_pin == UART0_RX_PB3 || rx_pin == UART0_RX_PD3)
	{
		rx_flag = UART_NUM0;
	}
	else if(rx_pin == UART1_RX_PC7 || rx_pin == UART1_RX_PD7 || rx_pin == UART1_RX_PE2)
	{
		rx_flag = UART_NUM1;
	}
	else
	{
		return UART_NONE;
	}

	if(rx_flag == tx_flag)
	{
		return rx_flag;
	}
	else
	{
		return UART_NONE;
	}
}

uart_num_redef_e uart_setup(uart_tx_pin_e tx_pin,uart_rx_pin_e rx_pin,
			unsigned int baudrate, unsigned int pclk,
			uart_parity_e parity, uart_stop_bit_e stop_bit)
{
	unsigned short div = 0;
	unsigned char bwpc = 0;
	uart_num_redef_e uart_num = UART_NONE;
	uart_num = get_uart_num(tx_pin, rx_pin);
	if(uart_num == UART_NONE)
	{
		return UART_NONE;
	}
	uart_set_pin(tx_pin, rx_pin);
	uart_reset(uart_num);
	uart_cal_div_and_bwpc(baudrate, pclk, &div, &bwpc);
	uart_set_rx_timeout(uart_num, bwpc, 12, UART_BW_MUL1);
	uart_init(uart_num, div, bwpc, parity, stop_bit);

	return uart_num;
}
#elif(MCU_CORE_B92)
uart_num_redef_e uart_setup(gpio_func_pin_e tx_pin,gpio_func_pin_e rx_pin,
			unsigned int baudrate, unsigned int pclk,
			uart_parity_e parity, uart_stop_bit_e stop_bit)
{
	unsigned short div = 0;
	unsigned char bwpc = 0;
	uart_num_redef_e uart_num = UART_NUM0;
	uart_set_pin((uart_num_e)uart_num,tx_pin, rx_pin);
	uart_reset((uart_num_e)uart_num);
	uart_cal_div_and_bwpc(baudrate, pclk, &div, &bwpc);
	uart_set_rx_timeout((uart_num_e)uart_num, bwpc, 12, UART_BW_MUL1);
	uart_init((uart_num_e)uart_num, div, bwpc, parity, stop_bit);

	return uart_num;
}
#endif

void read_bqb_calibration()
{
	unsigned char chnidx=0xff;
#if SUPPORT_CONFIGURATION
	if(usr_config.cap)
	{
		rf_turn_off_internal_cap();
	}
	else
	{
		if(usr_config.cal_pos == 1)
		{
			//OTP:need to be added.
		}
		if(usr_config.cal_pos == 2)
		{
			chnidx = read_sram8(CALIBRATION_CAP_RAM_ADDR);
		}
		else
		{
			//FLASH
			if(usr_config.flash == FLASH_SIZE_4M)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_4M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_2M)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_2M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_1M)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_1M, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_512K)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_512K, 1, &chnidx);
			}
			else if(usr_config.flash == FLASH_SIZE_128K || usr_config.flash == FLASH_SIZE_256K)
			{
				flash_read_page(CAP_SET_FLASH_ADDR_128K, 1, &chnidx);
			}
			else
			{
				flash_read_page(CAP_SET_FLASH_ADDR_64K, 1, &chnidx);
			}
		}

		if(chnidx!=0xff)
		{
			rf_update_internal_cap(chnidx);
		}
	}
#else
	#if SWITCH_INTERNAL_CAP
		#if SWITCH_CALIBRATION_POSITION
			flash_read_page(CAP_SET_FLASH_ADDR, 1, &chnidx);
		#else
			chnidx = read_sram8(CALIBRATION_CAP_RAM_ADDR);
		#endif

		if(chnidx!=0xff)
		{
			rf_update_internal_cap(chnidx);
		}

	#else
		rf_turn_off_internal_cap();
	#endif
#endif
}

void user_init(void)
{
#if	(MCU_CORE_B91)
	uart_tx_pin_e bqb_uart_tx_port = BQB_UART_TX_PORT;
	uart_rx_pin_e bqb_uart_rx_port = BQB_UART_RX_PORT;
#elif(MCU_CORE_B92)
	gpio_func_pin_e bqb_uart_tx_port = BQB_UART_TX_PORT;
	gpio_func_pin_e bqb_uart_rx_port = BQB_UART_RX_PORT;
#endif
#if SUPPORT_CONFIGURATION
	if(usr_config.cal_pos == 0)
	{
		rd_usr_definition(0);
	}
	get_uart_port(&bqb_uart_tx_port, &bqb_uart_rx_port);
#endif
	uart_using = uart_setup(bqb_uart_tx_port, bqb_uart_rx_port,
							BQB_UART_BAUD,
							sys_clk.pclk*1000*1000,
							UART_PARITY_NONE, UART_STOP_BIT_ONE);
	uart_set_irq_mask((uart_num_e)uart_using, UART_RX_IRQ_MASK);
	uart_clr_irq_mask((uart_num_e)uart_using, UART_TX_IRQ_MASK);
	uart_rx_irq_trig_level((uart_num_e)uart_using, 1);
	bqb_pa_init();
	bqbtest_init();
	write_sram8(CALIBRATION_CAP_RAM_ADDR, 0xff);//for sram calibration
	read_bqb_calibration();
}

void main_loop(void)
{
	bqb_serviceloop();
}

#endif

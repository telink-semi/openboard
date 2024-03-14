/********************************************************************************************************
 * @file    app_hspi_xip.c
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
#if (SPI_MODE == SPI_XIP_MODE)
/************************* Note **************************************************
 * before this test must down load led bin(LED boot_from_ilm) to sram by BDT.*/

#define SPI_XIP_SERIAL_CMD_READ	                     1
#define SPI_XIP_SINGLE_CMD_FAST_READ                 2
#define SPI_XIP_SINGLE_CMD_FAST_QUAD_READ            3
#define SPI_XIP_QUAD_CMD_FAST_READ                   4
#define SPI_XIP_QUAD_CMD_FAST_QUAD_READ              5
#define SPI_XIP_LOAD_PROGRAM_TO_PSRAM                6

#define SPI_XIP_TEST_MODE        SPI_XIP_SERIAL_CMD_READ

#if (SPI_XIP_TEST_MODE == SPI_XIP_SERIAL_CMD_READ)
#define SPI_CLOCK	    					  	24000000
#else
#define SPI_CLOCK	    					  	48000000
#endif
#define BUFF_BYTE_LEN		 256
volatile unsigned char buff_in_sram[BUFF_BYTE_LEN] = {0};
volatile unsigned char buff_read_from_psram[BUFF_BYTE_LEN] = {0};

hspi_pin_config_t hspi_pin_config = {
	.hspi_clk_pin  		= HSPI_CLK_PB4,
	.hspi_csn_pin  		= HSPI_CSN_PB6,
	.hspi_mosi_io0_pin  = HSPI_MOSI_IO0_PB3,
	.hspi_miso_io1_pin  = HSPI_MISO_IO1_PB2,
	.hspi_wp_io2_pin    = HSPI_WP_IO2_PB1,
	.hspi_hold_io3_pin  = HSPI_HOLD_IO3_PB0,
};

#if (SPI_XIP_TEST_MODE == SPI_XIP_SERIAL_CMD_READ)
hspi_config_t hspi_xip_config = {
	.hspi_io_mode 		= HSPI_SINGLE,
	.hspi_dummy_cnt 	= 0,
	.hspi_cmd_en 		= 1,
	.hspi_addr_en 		= 1,
	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
	.hspi_cmd_fmt_en 	= 0,//when hspi_cmd_en = false,invalid set.
	.hspi_addr_fmt_en 	= 0,//when hspi_addr_en = false,invalid set.
};

_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(2000);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);//max 33MHz for this mode
	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(4);
	hspi_xip_en();

	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		buff_in_sram[i] = i;
	}
	hspi_master_write_xip(PSRAM_WRITE_CMD, 0x00, (unsigned char *)buff_in_sram, BUFF_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	hspi_master_read_xip(PSRAM_READ_CMD, 0x00, (unsigned char *)buff_read_from_psram, BUFF_BYTE_LEN, SPI_MODE_RD_READ_ONLY);//read bin data from psram
	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		if (buff_in_sram[i] != buff_read_from_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
}

#elif (SPI_XIP_TEST_MODE == SPI_XIP_SINGLE_CMD_FAST_READ)
hspi_config_t hspi_xip_config = {
	.hspi_io_mode 		= HSPI_SINGLE,
	.hspi_dummy_cnt 	= 8,
	.hspi_cmd_en 		= 1,
	.hspi_addr_en 		= 1,
	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
	.hspi_cmd_fmt_en 	= 0,//when hspi_cmd_en = false,invalid set.
	.hspi_addr_fmt_en 	= 0,//when hspi_addr_en = false,invalid set.
};
_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(500);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);
	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(5);
	hspi_xip_en();

	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		buff_in_sram[i] = i;
	}
	hspi_master_write_xip(PSRAM_WRITE_CMD, 0x00, (unsigned char *)buff_in_sram, BUFF_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	hspi_master_read_xip(PSRAM_FAST_READ_CMD, 0x00, (unsigned char *)buff_read_from_psram, BUFF_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);//read bin data from psram
	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		if (buff_in_sram[i] != buff_read_from_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
}

#elif (SPI_XIP_TEST_MODE == SPI_XIP_SINGLE_CMD_FAST_QUAD_READ)
hspi_config_t hspi_xip_config = {
	.hspi_io_mode 		= HSPI_SINGLE,
	.hspi_dummy_cnt 	= 6,
	.hspi_cmd_en 		= 1,
	.hspi_addr_en 		= 1,
	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
	.hspi_cmd_fmt_en 	= 0,//when hspi_cmd_en = false,invalid set.
	.hspi_addr_fmt_en 	= 1,//when hspi_addr_en = false,invalid set.
};
_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(500);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);

	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(7);//must
	hspi_xip_en();

	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	hspi_set_quad_mode();

	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		buff_in_sram[i] = i;
	}
	hspi_master_write_xip(PSRAM_QUAD_WRITE_CMD, 0x00, (unsigned char *)buff_in_sram, BUFF_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	hspi_master_read_xip(PSRAM_FAST_READ_QUAD_CMD, 0x00, (unsigned char *)buff_read_from_psram, BUFF_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);//read bin data from psram
	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		if (buff_in_sram[i] != buff_read_from_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
}

#elif (SPI_XIP_TEST_MODE == SPI_XIP_QUAD_CMD_FAST_READ)
hspi_config_t hspi_xip_config = {
 	.hspi_io_mode 		= HSPI_SINGLE,
 	.hspi_dummy_cnt 	= 4,
 	.hspi_cmd_en 		= 1,
 	.hspi_addr_en 		= 1,
 	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
 	.hspi_cmd_fmt_en 	= 1,//when hspi_cmd_en = false,invalid set.
 	.hspi_addr_fmt_en 	= 1,//when hspi_addr_en = false,invalid set.
};
_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(500);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);

	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(7);//must
	hspi_xip_en();

	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	hspi_master_write_xip_cmd_data(PSRAM_ENTER_QUAD_MODE_CMD, 0x000000, 0, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	hspi_set_quad_mode();

	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		buff_in_sram[i] = i;
	}
	hspi_master_write_xip(PSRAM_QUAD_WRITE_CMD, 0x00, (unsigned char *)buff_in_sram, BUFF_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	hspi_master_read_xip(PSRAM_FAST_READ_CMD, 0x00, (unsigned char *)buff_read_from_psram, BUFF_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);//read bin data from psram
	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		if (buff_in_sram[i] != buff_read_from_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
	hspi_master_write_xip_cmd_data(PSRAM_EXIT_QUAD_MODE_CMD, 0x000000, 0, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
}


#elif (SPI_XIP_TEST_MODE == SPI_XIP_QUAD_CMD_FAST_QUAD_READ)
hspi_config_t hspi_xip_config = {
 	.hspi_io_mode 		= HSPI_SINGLE,
 	.hspi_dummy_cnt 	= 6,
 	.hspi_cmd_en 		= 1,
 	.hspi_addr_en 		= 1,
 	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
 	.hspi_cmd_fmt_en 	= 1,//when hspi_cmd_en = false,invalid set.
 	.hspi_addr_fmt_en 	= 1,//when hspi_addr_en = false,invalid set.
};
_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(500);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);

	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(7);//must
	hspi_xip_en();

	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	hspi_master_write_xip_cmd_data(PSRAM_ENTER_QUAD_MODE_CMD, 0x000000, 0, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	hspi_set_quad_mode();

	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		buff_in_sram[i] = i;
	}
	hspi_master_write_xip(PSRAM_QUAD_WRITE_CMD, 0x00, (unsigned char *)buff_in_sram, BUFF_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	hspi_master_read_xip(PSRAM_FAST_READ_QUAD_CMD, 0x00, (unsigned char *)buff_read_from_psram, BUFF_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);//read bin data from psram
	for (unsigned int i = 0; i < BUFF_BYTE_LEN; i++)
	{
		if (buff_in_sram[i] != buff_read_from_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
	hspi_master_write_xip_cmd_data(PSRAM_EXIT_QUAD_MODE_CMD, 0x000000, 0, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
}
#elif (SPI_XIP_TEST_MODE == SPI_XIP_LOAD_PROGRAM_TO_PSRAM)
#define LED_PROGRAM_BYTE_LEN			0x48 // determined by program size,
unsigned char led_program_in_sram[LED_PROGRAM_BYTE_LEN]={0x37, 0x07, 0x14, 0x80, 0x83, 0x07, 0xe7, 0x30, 0x93, 0xf7, 0xf7, 0x0f, 0x93, 0xe7, 0x07, 0x02,
											  0x23, 0x07, 0xf7, 0x30, 0x83, 0x07, 0xa7, 0x30, 0x93, 0xf7, 0xf7, 0x0d, 0x23, 0x05, 0xf7, 0x30,
											  0x6f, 0x00, 0x00, 0x01, 0x01, 0x00, 0x85, 0x07, 0x37, 0x07, 0x20, 0x00, 0xe3, 0xcc, 0xe7, 0xfe,
											  0x37, 0x07, 0x14, 0x80, 0x83, 0x07, 0xb7, 0x30, 0x93, 0xf7, 0xf7, 0x0f, 0x93, 0xc7, 0x07, 0x02,
											  0xa3, 0x05, 0xf7, 0x30, 0x81, 0x47, 0xcd, 0xb7};
volatile unsigned char led_program_in_psram[LED_PROGRAM_BYTE_LEN]={0};
hspi_config_t hspi_xip_config = {
 	.hspi_io_mode 		= HSPI_SINGLE,
 	.hspi_dummy_cnt 	= 6,
 	.hspi_cmd_en 		= 1,
 	.hspi_addr_en 		= 1,
 	.hspi_addr_len 		= 3,//when hspi_addr_en = false,invalid set.
 	.hspi_cmd_fmt_en 	= 1,//when hspi_cmd_en = false,invalid set.
 	.hspi_addr_fmt_en 	= 1,//when hspi_addr_en = false,invalid set.
};
_attribute_ram_code_sec_noinline_ void user_init()
{
	delay_ms(500);	//must
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_high_level(LED4);              	    //LED1  On
	gpio_set_low_level(LED2);

	/**** 1. initialize spi ******/
	spi_master_init(HSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SPI_CLOCK) - 1, SPI_MODE0);

	hspi_set_pin(&hspi_pin_config);
	hspi_master_config_plus(&hspi_xip_config);

	hspi_xip_seq_mode_en();//must
	//default page boundary size is 32byte, 2^page_size.
	//Use this combined with xip_mode being xip sequential mode
	hspi_xip_page_size(7);//must
	hspi_xip_en();
	/**** 2. initialize psram ******/
	hspi_master_write_xip_cmd_data(PSRAM_REST_ENABLE_CMD, 0x000000, PSRAM_REST_CMD, SPI_MODE_WR_WRITE_ONLY);//reset psram operation
	hspi_master_write_xip_cmd_data(PSRAM_MODE_RG_WRITE_CMD, 0x000000, 0x62, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	hspi_master_write_xip_cmd_data(PSRAM_ENTER_QUAD_MODE_CMD, 0x000000, 0, SPI_MODE_WR_WRITE_ONLY);//set psram mode register.
	while (spi_is_busy(HSPI_MODULE));

	hspi_set_quad_mode();

	/**** 3. write led program to psram ******/
	hspi_master_write_xip(PSRAM_QUAD_WRITE_CMD, 0x00, (unsigned char *)led_program_in_sram, LED_PROGRAM_BYTE_LEN, SPI_MODE_WR_WRITE_ONLY);//write bin data to psram
	/**** 4. read back led program from psram ******/
	hspi_master_read_xip(PSRAM_FAST_READ_QUAD_CMD, 0x00, (unsigned char *)led_program_in_psram, LED_PROGRAM_BYTE_LEN, SPI_MODE_RD_DUMMY_READ);//read bin data from psram
	for (unsigned int i = 0; i < LED_PROGRAM_BYTE_LEN; i++)
	{
		if (led_program_in_sram[i] != led_program_in_psram[i])
		{
			gpio_set_high_level(LED2);
			break;
		}
	}
	/**** 5. run led program in psram ******/
	__asm__("li t0,0x1000000");
	__asm__("jalr t0");
}
#endif

void main_loop(void)
{
	delay_ms(500);
	gpio_toggle(LED4);
}
#endif

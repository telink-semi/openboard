/********************************************************************************************************
 * @file    app_ramless_st77903.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2022
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
 *
 *******************************************************************************************************/
#include "app_config.h"

#if (SPI_DISPLAY_SELECT == SPI_RAMLESS_LCD)

#define RAMLESS_LCD_IVO1_6       0
#define RAMLESS_LCD_BOE1_72      1
#define RAMLESS_LCD_MODE         1


#if(RAMLESS_LCD_MODE ==  RAMLESS_LCD_BOE1_72 )
#include "ramless_st77903_data/ramless_lcd_pic_boe1_72.h"
#include "ramless_st77903_data/image_4bitram_boe1_72.h"
#define RAMLESS_LCD_RESET_PIN	GPIO_PE6
#define RAMLESS_LCD_POWER_ON_PIN    GPIO_PF7
#define RAMLESS_LCD_V_WIDTH	    380
#define RAMLESS_LCD_H_WIDTH		320
#define RAMLESS_LCD_VBP_CNT      6
#define RAMLESS_LCD_VFP_CNT      16
#elif(RAMLESS_LCD_MODE ==  RAMLESS_LCD_IVO1_6 )
#include "ramless_st77903_data/ramless_lcd_pic_ivo1_6.h"
#include "ramless_st77903_data/image_4bitram_ivo1_6.h"
#define RAMLESS_LCD_RESET_PIN	GPIO_PB5
#define RAMLESS_LCD_IM0         GPIO_PB3
#define RAMLESS_LCD_IM1         GPIO_PB4
#define RAMLESS_LCD_V_WIDTH	    400
#define RAMLESS_LCD_H_WIDTH		400
#define RAMLESS_LCD_VBP_CNT      6
#define RAMLESS_LCD_VFP_CNT      6
#endif

//when sending Line,cs should be pulled down at least 35us,so spi_clk should be at the highest 38.4m,otherwise picture jitters will occur.
#define SPI_CLK 				38400000   //The biggest is 48M

/*
 * BOE1.72 : since the screen does not draw out the IM0,IM1 pins,can only be the default value (0,0).
 */
#define	CMD1X_ADDR1X_DATA1X		1//(IM1,IM0) = (0,0)
#define	CMD1X_ADDR1X_DATA2X		2//(IM1,IM0) = (0,0)
#define	CMD1X_ADDR1X_DATA4X		3//(IM1,IM0) = (0,0)
#define	CMD4X_ADDR4X_DATA4X		4//(IM1,IM0) = (0,1)

#define LCD_DATA_LANE_MODE			CMD1X_ADDR1X_DATA4X

lspi_pin_config_t lspi_pin_config = {
	.spi_csn_pin 		= LSPI_CSN_PE0_PIN,
	.spi_clk_pin		= LSPI_CLK_PE1_PIN,
	.spi_mosi_io0_pin   = LSPI_MOSI_IO0_PE2_PIN,
	.spi_miso_io1_pin   = LSPI_MISO_IO1_PE3_PIN,//3line mode set 0
	.spi_io2_pin    	= LSPI_IO2_PE4_PIN,//set quad mode otherwise set 0
	.spi_io3_pin  		= LSPI_IO3_PE5_PIN,//set quad mode otherwise set 0
};

spi_wr_rd_config_t spi_ramless_lcd_protocol = {
	.spi_io_mode		= SPI_QUAD_MODE,
	.spi_dummy_cnt  	= 2,
	.spi_cmd_en 		= 1,
	.spi_addr_en 		= 1,
	.spi_addr_len 		= 3,//when spi_addr_en = 0,invalid set.
#if(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
	.spi_cmd_fmt_en  	= 1,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 1,//when spi_addr_en = 0,invalid set.
#else
	.spi_cmd_fmt_en  	= 0,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 0,//when spi_addr_en = 0,invalid set.
#endif
};
spi_wr_rd_config_t spi_ramless_lcd_protocol_config = {
	.spi_io_mode		= SPI_SINGLE_MODE,
	.spi_dummy_cnt  	= 2,
	.spi_cmd_en 		= 1,
	.spi_addr_en 		= 1,
	.spi_addr_len 		= 3,//when spi_addr_en = 0,invalid set.
#if(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
	.spi_cmd_fmt_en  	= 1,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 1,//when spi_addr_en = 0,invalid set.
#else
	.spi_cmd_fmt_en  	= 0,//when spi_cmd_en = 0,invalid set.
	.spi_addr_fmt_en 	= 0,//when spi_addr_en = 0,invalid set.
#endif
};

#if  RGB_SELECT==RGB565
#define RED    0x00F8
#define GREEN  0xE007
#define BLUE   0x1F00
#define WHITE  0xFFFF
#define BLACK  0x0000

#elif RGB_SELECT==RGB888
#define RED    0x0000FF
#define GREEN  0x00FF00
#define BLUE   0xFF0000
#define WHITE  0xFFFFFF
#define BLACK  0x000000

#elif RGB_SELECT==RGB666
#define RED    0x0000FC
#define GREEN  0x00FC00
#define BLUE   0xFC0000
#define WHITE  0xFCFCFC
#define BLACK  0x000000
#endif

#define LUT_RED    0x01
#define LUT_GREEN  0x02
#define LUT_BLUE   0x0C
#define LUT_WHITE  0x0D
#define LUT_BLACK  0x0F
/**
 * @brief    This function serves to reset ramless lcd.
 * @return   none
 */
void ramless_lcd_reset(void)
{
	gpio_set_low_level(RAMLESS_LCD_RESET_PIN);
	delay_ms(30);//20ms
	gpio_set_high_level(RAMLESS_LCD_RESET_PIN);
	delay_ms(180);//120ms
}

/**
 * @brief    This function serves to write "cmd(0xde) + address + data" to ramless lcd.
 * @param[in] 	addr 	- the operating address of ramless lcd.
 * @param[in] 	data 		-  pointer to the data need to write.
 * @param[in] 	data_len 	- length in byte of the data need to write.
 * @return   none
 */
void ramless_lcd_lspi_master_write_data(unsigned int addr, unsigned char *data, unsigned int data_len)
{
	spi_master_write_plus(LSPI_MODULE, 0xde, addr, data, data_len,SPI_MODE_WR_WRITE_ONLY);
}

/**
 * @brief    This function serves to write  "cmd(0xde) + address" to ramless lcd.
 * @param[in] 	addr 	- the operating address of ramless lcd.
 * @return   none
 */
void ramless_lcd_lspi_master_write_cmd(unsigned int addr)
{
	spi_master_write_plus(LSPI_MODULE,0xde,addr,0,0,SPI_MODE_WR_WRITE_ONLY);
}

/**
 * @brief    This function serves to draw a 1*1 point in ramless lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] y - ordinate of address.
 * @param[in] color_lut - the single color in lut.
 * @param[in] font_addr - the foreground address.
 * @return   none
 */
void ramless_lcd_draw_point(unsigned short x,unsigned short y,unsigned char color_lut,unsigned char *font_addr)
{
	unsigned int offset = (y * RAMLESS_LCD_H_WIDTH + x)>>1;
	font_addr[offset] = (x % 2) ? ((font_addr[offset]&0x0f) | (color_lut<<4)) : ((font_addr[offset]&0xf0) | color_lut);
}

/**
 * @brief    This function serves to fill a rectangular space of ramless lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @param[in] color_lut - the single color in lut.
 * @param[in] font_addr - the foreground address.
 * @return   none
 * @attention  x, x_delta must be multiple of 2.
 */
void ramless_lcd_fill_rectangle(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,unsigned char color_lut,unsigned char *font_addr)
{
	unsigned char color_temp = color_lut + (color_lut<<4);
	for(unsigned short i = y; i < y + y_delta; i++)
		for(unsigned short j = x; j < x + x_delta; j++)
		{
			font_addr[(i * RAMLESS_LCD_H_WIDTH + j)>>1] = color_temp;
		}
}

/**
 * @brief    This function serves to show a rectangular picture in ramless lcd.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of picture.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of picture.
 * @param[in] pic_addr - address of picture buffer.
 * @param[in] font_addr - the foreground address.
 * @return   none
 * @attention x, x_delta must be multiple of 2.
 */
void ramless_lcd_show_picture(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,  unsigned char *pic_addr,unsigned char *font_addr)
{
	unsigned char * ram_offset;
	for(unsigned short i = y; i < y + y_delta; i++)
	{
		ram_offset = & font_addr[(i * RAMLESS_LCD_H_WIDTH + x)>>1];
		for(unsigned short j = 0; j < (x_delta >> 1); j++)
		{
			*(ram_offset++) = *(pic_addr++);
		}
	}
}
/**
 * @brief     	This function servers to config ramless lcd.
 * @return  	none
 */
void ramless_lcd_config_boe1_72(void)
{
	unsigned char data[20];

	ramless_lcd_reset();

	data[0] = 0xC3;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0x96;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0xa5;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0x11;
	data[1] = 0x09;
	data[2] = 0xaa;
    data[3] = 0x11;
	ramless_lcd_lspi_master_write_data(0x00c100, data, 4);

	data[0] = 0x11;
	data[1] = 0x09;
	data[2] = 0xaa;
	data[3] = 0x11;
	ramless_lcd_lspi_master_write_data(0x00c200, data, 4);

	data[0] = 0x44;
	data[1] = 0x03;
	data[2] = 0x33;
	data[3] = 0x04;
	ramless_lcd_lspi_master_write_data(0x00c300, data, 4);

	data[0] = 0x44;
	data[1] = 0x03;
	data[2] = 0x33;
	data[3] = 0x04;
	ramless_lcd_lspi_master_write_data(0x00c400, data, 4);

	data[0] = 0x4d;
	ramless_lcd_lspi_master_write_data(0x00c500, data, 1);

	data[0] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00d600, data, 1);

	data[0] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00d700, data, 1);

	data[0] = 0xf0;
	data[1] = 0x0d;
	data[2] = 0x14;
	data[3] = 0x0a;
	data[4] = 0x09;
	data[5] = 0x06;
	data[6] = 0x3a;
	data[7] = 0x43;
	data[8] = 0x51;
	data[9] = 0x07;
	data[10] = 0x14;
	data[11] = 0x15;
	data[12] = 0x30;
	data[13] = 0x35;
	ramless_lcd_lspi_master_write_data(0x00e000, data, 14);

	data[0] = 0xf0;
	data[1] = 0x0d;
	data[2] = 0x13;
	data[3] = 0x0b;
	data[4] = 0x09;
	data[5] = 0x07;
	data[6] = 0x3b;
	data[7] = 0x43;
	data[8] = 0x50;
	data[9] = 0x08;
	data[10] = 0x14;
	data[11] = 0x15;
	data[12] = 0x30;
	data[13] = 0x35;
	ramless_lcd_lspi_master_write_data(0x00e100, data, 14);

	data[0] = 0xb9;
	data[1] = 0xf5;
	data[2] = 0xb5;
	data[3] = 0x55;
	data[4] = 0x22;
	data[5] = 0x25;
	data[6] = 0x10;
	data[7] = 0x22;
	data[8] = 0x22;
	data[9] = 0x22;
	data[10] = 0x22;
	data[11] = 0x22;
	data[12] = 0x22;
	data[13] = 0x22;
	ramless_lcd_lspi_master_write_data(0x00e500, data, 14);

	data[0] = 0xb9;
	data[1] = 0xf5;
	data[2] = 0xb5;
	data[3] = 0x55;
	data[4] = 0x22;
	data[5] = 0x25;
	data[6] = 0x10;
	data[7] = 0x22;
	data[8] = 0x22;
	data[9] = 0x22;
	data[10] = 0x22;
	data[11] = 0x22;
	data[12] = 0x22;
	data[13] = 0x22;
	ramless_lcd_lspi_master_write_data(0x00e600, data, 14);

	data[0] = 0x80;
	data[1] = 0x77;
	data[2] = 0x1f;
	data[3] = 0xcc;
	ramless_lcd_lspi_master_write_data(0x00e700, data, 4);

	data[0] = 0x00;
	data[1] = 0x55;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x88;
	ramless_lcd_lspi_master_write_data(0x00ec00, data, 6);

#if (RGB_SELECT == RGB888)
	data[0] = 0x07;
#elif (RGB_SELECT == RGB666)
	data[0] = 0x06;
#elif (RGB_SELECT == RGB565)
	data[0] = 0x05;
#endif

	ramless_lcd_lspi_master_write_data(0x003a00, data, 1);

	data[0] = 0xfe;
	data[1] = 0xdf;
	ramless_lcd_lspi_master_write_data(0x00b100, data, 2);

	data[0] = 0x09;
	ramless_lcd_lspi_master_write_data(0x00b200, data, 1);

	data[0] = 0x01;
	ramless_lcd_lspi_master_write_data(0x00b300, data, 1);

	data[0] = 0x01;
	ramless_lcd_lspi_master_write_data(0x00b400, data, 1);

	data[0] = 0x00;
	data[1] = 0x08;
	data[2] = 0x00;
	data[3] = 0x08;
	ramless_lcd_lspi_master_write_data(0x00b500, data, 4);

	data[0] = 0xbd;
	data[1] = 0x27;
	ramless_lcd_lspi_master_write_data(0x00b600, data, 2);

	data[0] = 0xc0;
	data[1] = 0x6b;
	ramless_lcd_lspi_master_write_data(0x00a400, data, 2);

	data[0] = 0x11;
	data[1] = 0x53;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x20;
	data[5] = 0x15;
	data[6] = 0x2a;
	data[7] = 0xba;
	data[8] = 0x02;
	ramless_lcd_lspi_master_write_data(0x00a500, data, 9);

	data[0] = 0x11;
	data[1] = 0x53;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x20;
	data[5] = 0x15;
	data[6] = 0x2a;
	data[7] = 0xba;
	data[8] = 0x02;
	ramless_lcd_lspi_master_write_data(0x00a600, data, 9);

	data[0] = 0x58;
	data[1] = 0x0a;
	data[2] = 0x34;
	data[3] = 0x10;
	data[4] = 0x22;
	data[5] = 0x01;
	data[6] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00ba00, data, 7);

	data[0] = 0x00;
	data[1] = 0x33;
	data[2] = 0x00;
	data[3] = 0x2c;
	data[4] = 0x83;
	data[5] = 0x07;
	data[6] = 0x18;
	data[7] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00bb00, data, 8);

	data[0] = 0x00;
	data[1] = 0x33;
	data[2] = 0x00;
	data[3] = 0x2c;
	data[4] = 0x83;
	data[5] = 0x07;
	data[6] = 0x18;
	data[7] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00bc00, data, 8);

	data[0] = 0x21;
	data[1] = 0x12;
	data[2] = 0xff;
	data[3] = 0xff;
	data[4] = 0x67;
	data[5] = 0x58;
	data[6] = 0x85;
	data[7] = 0x76;
	data[8] = 0xab;
	data[9] = 0xff;
	data[10] = 0x03;
	ramless_lcd_lspi_master_write_data(0x00bd00, data, 11);


	data[0] = 0xc3;
	ramless_lcd_lspi_master_write_data(0x00ed00, data, 1);

	data[0] = 0x08;
	data[1] = 0x2f;
	ramless_lcd_lspi_master_write_data(0x00e400, data, 2);

	data[0] = 0x22;
	ramless_lcd_lspi_master_write_data(0x00d900, data, 1);

	data[0] = 0x0c;
	ramless_lcd_lspi_master_write_data(0x003600, data, 1);

	ramless_lcd_lspi_master_write_cmd(0x002100);

	ramless_lcd_lspi_master_write_cmd(0x001100);

	delay_ms(120);//120ms

	ramless_lcd_lspi_master_write_cmd(0x002900);

}
/**
 * @brief     	This function servers to config ramless lcd.
 * @return  	none
 */
void ramless_lcd_config_ivo1_6(void)
{
	unsigned char data[20];

	ramless_lcd_reset();

	data[0] = 0xC3;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0x96;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0xa5;
	ramless_lcd_lspi_master_write_data(0x00f000, data, 1);

	data[0] = 0x20;
	ramless_lcd_lspi_master_write_data(0x00e900, data, 1);

	data[0] = 0x80;
	data[1] = 0x77;
	data[2] = 0x1f;
	data[3] = 0xcc;
	ramless_lcd_lspi_master_write_data(0x00e700, data, 4);

	data[0] = 0x77;
	data[1] = 0x07;
	data[2] = 0xc2;
	data[3] = 0x07;
	ramless_lcd_lspi_master_write_data(0x00c100, data, 4);

	data[0] = 0x77;
	data[1] = 0x07;
	data[2] = 0xc2;
	data[3] = 0x07;
	ramless_lcd_lspi_master_write_data(0x00c200, data, 4);

	data[0] = 0x22;
	data[1] = 0x02;
	data[2] = 0x22;
	data[3] = 0x04;
	ramless_lcd_lspi_master_write_data(0x00c300, data, 4);

	data[0] = 0x22;
	data[1] = 0x02;
	data[2] = 0x22;
	data[3] = 0x04;
	ramless_lcd_lspi_master_write_data(0x00c400, data, 4);

	data[0] = 0x71;
	ramless_lcd_lspi_master_write_data(0x00c500, data, 1);

	data[0] = 0x87;
	data[1] = 0x09;
	data[2] = 0x0c;
	data[3] = 0x06;
	data[4] = 0x05;
	data[5] = 0x03;
	data[6] = 0x29;
	data[7] = 0x32;
	data[8] = 0x49;
	data[9] = 0x0f;
	data[10] = 0x1b;
	data[11] = 0x17;
	data[12] = 0x2a;
	data[13] = 0x2f;
	ramless_lcd_lspi_master_write_data(0x00e000, data, 14);

	data[0] = 0x87;
	data[1] = 0x09;
	data[2] = 0x0c;
	data[3] = 0x06;
	data[4] = 0x05;
	data[5] = 0x03;
	data[6] = 0x29;
	data[7] = 0x32;
	data[8] = 0x49;
	data[9] = 0x0f;
	data[10] = 0x1b;
	data[11] = 0x17;
	data[12] = 0x2a;
	data[13] = 0x2f;
	ramless_lcd_lspi_master_write_data(0x00e100, data, 14);

	data[0] = 0xb2;
	data[1] = 0xf5;
	data[2] = 0xbd;
	data[3] = 0x24;
	data[4] = 0x22;
	data[5] = 0x25;
	data[6] = 0x10;
	data[7] = 0x22;
	data[8] = 0x22;
	data[9] = 0x22;
	data[10] = 0x22;
	data[11] = 0x22;
	data[12] = 0x22;
	data[13] = 0x22;
	ramless_lcd_lspi_master_write_data(0x00e500, data, 14);

	data[0] = 0xb2;
	data[1] = 0xf5;
	data[2] = 0xbd;
	data[3] = 0x24;
	data[4] = 0x22;
	data[5] = 0x25;
	data[6] = 0x10;
	data[7] = 0x22;
	data[8] = 0x22;
	data[9] = 0x22;
	data[10] = 0x22;
	data[11] = 0x22;
	data[12] = 0x22;
	data[13] = 0x22;
	ramless_lcd_lspi_master_write_data(0x00e600, data, 14);

	data[0] = 0x40;
	data[1] = 0x03;
	ramless_lcd_lspi_master_write_data(0x00ec00, data, 2);

	data[0] = 0x0c;
	ramless_lcd_lspi_master_write_data(0x003600, data, 1);

#if (RGB_SELECT == RGB888)
	data[0] = 0x07;
#elif (RGB_SELECT == RGB666)
	data[0] = 0x06;
#elif (RGB_SELECT == RGB565)
	data[0] = 0x05;
#endif

	ramless_lcd_lspi_master_write_data(0x003a00, data, 1);

	data[0] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00b200, data, 1);

	data[0] = 0x01;
	ramless_lcd_lspi_master_write_data(0x00b300, data, 1);

	data[0] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00b400, data, 1);

	data[0] = 0x00;
	data[1] = 0x08;
	data[2] = 0x00;
	data[3] = 0x08;
	ramless_lcd_lspi_master_write_data(0x00b500, data, 4);

	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x20;
	data[5] = 0x15;
	data[6] = 0x2a;
	data[7] = 0x8a;
	data[8] = 0x02;
	ramless_lcd_lspi_master_write_data(0x00a500, data, 9);

	data[0] = 0x00;
	data[1] = 0x00;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x20;
	data[5] = 0x15;
	data[6] = 0x2a;
	data[7] = 0x8a;
	data[8] = 0x02;
	ramless_lcd_lspi_master_write_data(0x00a600, data, 9);

	data[0] = 0x0a;
	data[1] = 0x5a;
	data[2] = 0x23;
	data[3] = 0x10;
	data[4] = 0x25;
	data[5] = 0x02;
	data[6] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00ba00, data, 7);

	data[0] = 0x00;
	data[1] = 0x30;
	data[2] = 0x00;
	data[3] = 0x29;
	data[4] = 0x88;
	data[5] = 0x87;
	data[6] = 0x18;
	data[7] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00bb00, data, 8);

	data[0] = 0x00;
	data[1] = 0x30;
	data[2] = 0x00;
	data[3] = 0x29;
	data[4] = 0x88;
	data[5] = 0x87;
	data[6] = 0x18;
	data[7] = 0x00;
	ramless_lcd_lspi_master_write_data(0x00bc00, data, 8);

	data[0] = 0xa1;
	data[1] = 0xb2;
	data[2] = 0x2b;
	data[3] = 0x1a;
	data[4] = 0x56;
	data[5] = 0x43;
	data[6] = 0x34;
	data[7] = 0x65;
	data[8] = 0xff;
	data[9] = 0xff;
	data[10] = 0x0f;
	ramless_lcd_lspi_master_write_data(0x00bd00, data, 11);

	data[0] = 0x00;
	ramless_lcd_lspi_master_write_data(0x003500, data, 1);

	ramless_lcd_lspi_master_write_cmd(0x002100);

	ramless_lcd_lspi_master_write_cmd(0x001100);

	delay_ms(180);//120ms

	ramless_lcd_lspi_master_write_cmd(0x002900);

	delay_ms(180);//120ms

	ramless_lcd_lspi_master_write_cmd(0x002c00);

}

/**
 * @brief    This function serves to enable ramless lcd bar mode.
 * @return   none
 * @attention bar mode is used to test the spi connection between mcu and lcd, you will see stripe in lcd.
 */
void ramless_lcd_bar_mode(void)
{
	unsigned char data[15];
	data[0] = 0xA5;
	ramless_lcd_lspi_master_write_data(0x00b000, data, 1);
	data[0] = 0x40;
	data[1] = 0x00;
	data[2] = 0x3f;
	data[3] = 0x00;
	data[4] = 0x14;
	data[5] = 0x14;
	data[6] = 0x20;
	data[7] = 0x20;
	data[8] = 0x03;
	ramless_lcd_lspi_master_write_data(0x00cc00, data, 9);
}

/**
 * @brief     This function serves to enable ramless lcd scan.
 * @param[in] font_addr - foreground address.
 * @param[in] back_addr - background address.
 * @return    none
 * @note      this interface is display in little endian mode.
 */
void ramless_lcd_scan_config(unsigned int font_addr,unsigned int back_addr)
{

#if(LCD_DATA_LANE_MODE == CMD1X_ADDR1X_DATA1X)
	lspi_lcd_set_cmd(0xD8);
	spi_set_normal_mode(LSPI_MODULE);
#elif(LCD_DATA_LANE_MODE == CMD1X_ADDR1X_DATA2X)
	lspi_lcd_set_cmd(0xDB);
	spi_set_dual_mode(LSPI_MODULE);
#elif(LCD_DATA_LANE_MODE == CMD1X_ADDR1X_DATA4X)
	lspi_lcd_set_cmd(0xDE);
	spi_set_quad_mode(LSPI_MODULE);
#elif(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
	lspi_lcd_set_cmd(0xDE);
	spi_set_quad_mode(LSPI_MODULE);
#endif

#if (RGB_SELECT == RGB888) || (RGB_SELECT == RGB666)
	lspi_lcd_set_display_transmode(SPI_MODE_WRITE_ONLY);//write only for Display dummy
	lspi_lcd_set_porch_transmode(SPI_MODE_NONE_DATA); ////nodata for CMD_DUMMY

#elif (RGB_SELECT == RGB565)
#if(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
	lspi_lcd_set_display_transmode(SPI_MODE_DUMMY_WRITE);//write only for Display dummy
	lspi_lcd_set_porch_transmode(SPI_MODE_NONE_DATA); ////nodata for CMD_DUMMY
#else
	lspi_lcd_set_display_transmode(SPI_MODE_WRITE_ONLY);//write only for Display dummy
	lspi_lcd_set_porch_transmode(SPI_MODE_NONE_DATA); ////nodata for CMD_DUMMY
#endif
#endif
	lspi_lcd_rgb_little_endian_mode();
//vbp vfp given by the manufacturer test
	lspi_lcd_vbp_line_cnt(RAMLESS_LCD_VBP_CNT);

	lspi_lcd_vfp_line_cnt(RAMLESS_LCD_VFP_CNT);

	lspi_lcd_bimage_start_addr(back_addr);
	lspi_lcd_fimage_start_addr(convert_ram_addr_cpu2bus(font_addr));

#if (RGB_SELECT == RGB888)
	lspi_lcd_rgb_mode(LSPI_LCD_RGB888); // 1:565   2��666   3:888
#elif (RGB_SELECT == RGB666)
	lspi_lcd_rgb_mode(LSPI_LCD_RGB666);
#elif (RGB_SELECT == RGB565)
	lspi_lcd_rgb_mode(LSPI_LCD_RGB565);
#endif
//the minimum time of porch and display need meet 40us, and the delay between line and line not be less than 500ns.
	lspi_lcd_porch_min_line_time(1539);
	lspi_lcd_display_min_line_time(1539);
	spi_set_cs_high_time(LSPI_MODULE,26);

	lspi_lcd_set_res(RAMLESS_LCD_V_WIDTH,RAMLESS_LCD_H_WIDTH);
	lspi_lcd_single_color_mode_dis();
	lspi_lcd_scan_en();
}

volatile unsigned int frame_num = 0;
unsigned char read_data[20]={0};
_attribute_ram_code_sec_noinline_ void lspi_irq_handler(void)
{
	if(lspi_lcd_get_irq_status(FLD_LSPI_LCD_FRAME_INT_STA))
	{
		lspi_lcd_clr_irq_status(FLD_LSPI_LCD_FRAME_INT_STA);//clr
		frame_num ++;
		gpio_toggle(LED2);
	}
}
PLIC_ISR_REGISTER(lspi_irq_handler, IRQ_LSPI)
__attribute__((section(".ram_code"))) void user_init(void)
{

	gpio_output_en(RAMLESS_LCD_RESET_PIN); 		//enable output
	gpio_input_dis(RAMLESS_LCD_RESET_PIN);		//disable input
	gpio_set_high_level(RAMLESS_LCD_RESET_PIN);
	gpio_function_en(RAMLESS_LCD_RESET_PIN);

#if(RAMLESS_LCD_MODE ==  RAMLESS_LCD_BOE1_72)
	gpio_output_en(RAMLESS_LCD_POWER_ON_PIN); 		//enable output
	gpio_input_dis(RAMLESS_LCD_POWER_ON_PIN);		//disable input
	gpio_set_low_level(RAMLESS_LCD_POWER_ON_PIN);
	gpio_function_en(RAMLESS_LCD_POWER_ON_PIN);
#elif(RAMLESS_LCD_MODE ==  RAMLESS_LCD_IVO1_6)
#if(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
	gpio_output_en(RAMLESS_LCD_IM0); 		//enable output
	gpio_input_dis(RAMLESS_LCD_IM0);		//disable input
	gpio_set_high_level(RAMLESS_LCD_IM0);
	gpio_function_en(RAMLESS_LCD_IM0);
#else
	gpio_output_en(RAMLESS_LCD_IM0); 		//enable output
	gpio_input_dis(RAMLESS_LCD_IM0);		//disable input
	gpio_set_low_level(RAMLESS_LCD_IM0);
	gpio_function_en(RAMLESS_LCD_IM0);
#endif
	gpio_output_en(RAMLESS_LCD_IM1); 		//enable output
	gpio_input_dis(RAMLESS_LCD_IM1);		//disable input
	gpio_set_low_level(RAMLESS_LCD_IM1);
	gpio_function_en(RAMLESS_LCD_IM1);
#endif

	gpio_output_en(LED1); 		//enable output
	gpio_input_dis(LED1);		//disable input
	gpio_set_low_level(LED1);
	gpio_function_en(LED1);

	gpio_output_en(LED2); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_set_low_level(LED2);
	gpio_function_en(LED2);


	core_interrupt_disable();
	lspi_lcd_clr_irq_status(FLD_LSPI_LCD_FRAME_INT_STA);//clr
	lspi_lcd_set_irq_mask(FLD_LSPI_LCD_FRAME_INT_EN);//endint_en

	spi_master_init(LSPI_MODULE, sys_clk.pll_clk * 1000000/SPI_CLK, SPI_MODE0);//48M SPI_CLK
	spi_tx_irq_trig_cnt(LSPI_MODULE,20);
	plic_interrupt_enable(IRQ_LSPI);
	core_interrupt_enable();
	lspi_set_pin(&lspi_pin_config);


	//when setting ramless lcd configuration,only the CMD4X_ADDR4X_DATA4X/CMD1X_ADDR1X_DATA1X modes are supported.
	#if(LCD_DATA_LANE_MODE == CMD4X_ADDR4X_DATA4X)
		spi_master_config_plus(LSPI_MODULE, &spi_ramless_lcd_protocol);
	#else
		spi_master_config_plus(LSPI_MODULE, &spi_ramless_lcd_protocol_config);
	#endif

	lspi_lcd_set_lut(LUT_RED, RED);
	lspi_lcd_set_lut(LUT_GREEN, GREEN);
	lspi_lcd_set_lut(LUT_BLUE, BLUE);
	lspi_lcd_set_lut(LUT_WHITE, WHITE);
	lspi_lcd_set_lut(LUT_BLACK, BLACK);
#if(RAMLESS_LCD_MODE ==  RAMLESS_LCD_BOE1_72)
	ramless_lcd_config_boe1_72();
#elif(RAMLESS_LCD_MODE ==  RAMLESS_LCD_IVO1_6)
	ramless_lcd_config_ivo1_6();
#endif
	spi_master_config_plus(LSPI_MODULE, &spi_ramless_lcd_protocol);
#if(RAMLESS_LCD_MODE ==  RAMLESS_LCD_BOE1_72)
#if (RGB_SELECT == RGB888)
	ramless_lcd_scan_config((unsigned int)image_4bitram_boe1_72,(unsigned int)gImage_boe1_72_rgb888);
#elif (RGB_SELECT == RGB666)
	ramless_lcd_scan_config((unsigned int)image_4bitram_boe1_72,(unsigned int)gImage_boe1_72_rgb666);
#elif (RGB_SELECT == RGB565)
	ramless_lcd_scan_config((unsigned int)image_4bitram_boe1_72,(unsigned int)gImage_boe1_72_rgb565);
#endif
	ramless_lcd_fill_rectangle(100,100,100,100,LUT_GREEN,(unsigned char *)image_4bitram_boe1_72);
	unsigned int i = 250, j = 0;
	for(j=0;j<RAMLESS_LCD_V_WIDTH;j++)
	ramless_lcd_draw_point(i,j,LUT_RED,(unsigned char *)image_4bitram_boe1_72);
#elif(RAMLESS_LCD_MODE ==  RAMLESS_LCD_IVO1_6)
#if (RGB_SELECT == RGB888)
	ramless_lcd_scan_config((unsigned int)image_4bitram_ivo1_6,(unsigned int)gimage_ivo1_6_rgb888);
#elif (RGB_SELECT == RGB666)
	ramless_lcd_scan_config((unsigned int)image_4bitram_ivo1_6,(unsigned int)gimage_ivo1_6_rgb666);
#elif (RGB_SELECT == RGB565)
	ramless_lcd_scan_config((unsigned int)image_4bitram_ivo1_6,(unsigned int)gimage_ivo1_6_rgb565);
#endif
	ramless_lcd_fill_rectangle(100,100,100,100,LUT_GREEN,(unsigned char *)image_4bitram_ivo1_6);
	unsigned int i = 250, j = 0;
	for(j=0;j<RAMLESS_LCD_V_WIDTH;j++)
	ramless_lcd_draw_point(i,j,LUT_RED,(unsigned char *)image_4bitram_ivo1_6);
#endif

}

void main_loop (void)
{
	delay_ms(200);
	gpio_toggle(LED1);
}
#endif

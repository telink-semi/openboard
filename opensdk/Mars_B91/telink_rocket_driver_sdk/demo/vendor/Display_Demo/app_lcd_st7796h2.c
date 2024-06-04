/********************************************************************************************************
 * @file    app_lcd_st7796h2.c
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
#if (SPI_DISPLAY_SELECT == SPI_LCD)
#include "lcd_st7796h2_data/lcd_st7796h2_pic.h"

#define SPI_CLK 				24000000

#define LCD_RESET_PIN			GPIO_PB5

#define	LCD_1DATA_LANE			1
#define	LCD_2DATA_LANE			2
#define LCD_DATA_LANE_MODE		LCD_2DATA_LANE

#define LCD_DMA_EN   			1
#define LCD_DMA_CHN				DMA4

lspi_pin_config_t lspi_pin_config = {
	.spi_csn_pin 		= LSPI_CSN_PE0_PIN,
	.spi_clk_pin		= LSPI_CLK_PE1_PIN,
	.spi_mosi_io0_pin   = LSPI_MOSI_IO0_PE2_PIN,
	.spi_miso_io1_pin   = LSPI_MISO_IO1_PE3_PIN,//3line mode set 0
	.spi_io2_pin    	= LSPI_IO2_PE4_PIN,//set quad mode otherwise set 0
	.spi_io3_pin  		= LSPI_IO3_PE5_PIN,//set quad mode otherwise set 0
};

spi_wr_rd_config_t lspi_lcd_protocol_config = {
	.spi_io_mode	= SPI_SINGLE_MODE,
	.spi_dummy_cnt 	= 0,
	.spi_cmd_en		= 0,
	.spi_addr_en	= 0,
	.spi_addr_len	= 0,//when spi_addr_en = false, invalid set.
	.spi_cmd_fmt_en	= 0,//when spi_cmd_en = false, invalid set.
	.spi_addr_fmt_en= 0,//when spi_addr_en = false, invalid set.
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

/**
 * @brief    This function serves to reset lcd.
 * @return   none
 */
void lcd_reset(void)
{
	gpio_set_low_level(LCD_RESET_PIN);
	delay_ms(120);
	gpio_set_high_level(LCD_RESET_PIN);
	delay_ms(120);
}

/**
 * @brief    This function serves to write one byte cmd to lcd.
 * @param[in] cmd - cmd byte.
 * @return   none
 */
void lcd_write_cmd(unsigned char cmd)
{
	lspi_set_lcd_dcx_cmd();
	spi_master_write(LSPI_MODULE,&cmd,1);
}
/**
 * @brief    This function serves to write one byte data to lcd.
 * @param[in] data - data byte.
 * @return   none
 */
void lcd_write_data(unsigned char data)
{
	lspi_set_lcd_dcx_data();
	spi_master_write(LSPI_MODULE,&data,1);
}

/**
 * @brief    This function serves to write operating memory address and space size to lcd.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @return   none
 */
void lcd_set_address(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta)
{
	unsigned char x_h,x_l,xe_h,xe_l;
	unsigned char y_h,y_l,ye_h,ye_l;
	x_h = x>>8;
	x_l = x&0xff;
	xe_h = (x_delta+x-1)>>8;
	xe_l = (x_delta+x-1)&0xff;

	y_h = y>>8;
	y_l = y&0xff;
	ye_h = (y_delta+y-1)>>8;
	ye_l = (y_delta+y-1)&0xff;

	while (spi_is_busy(LSPI_MODULE));
	lspi_lcd_2lane_dis();
	spi_set_normal_mode(LSPI_MODULE);

	lcd_write_cmd(0x11);//Sleep out
	lcd_write_cmd(0x2A);
	lcd_write_data(x_h);
	lcd_write_data(x_l);
	lcd_write_data(xe_h);
	lcd_write_data(xe_l);

	lcd_write_cmd(0x2B);
	lcd_write_data(y_h);
	lcd_write_data(y_l);
	lcd_write_data(ye_h);
	lcd_write_data(ye_l);

	lcd_write_cmd(0x2c);//enable write
}

/**
 * @brief    This function serves to fill a rectangular space of lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of space.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of space.
 * @param[in] color - the single color.
 * @return   none
 */
void lcd_fill_rectangle(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,unsigned int color)
{
	unsigned int display_size;
	lcd_set_address(x, x_delta, y, y_delta);
	display_size = x_delta * y_delta;
	lspi_set_lcd_dcx_data();

#if RGB_SELECT==RGB565
	lspi_lcd_rgb_mode(LSPI_LCD_RGB565);
#elif RGB_SELECT==RGB666
	lspi_lcd_rgb_mode(LSPI_LCD_RGB666);
#elif RGB_SELECT==RGB888
	lspi_lcd_rgb_mode(LSPI_LCD_RGB888);
#endif
#if  LCD_DATA_LANE_MODE == LCD_2DATA_LANE
	lspi_lcd_2lane_en();
	spi_set_dual_mode(LSPI_MODULE);
#elif LCD_DATA_LANE_MODE == LCD_1DATA_LANE
	lspi_lcd_2lane_dis();
#endif

#if RGB_SELECT==RGB565
	spi_master_write_repeat(LSPI_MODULE, (unsigned char*)&color, 2, display_size);
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	spi_master_write_repeat(LSPI_MODULE, (unsigned char*)&color, 3, display_size);
#endif
}

/**
 * @brief    This function serves to draw a 1*1 point in lcd with single color.
 * @param[in] x - abscissa of address.
 * @param[in] y - ordinate of address.
 * @param[in] color - the single color.
 * @return   none
 */
void lcd_draw_point(unsigned short x,unsigned short y,unsigned int color)
{
	lcd_fill_rectangle(x, 1, y, 1,color);
}

/**
 * @brief     	This function servers to config lcd.
 * @return  	none
 */
volatile unsigned int id;
void lcd_config(void)
{
	lcd_reset();

	lcd_write_cmd(0x11);//Sleep out
	delay_ms(120);       //ms

	lcd_write_cmd(0xF0);// Command Set Control
	lcd_write_data(0xC3);//Enable command2 part1

	lcd_write_cmd(0xF0);// Command Set Control
	lcd_write_data(0x96);//Enable command2 part2

	lcd_write_cmd(0x36);// Memory Access Control
	lcd_write_data(0x48);

	lcd_write_cmd(0x3A);  //Interface Pixel Format
#if  RGB_SELECT==RGB565
	lcd_write_data(0x05);	//RGB565
#elif RGB_SELECT==RGB666
	lcd_write_data(0x06);  //RGB666
#elif RGB_SELECT==RGB888
	lcd_write_data(0x07);  //RGB888
#endif
	lcd_write_cmd(0xB4);//Display Inversion Control
	lcd_write_data(0x00); //Column inversion

	lcd_write_cmd(0xB1);//Frame Rate Control (In Normal Mode/Full Colors)
	lcd_write_data(0xA0);
	lcd_write_data(0x08);

	lcd_write_cmd(0xB5);//Blanking Porch Control
	lcd_write_data(0x02);//VFP
	lcd_write_data(0x02);//VBP
	lcd_write_data(0x00);//reserved
	lcd_write_data(0x04);//HBP

	lcd_write_cmd(0xB6);// Display Function Control
	lcd_write_data(0x00); //Normal
	lcd_write_data(0x02); //Normal 0x00
	lcd_write_data(0x3B);// Sets the max number of lines to drive the LCD:8*(0x3b+1) = 480 line.

	lcd_write_cmd(0xB9);// Mode Selection
	lcd_write_data(0x00); //Digital gamma function off, dithering function control off.


#if  LCD_DATA_LANE_MODE == LCD_2DATA_LANE
	lcd_write_data(0xE1);//LCD_2DATA_LANE enable 0x01
#elif LCD_DATA_LANE_MODE == LCD_1DATA_LANE
	lcd_write_data(0xE0);// 2-Lane SPI function disable 0x00
#endif

	lcd_write_cmd(0xC0);//Power Control 1
	lcd_write_data(0xF0); // AVDD:6.8V, AVCL: -5.0
	lcd_write_data(0x00); // VGH:12.541, VGL:-7.158

	lcd_write_cmd(0xC1);//Power Control 2
	lcd_write_data(0x1F); //VOP=5.1

	lcd_write_cmd(0xC2);//Power Control 3
	lcd_write_data(0xA7);//SOP:0x02,GOP:0x03

	lcd_write_cmd(0xC5);//VCOM Control(VCOM: a power supply for the TFT-LCD common electrode)
	lcd_write_data(0x15); //VCOM: 0.825

	lcd_write_cmd(0xE8);//Display Output Ctrl Adjust
	lcd_write_data(0x40);
	lcd_write_data(0x8A);
	lcd_write_data(0x00);
	lcd_write_data(0x00);
	lcd_write_data(0x29);// Set Source equalizing period time. (0x9*1.5 + 9)us
	lcd_write_data(0x19);//	Gate timing Control G_START:Tclk*0x19
	lcd_write_data(0xA5);// G_EQ: Gate driver EQ function ON. Gate timing Control G_END:Tclk*0x25
	lcd_write_data(0x33);

	lcd_write_cmd(0xE0);// Positive Gamma Control, adjust the gamma characteristics of the TFT panel.
	lcd_write_data(0xF0);
	lcd_write_data(0x10);
	lcd_write_data(0x16);
	lcd_write_data(0x0C);
	lcd_write_data(0x0A);
	lcd_write_data(0x15);
	lcd_write_data(0x3A);
	lcd_write_data(0x43);
	lcd_write_data(0x4E);
	lcd_write_data(0x18);
	lcd_write_data(0x14);
	lcd_write_data(0x13);
	lcd_write_data(0x2E);
	lcd_write_data(0x33);

	lcd_write_cmd(0xE1);//Negative Gamma Control,adjust the gamma characteristics of the TFT panel.
	lcd_write_data(0xF0);
	lcd_write_data(0x10);
	lcd_write_data(0x16);
	lcd_write_data(0x0C);
	lcd_write_data(0x0A);
	lcd_write_data(0x15);
	lcd_write_data(0x3A);
	lcd_write_data(0x43);
	lcd_write_data(0x4E);
	lcd_write_data(0x18);
	lcd_write_data(0x14);
	lcd_write_data(0x13);
	lcd_write_data(0x2E);
	lcd_write_data(0x33);

	lcd_write_cmd(0xF0);//Command Set Control
	lcd_write_data(0x3C);//Disable command2 part1

	lcd_write_cmd(0xF0);//Command Set Control

	lcd_write_data(0x69);//Disable command2 part2

	lcd_write_cmd(0x29);//Display on

	lcd_write_cmd(0x21);//Display Inversion On

	lcd_write_cmd(0x35);//Tearing Effect setting

}

/**
 * @brief    This function serves to show a rectangular picture in lcd.
 * @param[in] x - abscissa of address.
 * @param[in] x_delta - horizontal width of picture.
 * @param[in] y - ordinate of address.
 * @param[in] y_delta - vertical width of picture.
 * @param[in] pic_addr - address of picture buffer.
 * @return   none
 */
void lcd_show_picture(unsigned short x, unsigned short x_delta, unsigned short y,unsigned short y_delta,  unsigned char *pic_addr)
{
	unsigned int display_size;
	lcd_set_address(x, x_delta, y, y_delta);
	lspi_set_lcd_dcx_data();

#if RGB_SELECT==RGB565
	lspi_lcd_rgb_mode(LSPI_LCD_RGB565);
#elif RGB_SELECT==RGB666
	lspi_lcd_rgb_mode(LSPI_LCD_RGB666);
#elif RGB_SELECT==RGB888
	lspi_lcd_rgb_mode(LSPI_LCD_RGB888);
#endif
#if  LCD_DATA_LANE_MODE == LCD_2DATA_LANE
	lspi_lcd_2lane_en();
	spi_set_dual_mode(LSPI_MODULE);
#elif LCD_DATA_LANE_MODE == LCD_1DATA_LANE
	lspi_lcd_2lane_dis();
#endif

#if RGB_SELECT==RGB565
	display_size=2*x_delta * y_delta;
#elif (RGB_SELECT==RGB666 || RGB_SELECT==RGB888) //rgb_666 & RGB_888
	display_size=3*x_delta * y_delta;
#endif
#if LCD_DMA_EN
	spi_master_write_dma(LSPI_MODULE, (unsigned char*)pic_addr, display_size);
#else
	spi_master_write(LSPI_MODULE, (unsigned char*)pic_addr, display_size);
#endif
}
volatile unsigned char spi_rx_buff[240*2*2] __attribute__((aligned(4))) = {0x00};

void user_init(void)
{
	gpio_output_en(LCD_RESET_PIN);
	gpio_input_dis(LCD_RESET_PIN);
	gpio_set_high_level(LCD_RESET_PIN);
	gpio_function_en(LCD_RESET_PIN);

	spi_master_init(LSPI_MODULE, sys_clk.pll_clk * 1000000 / SPI_CLK, SPI_MODE0);
	lspi_set_pin(&lspi_pin_config);
#if LCD_DMA_EN
	spi_set_tx_dma_config(LSPI_MODULE, LCD_DMA_CHN);
#endif
	spi_master_config_plus(LSPI_MODULE, &lspi_lcd_protocol_config);
	lspi_lcd_dcx_en();

	lcd_config();

	//show a picture in lcd
#if RGB_SELECT==RGB565
	lcd_show_picture(0,320,0,380,(unsigned char *)gImage_lcd565);
#elif RGB_SELECT==RGB666
	lcd_show_picture(0,320,0,380,(unsigned char *)gImage_lcd666);
#elif RGB_SELECT==RGB888
	lcd_show_picture(0,320,0,380,(unsigned char *)gImage_lcd888);
#endif

//	//fill a rectangle with red color in lcd
	lcd_fill_rectangle(150,100,300,30,RED);

}
volatile unsigned short point_x=150;
volatile unsigned short point_y=340;
void main_loop (void)
{
	if(point_x != 250)
	{
		//draw a line with blue color in lcd
		lcd_draw_point(point_x,point_y,BLUE);
		point_x ++;
	}
}
#endif

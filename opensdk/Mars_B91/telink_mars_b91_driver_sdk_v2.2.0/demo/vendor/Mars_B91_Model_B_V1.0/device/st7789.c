#include "../app_config.h"
#include "st7789.h"

LCD_ST7789_ATTRIBUTES LCD_ST7789_ST;

pspi_pin_config_t pspi_pin_config = {
	.pspi_clk_pin 		= PSPI_CK_PIN,
	.pspi_csn_pin   	= PSPI_CN_PIN,
	.pspi_mosi_io0_pin  = PSPI_MOSI_PIN,
	.pspi_miso_io1_pin  = PSPI_MISO_PIN,
};

pspi_config_t pspi_slave_protocol_config = {
	.pspi_io_mode 	= PSPI_SINGLE,/*Io mode set to PSPI_3LINE when LCD_ST7789_ADDR.*/
	.pspi_dummy_cnt = 8,
	.pspi_cmd_en 	= 0,
};

void LCD_ST7789_SendCommand(unsigned char cmd)
{
	LCD_ST7789_DC_0;
    spi_master_write_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)&cmd, 1, SPI_MODE_WR_WRITE_ONLY);
}

void LCD_ST7789_SendByte(unsigned char data)
{
    LCD_ST7789_DC_1;
    spi_master_write_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, (unsigned char*)&data, 1, SPI_MODE_WR_WRITE_ONLY);
}

void LCD_ST7789_SendBytes(unsigned char *data,unsigned int len)
{
    LCD_ST7789_DC_1;
    spi_master_write_plus(0, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL,data, len, SPI_MODE_WR_WRITE_ONLY);
}

void PSPI_ST7789_Init(void)
{
	gpio_function_en(LCD_ST7789_DC);
	gpio_output_en(LCD_ST7789_DC);
	gpio_input_dis(LCD_ST7789_DC);
	gpio_function_en(LCD_ST7789_CN);
	gpio_output_en(LCD_ST7789_CN);
	gpio_input_dis(LCD_ST7789_CN);
    LCD_ST7789_CN_0;
	spi_master_init(0, sys_clk.pclk * 1000000 / (2 * SPI_CLK_SPEED) - 1, SPI_MODE0);
	// pspi_set_pin(&pspi_pin_config);
	extern void pspi_set_pin_mux(pspi_pin_def_e pin);
	pspi_set_pin_mux(pspi_pin_config.pspi_clk_pin);
	pspi_set_pin_mux(pspi_pin_config.pspi_mosi_io0_pin);
	//pspi_master_config_plus(&pspi_slave_protocol_config);
}

void LCD_ST7789_InitReg(void)
{
    LCD_ST7789_SendCommand(0x36);
    LCD_ST7789_SendByte(0x00);

    LCD_ST7789_SendCommand(0x3A);
    LCD_ST7789_SendByte(0x05);

    LCD_ST7789_SendCommand(0xB2);
    LCD_ST7789_SendByte(0x0B);
    LCD_ST7789_SendByte(0x0B);
    LCD_ST7789_SendByte(0x00);
    LCD_ST7789_SendByte(0x33);
    LCD_ST7789_SendByte(0x35);

    LCD_ST7789_SendCommand(0xB7);
    LCD_ST7789_SendByte(0x11);

    LCD_ST7789_SendCommand(0xBB);
    LCD_ST7789_SendByte(0x35);

    LCD_ST7789_SendCommand(0xC0);
    LCD_ST7789_SendByte(0x2C);

    LCD_ST7789_SendCommand(0xC2);
    LCD_ST7789_SendByte(0x01);

    LCD_ST7789_SendCommand(0xC3);
    LCD_ST7789_SendByte(0x0D);

    LCD_ST7789_SendCommand(0xC4);
    LCD_ST7789_SendByte(0x20);

    LCD_ST7789_SendCommand(0xC6);
    LCD_ST7789_SendByte(0x13);

    LCD_ST7789_SendCommand(0xD0);
    LCD_ST7789_SendByte(0xA4);
    LCD_ST7789_SendByte(0xA1);

    LCD_ST7789_SendCommand(0xD6);
    LCD_ST7789_SendByte(0xA1);

    LCD_ST7789_SendCommand(0xE0);
    LCD_ST7789_SendByte(0xF0);
    LCD_ST7789_SendByte(0x06);
    LCD_ST7789_SendByte(0x0B);
    LCD_ST7789_SendByte(0x0A);
    LCD_ST7789_SendByte(0x09);
    LCD_ST7789_SendByte(0x26);
    LCD_ST7789_SendByte(0x29);
    LCD_ST7789_SendByte(0x33);
    LCD_ST7789_SendByte(0x41);
    LCD_ST7789_SendByte(0x18);
    LCD_ST7789_SendByte(0x16);
    LCD_ST7789_SendByte(0x15);
    LCD_ST7789_SendByte(0x29);
    LCD_ST7789_SendByte(0x2D);

    LCD_ST7789_SendCommand(0xE1);
    LCD_ST7789_SendByte(0xF0);
    LCD_ST7789_SendByte(0x04);
    LCD_ST7789_SendByte(0x08);
    LCD_ST7789_SendByte(0x08);
    LCD_ST7789_SendByte(0x07);
    LCD_ST7789_SendByte(0x03);
    LCD_ST7789_SendByte(0x28);
    LCD_ST7789_SendByte(0x32);
    LCD_ST7789_SendByte(0x40);
    LCD_ST7789_SendByte(0x3B);
    LCD_ST7789_SendByte(0x19);
    LCD_ST7789_SendByte(0x18);
    LCD_ST7789_SendByte(0x2A);
    LCD_ST7789_SendByte(0x2E);

    LCD_ST7789_SendCommand(0xE4);
    LCD_ST7789_SendByte(0x25);
    LCD_ST7789_SendByte(0x00);
    LCD_ST7789_SendByte(0x00);

    LCD_ST7789_SendCommand(0x21);

    LCD_ST7789_SendCommand(0x11);
    delay_ms(120);
    LCD_ST7789_SendCommand(0x29);
}

void LCD_ST7789_SetAttributes(unsigned int Scan_dir)
{
    // Get the screen scan direction
	LCD_ST7789_ST.SCAN_DIR = Scan_dir;
    unsigned char MemoryAccessReg = 0x00;
    // Get GRAM and LCD width and height
    if (Scan_dir == HORIZONTAL) {
    	LCD_ST7789_ST.HEIGHT = LCD_ST7789_WIDTH;
    	LCD_ST7789_ST.WIDTH  = LCD_ST7789_HEIGHT;
        MemoryAccessReg = 0X70;
    }
    else {
    	LCD_ST7789_ST.HEIGHT = LCD_ST7789_HEIGHT;
    	LCD_ST7789_ST.WIDTH  = LCD_ST7789_WIDTH;
        MemoryAccessReg = 0X00;
    }
    // Set the read / write scan direction of the frame memory
    LCD_ST7789_SendCommand(0x36); // MX, MY, RGB mode
    LCD_ST7789_SendByte(MemoryAccessReg); // 0x08 set RGB
}

void LCD_ST7789_SetWindows(unsigned short Xstart, unsigned short Ystart, unsigned short Xend, unsigned short Yend)
{
    if (LCD_ST7789_ST.SCAN_DIR == VERTICAL) {
		// set the X coordinates
        LCD_ST7789_SendCommand(0x2A);
        LCD_ST7789_SendByte(Xstart >> 8);
        LCD_ST7789_SendByte(Xstart);
        LCD_ST7789_SendByte((Xend-1) >> 8);
        LCD_ST7789_SendByte(Xend-1);
        // set the Y coordinates
        LCD_ST7789_SendCommand(0x2B);
        LCD_ST7789_SendByte((Ystart+20) >> 8);
        LCD_ST7789_SendByte(Ystart+20);
        LCD_ST7789_SendByte((Yend+20-1) >> 8);
        LCD_ST7789_SendByte(Yend+20-1);
    }
    else {
        // set the X coordinates
        LCD_ST7789_SendCommand(0x2A);
        LCD_ST7789_SendByte((Xstart+20) >> 8);
        LCD_ST7789_SendByte(Xstart+20);
        LCD_ST7789_SendByte((Xend+20-1) >> 8);
        LCD_ST7789_SendByte(Xend+20-1);
        // set the Y coordinates
        LCD_ST7789_SendCommand(0x2B);
        LCD_ST7789_SendByte(Ystart >> 8);
        LCD_ST7789_SendByte(Ystart);
        LCD_ST7789_SendByte((Yend-1) >> 8);
        LCD_ST7789_SendByte(Yend-1);
    }
    LCD_ST7789_SendCommand(0X2C);
}

void LCD_Init(void)
{
	PSPI_ST7789_Init();
	LCD_ST7789_SetAttributes(VERTICAL);
	LCD_ST7789_InitReg();
    LCD_ST7789_SetWindows(0, 0, LCD_ST7789_ST.WIDTH, LCD_ST7789_ST.HEIGHT);
}

unsigned char g_lcd_buff[LCD_ST7789_WIDTH*LCD_ST7789_HEIGHT*2];

void LCD_Fill(unsigned short color)
{

#if 0
	for(unsigned int i = 0; i < LCD_ST7789_WIDTH; i++){
		for(unsigned int j = 0; j < LCD_ST7789_HEIGHT; j++){
			LCD_ST7789_SendByte((color >> 8) & 0xFF);
			LCD_ST7789_SendByte((color & 0xFF));
		}
	}
#else
	for(unsigned int i = 0; i < LCD_ST7789_WIDTH; i++){
		for(unsigned int j = 0; j < LCD_ST7789_HEIGHT; j++){
			unsigned int index = (i*LCD_ST7789_HEIGHT + j)*2;
			g_lcd_buff[index] = (color >> 8) & 0xFF;
			g_lcd_buff[index+1] = (color & 0xFF);
		}
	}
	LCD_ST7789_SendBytes(g_lcd_buff,LCD_ST7789_WIDTH*LCD_ST7789_HEIGHT*2);
#endif
}

void LCD_Test(void)
{
	LCD_Init();
	LCD_Fill(BLUE);
}

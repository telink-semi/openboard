#ifndef	__OLED_H
#define	__OLED_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define WHITE					0xFFFF
#define BLACK					0x0000
#define BLUE 					0x001F
#define BRED 					0XF81F
#define GRED 					0XFFE0
#define GBLUE					0X07FF
#define RED  					0xF800
#define MAGENTA				    0xF81F
#define GREEN					0x07E0
#define CYAN 					0x7FFF
#define YELLOW				    0xFFE0
#define BROWN					0XBC40
#define BRRED					0XFC07
#define GRAY 					0X8430
#define DARKBLUE			    0X01CF
#define LIGHTBLUE			    0X7D7C
#define GRAYBLUE                0X5458
#define LIGHTGREEN              0X841F
#define LGRAY 			        0XC618
#define LGRAYBLUE               0XA651
#define LBBLUE                  0X2B12

#define	LCD_ST7789_DC			PSPI_MISO_PIN
#define	LCD_ST7789_DC_0			gpio_set_low_level(LCD_ST7789_DC)
#define	LCD_ST7789_DC_1			gpio_set_high_level(LCD_ST7789_DC)
#define	LCD_ST7789_CN			PSPI_CN_PIN
#define	LCD_ST7789_CN_0			gpio_set_low_level(LCD_ST7789_CN)
#define	LCD_ST7789_CN_1			gpio_set_high_level(LCD_ST7789_CN)
#define HORIZONTAL 				0
#define VERTICAL   				1
#define LCD_ST7789_HEIGHT 		280
#define LCD_ST7789_WIDTH 		240

typedef struct{
    unsigned short WIDTH;
    unsigned short HEIGHT;
    unsigned int   SCAN_DIR;
}LCD_ST7789_ATTRIBUTES;
extern LCD_ST7789_ATTRIBUTES LCD_ST7789_ST;

void LCD_Init(void);
void LCD_Test(void);
void LCD_Fill(unsigned short color);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

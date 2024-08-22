#include "ws2812b.h"
#include "app_config.h"
void ws2812b_bit02();
void ws2812b_bit12();

volatile unsigned int g_ws2812b_color2[] = {0x00FF00,0x0000ff,0xff0000};

 void ws2812b_fill2(unsigned int color,unsigned int num)
{
	unsigned int r = core_interrupt_disable();
	for(unsigned int n = num; n > 0; n--){
		//BRG
		for (int i = 0; i < 24; i++) { // high order first send, send data in the order of 24bit GRB
			if (color & BIT(i)) {
				ws2812b_bit12();
			} else {
				ws2812b_bit02();
			}
		}
	}
	gpio_set_low_level(WS2812B_IO2);
	delay_us(400);
	core_restore_interrupt(r);
}

void ws2812b_init2(void)
{
	gpio_function_en(WS2812B_IO2);
	gpio_output_en(WS2812B_IO2);
	gpio_input_dis(WS2812B_IO2);
}

__attribute__((aligned(4))) unsigned char GPIOB_READ2;

void ws2812b_test2(void)
{
	GPIOB_READ2 = read_reg8(0x8014030B);
	static unsigned int index = 0;
	ws2812b_fill2(0,WS2812B_NUM2);
	ws2812b_fill2(g_ws2812b_color2[index++%3],WS2812B_NUM2);
}

void ws2812b_set2(unsigned int color)
{
	GPIOB_READ2 = read_reg8(0x8014030B);
	ws2812b_fill2(0,WS2812B_NUM2);
	ws2812b_fill2(color,WS2812B_NUM2);
}


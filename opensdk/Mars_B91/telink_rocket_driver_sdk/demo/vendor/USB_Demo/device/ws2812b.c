#include "../app_config.h"
#include "ws2812b.h"

extern void ws2812b_bit0();
extern void ws2812b_bit1();
//GRB
volatile unsigned int g_ws2812b_color[] = {0x00ff00,0xff0000,0x0000ff,0x000000};

 void ws2812b_fill(unsigned int color,unsigned int num)
{
	unsigned int r = core_interrupt_disable();
	for(unsigned int n = num; n > 0; n--){
		for (int32_t i = 23; i >= 0; i--) { // high order first send, send data in the order of 24bit GRB
			if (color & BIT(i)) {
				ws2812b_bit1();
			} else {
				ws2812b_bit0();
			}
		}
	}
	gpio_set_low_level(WS2812B_IO);
	core_restore_interrupt(r);
	delay_us(500);
}

void ws2812b_init(void)
{
	gpio_function_en(WS2812B_IO);
	gpio_output_en(WS2812B_IO);
	gpio_input_dis(WS2812B_IO);
}

__attribute__((aligned(4))) unsigned char GPIOB_READ;

void ws2812b_test(void)
{
	GPIOB_READ = read_reg8(0x8014030B);
	static unsigned int index = 0;
	//ws2812b_fill(0,WS2812B_NUM);
	ws2812b_fill(g_ws2812b_color[index++%3],WS2812B_NUM);
}

void ws2812b_set(unsigned int color)
{
	GPIOB_READ = read_reg8(0x8014030B);
	ws2812b_fill(0,WS2812B_NUM);
	ws2812b_fill(color,WS2812B_NUM);
}

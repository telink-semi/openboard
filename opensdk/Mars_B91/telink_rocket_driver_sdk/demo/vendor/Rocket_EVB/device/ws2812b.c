#include "../app_config.h"
#include "ws2812b.h"

void ws2812b_bit0();
void ws2812b_bit1();

volatile unsigned int g_ws2812b_color[] = {0xffffff,0xff0000,0x00ff00,0x0000ff};

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
	delay_us(400);
	core_restore_interrupt(r);
}


void ws2812b_init(void)
{
	gpio_function_en(WS2812B_IO);
	gpio_output_en(WS2812B_IO);
	gpio_input_dis(WS2812B_IO);
	ws2812b_fill(0,WS2812B_NUM);
}

__attribute__((aligned(4))) unsigned char GPIOB_READ;


void ws2812b_test(void)
{
	GPIOB_READ = read_reg8(0x8014030B);
	static unsigned int index = 0;
	ws2812b_fill(g_ws2812b_color[index++%4],WS2812B_NUM);
}

void ws2812b_set(unsigned int color)
{
	GPIOB_READ = read_reg8(0x8014030B);
	ws2812b_fill(0,WS2812B_NUM);
	ws2812b_fill(color,WS2812B_NUM);
}


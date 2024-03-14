#include "../app_config.h"
#include "ws2812b.h"

#define ONE_BIT(PIN) do { 								\
	gpio_set_high_level(PIN);							\
	__asm volatile ("nop");	gpio_set_high_level(PIN);__asm volatile ("nop");   \
	gpio_set_low_level(PIN);							\
	__asm volatile ("nop");								\
} while (0)

#define ZERO_BIT(PIN) do { 								\
	gpio_set_high_level(PIN);							\
	__asm volatile ("nop");								\
	gpio_set_low_level(PIN);							\
	__asm volatile ("nop");	gpio_set_low_level(PIN);__asm volatile ("nop");    \
} while (0)

#define RESET_BIT(PIN) do { 							\
	gpio_set_low_level(PIN);							\
	delay_us(100);										\
} while (0)

volatile unsigned int g_ws2812b_color[] = {0xffffff,0xff0000,0x00ff00,0x0000ff};

void ws2812b_fill(unsigned int color,unsigned int num)
{
	unsigned int r = core_interrupt_disable();
	for(unsigned int n = num; n > 0; n--){
		for (int32_t i = 23; i >= 0; i--) { // high order first send, send data in the order of 24bit GRB
			if (color & BIT(i)) {
				ONE_BIT(WS2812B_IO);
			} else {
				ZERO_BIT(WS2812B_IO);
			}
		}
	}
	RESET_BIT(WS2812B_IO);
	core_restore_interrupt(r);
}

void ws2812b_init(void)
{
	gpio_function_en(WS2812B_IO);
	gpio_output_en(WS2812B_IO);
	gpio_input_dis(WS2812B_IO);
	ws2812b_fill(0x000000,WS2812B_MAX);
}

void ws2812b_test(void)
{
	static unsigned int index = 0;
	ws2812b_fill(g_ws2812b_color[index++%4],WS2812B_NUM);
	delay_ms(500);
}



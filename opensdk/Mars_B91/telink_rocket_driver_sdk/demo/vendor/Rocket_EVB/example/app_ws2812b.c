#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_WS2812B)
#include "../device/ws2812b.h"
volatile unsigned int  g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init(1000000);
	ws2812b_init();
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time1,100*1000)){
			ws2812b_test();
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

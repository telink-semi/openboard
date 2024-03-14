#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_LCD)
#include "../device/st7789.h"

volatile unsigned int  g_time0;
volatile unsigned int  g_time1;
unsigned int g_index = 0;
unsigned short g_color[] = {WHITE,BLACK,BLUE,BRED,GRED};
void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	LCD_Init();
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time0,100*1000)){
			gpio_toggle(LED);
			LCD_Fill(g_color[g_index++%5]);
			g_time0 = stimer_get_tick();
		}
	}
}

#endif

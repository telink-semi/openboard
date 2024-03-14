#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_ADC)
#include "../device/photoresistor.h"
volatile unsigned int  g_time0;
volatile unsigned int  g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init();
	photoresistor_init();
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time1,200*1000)){
			if(get_res_value() > 9000){
				gpio_set_high_level(LED);
			}
			else{
				gpio_set_low_level(LED);
			}
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

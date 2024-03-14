#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_UART)
#include "../device/serial_port.h"
volatile unsigned int  g_time0;
volatile unsigned int  g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init();
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		uart_rec_handle(uart_send_array);
		if(clock_time_exceed(g_time0,500*1000)){
			gpio_toggle(LED);
			g_time0 = stimer_get_tick();
		}
	}
}

#endif

#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_KEY)
#include "../device/button.h"
volatile unsigned int  g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	init_button();
	binding_button();
	serial_port_init(1000000);
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

extern struct Button btn_left;
extern struct Button btn_mid;
extern struct Button btn_right;
void BTN_SINGLE_CLICK_Handler(void* btn){
	if(btn == &btn_left){
		gpio_toggle(LED);
	}
	else if(btn == &btn_mid){
		gpio_toggle(LED);
	}
	else if(btn == &btn_right){
		gpio_toggle(LED);
	}
}

//void BTN_DOUBLE_CLICK_Handler(void* btn){
//	if(btn == &btn_left){
//		gpio_toggle(LED);delay_ms(200);gpio_toggle(LED);
//	}
//	else if(btn == &btn_mid){
//		gpio_toggle(LED);delay_ms(200);gpio_toggle(LED);
//	}
//	else if(btn == &btn_right){
//		gpio_toggle(LED);delay_ms(200);gpio_toggle(LED);
//	}
//}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time1,TICKS_INTERVAL*1000)){
			button_ticks();
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

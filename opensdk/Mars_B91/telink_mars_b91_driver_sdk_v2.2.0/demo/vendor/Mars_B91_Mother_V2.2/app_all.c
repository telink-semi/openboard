#include "app_config.h"
#if (EXAMPLE_SELECT == EXAMPLE_ALL)
#include "device/oled.h"
#include "device/sht30.h"
#include "device/lis2dh.h"
#include "device/button.h"
#include "device/ws2812b.h"
#include "device/serial_port.h"
#include "device/photoresistor.h"
#include "printf.h"
volatile unsigned int  g_time0;
volatile unsigned int  g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
    i2c_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);
    i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));
	OLED_Init();
	OLED_Show_Arrow(0);
	Lis2dh_Init();
	init_button();
	binding_button();
	ws2812b_init();
	serial_port_init();
	photoresistor_init();
	core_interrupt_enable();
	printf("Mar_%s","B91");
	g_time0 = stimer_get_tick();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		uart_rec_handle(uart_send_array);
		if(clock_time_exceed(g_time0,100*1000)){
			sht3x_test();
			get_res_value();
			get_acc_value(&g_axis_info);
			g_time0 = stimer_get_tick();
		}
		if(clock_time_exceed(g_time1,TICKS_INTERVAL*1000)){
			button_ticks();
			ws2812b_test();
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

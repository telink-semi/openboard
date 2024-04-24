#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_LIS2DHTR)
#include "../device/lis2dh.h"
volatile unsigned int g_time1;

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init(1000000);
    i2c_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);
    i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));
	Lis2dh_Init();
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time1,100*1000)){
			get_acc_value(&g_axis_info);
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

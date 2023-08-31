#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_SHT30)
#include "../device/oled.h"
#include "../device/sht30.h"
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
	core_interrupt_enable();
	g_time1 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time1,100*1000)){
			sht3x_test();
			g_time1 = stimer_get_tick();
		}
	}
}

#endif

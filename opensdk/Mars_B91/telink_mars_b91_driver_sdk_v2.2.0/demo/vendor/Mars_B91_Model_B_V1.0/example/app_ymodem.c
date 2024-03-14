#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_YMODEM)
#include "../device/sdcard.h"
#include "../common/ymodem.h"
#include "../device/sys_norflash.h"
#include "../ff15/extend/fatfs_extend.h"

volatile unsigned int  g_time0;

int putchar(int ch)
{
	uart_send_byte(UART0, ch);
	uart_rtx_pin_tx_trig(UART0);
    return ch;
}

void Ymodem_init_pre(void)
{
	serial_port_init();
	while(SD_Card_init()){
		printf("SD_Card_init error\r\n");
		delay_ms(100);
	}
	while(f_mount(&g_fs[0],"0:",1)){ // mount SD card
		printf("mount SD card error\r\n");
		delay_ms(100);
	}
	printf("Ymodem_init_pre\r\n");
}

unsigned int g_test = 1;
void ReadRecive(char* data,uint32_t len){
	if(12 == len && data[0] == 'a' && data[1] == 'r' && data[2] == 'e'){
		uart_send_str("ojbk");
		g_test = 0;
	}
	else{
		printf("len:%ld\r\n",len);
	}
}

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	Ymodem_init(Ymodem_init_pre);
	core_interrupt_enable();
	g_time0 = stimer_get_tick();
	while(g_test){
		uart_rec_handle(ReadRecive);
		delay_ms(100);
	}
	Ymodem_receive();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time0,500*1000)){
			gpio_toggle(LED);
			printf("main_loop\r\n");
			g_time0 = stimer_get_tick();
		}
	}
}

#endif

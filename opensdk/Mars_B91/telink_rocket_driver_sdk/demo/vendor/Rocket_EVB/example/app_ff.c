#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_FATFS)

#include "../device/sys_norflash.h"
#include "../telink_lua_lib/ff15/extend/tlkfs_disk.h"
#include "../telink_lua_lib/ff15/extend/fatfs_extend.h"

// http://elm-chan.org/fsw/ff/
#define		HELLO_STR					("\"Thank you for using Telink Mars_B91 ^_^\"")

volatile unsigned int  g_time0;

#include "../lua_interface/my_lua_interface.h"
//extern int init_fs(void);

static void ff_test(void)
{
	unsigned int tot_sect;
	unsigned int fre_sect;
	init_fs();
	ff_scan_files("0:");
	ff_exf_getfree("0:",&tot_sect,&fre_sect);
	ff_unlink("0:/test1.txt");
	ff_write_file("0:/test1.txt",HELLO_STR);
	ff_read_file("0:/test1.txt");
}

__attribute__((weak)) int putchar(int ch)
{
	uart_send_byte(UART0, ch);
	uart_rtx_pin_tx_trig(UART0);
    return ch;
}

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init(1000000);
	ff_test();
	core_interrupt_enable();
	g_time0 = stimer_get_tick();
}

void main_loop(void)
{
	while(1){
		if(clock_time_exceed(g_time0,500*1000)){
			gpio_toggle(LED);
			g_time0 = stimer_get_tick();
		}
	}
}

#endif

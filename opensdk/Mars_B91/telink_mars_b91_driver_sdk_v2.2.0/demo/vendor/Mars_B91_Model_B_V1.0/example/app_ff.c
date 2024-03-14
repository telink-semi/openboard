#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_FATFS)
#include "../device/sdcard.h"
#include "../device/sys_norflash.h"
#include "../ff15/extend/fatfs_extend.h"

// http://elm-chan.org/fsw/ff/
#define 	SD_NAND_FLASH_SUPPORT		1

#define		HELLO_STR					("\"Thank you for using Mars_B91_Model_B ^_^\"")

volatile unsigned int  g_time0;

__attribute__((unused)) static void ff_test(void)
{
#if(SD_NAND_FLASH_SUPPORT)
	while(f_mount(&g_fs[0],"0:",1)){ // mount SD card
		printf("mount SD card error\r\n");
	}
	ff_scan_files("0:");
	ff_exf_getfree("0:");
	ff_unlink("0:/test1.txt");
	ff_write_read_file("0:/test1.txt",HELLO_STR);
#endif
	while(f_mount(&g_fs[1],"1:",1)){ // mount internal flash
		printf("mount internal flash error, try to format the Flash\r\n");
		fs_mkfs("1:");// the first time you mount Flash, you must first perform formatting
		delay_ms(500);
	}
	ff_scan_files("1:");
	ff_exf_getfree("1:");
	ff_unlink("1:/test1.txt");
	ff_write_file("1:/test1.txt",HELLO_STR);
	ff_read_file("1:/test1.txt");
#if(3 == FF_VOLUMES)
	while(f_mount(&g_fs[2],"2:",1)){ // mount internal RAM
		uart_send_str("mount internal RAM error, try to format the RAM\r\n");
		fs_mkfs("2:");
		delay_ms(500);
	}
	ff_scan_files("2:");
	ff_exf_getfree("2:");
	ff_unlink("2:/test1.txt");
	ff_unlink("2:/test2.txt");
	ff_write_file("2:/test1.txt",HELLO_STR);
	ff_read_file("2:/test1.txt");
#endif
}

int putchar(int ch)
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
	serial_port_init();
#if(SD_NAND_FLASH_SUPPORT)
	while(SD_Card_init()){
		printf("SD_Card_init error\r\n");
		delay_ms(100);
	}
#endif
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

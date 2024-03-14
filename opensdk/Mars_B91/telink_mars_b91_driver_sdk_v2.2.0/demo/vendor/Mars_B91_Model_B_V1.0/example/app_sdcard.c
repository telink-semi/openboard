#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_SDCARD)
#include "../device/sdcard.h"
volatile unsigned int  g_time0;

void SD_Read_Sectorx(unsigned int sec)
{
    char buf[512];
	unsigned int i;

	i = SD_GetSectorCount();
	sprintf(buf,"SD Card Size:  %d MB\r\n",i>>11);
	uart_send_array(buf,strlen(buf));

	for(i = 0; i < 512; i++)	buf[i] = '1';
	unsigned char res = SD_WriteDisk((unsigned char*)buf,sec,1);
	sprintf((char*)buf,"res:  %d\r\n",res);
	uart_send_array(buf,strlen(buf));

	if(SD_ReadDisk((unsigned char*)buf,sec,1) == 0)
	{
		uart_send_array("SECTOR 0 DATA:\r\n",strlen("SECTOR 0 DATA:\r\n"));
		for(i = 0; i < 512; i++)	uart_send_byte(UART0, buf[i]);
		uart_send_array("\r\nDATA ENDED\r\n",strlen("\r\nDATA ENDED\r\n"));
	}
}

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init();
	uart_send_array("hello world\r\n",strlen("hello world\r\n"));
	SD_Card_init();
	SD_Read_Sectorx(0);
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

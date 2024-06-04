#include "my_lua_interface.h"
#include "../common/my_malloc.h"

int putchar(int ch)
{
	uart_send_byte(UART0, ch);
	uart_rtx_pin_tx_trig(UART0);
    return ch;
}

void lua_uart_init()
{
	unsigned short div;
	unsigned char bwpc;
    uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2, UART0_RX_PB3);
	uart_rtx_en(UART0);
	uart_cal_div_and_bwpc(1000000, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	//uart_tx_irq_trig_level(UART0, 0);
	uart_rx_irq_trig_level(UART0, 1);
	uart_set_irq_mask(UART0, UART_RX_IRQ_MASK|UART_ERR_IRQ_MASK);
	plic_interrupt_enable(IRQ19_UART0);
}
//
void Ymodem_send_byte(unsigned char data)
{
	uart_send_byte(UART0, data);
	uart_rtx_pin_tx_trig(UART0);
}
//
unsigned int Ymodem_read_data(unsigned char *read_buf,unsigned int max_read){
	unsigned int stamp = stimer_get_tick();
	unsigned int read_index = 0;
	// 先清空RXFIFO 再进行接收
	do{ //一定时间内没收到数据 则认为一笔收据接收完成
		if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS))
		{
			if(read_index < max_read){
				read_buf[read_index++] = uart_read_byte(UART0);
				stamp = stimer_get_tick();
			}
			else{
				uart_read_byte(UART0);
				stamp = stimer_get_tick();
			}
		}
	}
	while(!clock_time_exceed(stamp,10*1000));// 两个字节最大间隔 时间可以放久一点
	return read_index;
}

unsigned int Ymodem_read_data_wait(unsigned char *read_buf,unsigned int max_read,unsigned int wait_time){
	unsigned int read_size = 0;
	unsigned int stamp = stimer_get_tick();
	do{
		read_size = Ymodem_read_data(read_buf,max_read);
	}while((!clock_time_exceed(stamp,wait_time))&&(0==read_size));
	return read_size;
}

tlkfs_disk_t FsDiskFlash = {
	1,  // isReady;
	8,  // blockSize;
	512,// sectorSize;
	0,  // sectorCount;
	nullptr,
	sys_norflash_read_lba,
	sys_norflash_write_lba,
};

int init_fs(void){
	FsDiskFlash.sectorCount = SYS_NORFLASH_DISK_SIZE/FsDiskFlash.sectorSize;
	tlkfs_addDisk((tlkfs_disk_t*)&FsDiskFlash,0);
	while(f_mount(&g_fs[0],"0:",1)){ // mount internal flash
		printf("mount internal flash error,try formatting the disk and retry mount internal flash\r\n");
		fs_mkfs("0:");// the first time you mount Flash, you must first perform formatting
		delay_ms(50);
	}
	return 0;
}
// 指定复位后执行脚本的盘符，不过在下载时选定哪个盘符 就会临时指定在哪个盘符执行
unsigned char set_script_volNum()
{
	return 0;
}

unsigned int irq_uart_disable(void)
{
	plic_interrupt_disable(IRQ19_UART0);
	return 0;
}

void irq_uart_enable(void)
{
	plic_interrupt_enable(IRQ19_UART0);
}
//
_m_lua_interface lua_interface =
{
	.lua_uart_init = lua_uart_init,
	.Ymodem_send_byte = Ymodem_send_byte,
	.Ymodem_read_data_wait = Ymodem_read_data_wait,
	.lua_irq_uart_disable  = irq_uart_disable,
	.lua_irq_uart_enable = irq_uart_enable,
	.init_fs   = init_fs,
	.mem_init  = vPortInitialiseBlocks,
	.myfree    = vPortFree,
	.mymalloc  = pvPortMalloc,
	.myrealloc = pvPortReMalloc,
	.set_script_volNum = set_script_volNum,
};


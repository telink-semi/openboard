#ifndef _LUA_INTERFACE_DEFINED
#define _LUA_INTERFACE_DEFINED

#include "driver.h"

#include "../device/sdcard.h"
#include "../device/sys_norflash.h"
#include "../telink_lua_lib/ff15/extend/tlkfs_disk.h"
#include "../telink_lua_lib/ff15/extend/fatfs_extend.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void (*lua_uart_init)(void);
	void (*Ymodem_send_byte)(unsigned char data);
	unsigned int (*Ymodem_read_data_wait)(unsigned char *read_buf,unsigned int max_read,unsigned int wait_time);
	unsigned int (*lua_uart_irq_disable)(void);
	int (*init_fs)(void);
	void (*mem_init)(void);
	void  (*myfree)(void *ptr);
	void* (*mymalloc)(unsigned int size);
	void* (*myrealloc)(void *ptr,unsigned int size);
	unsigned char (*set_script_volNum)(void);
}_m_lua_interface;

extern _m_lua_interface lua_interface;
int init_fs(void);
#ifdef __cplusplus
}
#endif

#endif

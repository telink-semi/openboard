#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_LUA)
#include "../device/sdcard.h"
#include "../device/sys_norflash.h"
#include "../ff15/extend/fatfs_extend.h"

#include "../common/malloc.h"
#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

volatile unsigned int  g_time0;

const char LUA_SCRIPT_GLOBAL[] ="local a = 1 repeat a = a *1.01 print('a:'..a) led_toggle() print_mem_perused() until(false)";

__attribute__((unused)) static int lua_led_toggle(lua_State *L)
{
	(void)L;
	gpio_toggle(LED);
	delay_ms(100);
    return 0;
}

__attribute__((unused)) static int lua_print_mem_perused(lua_State *L)
{
	(void)L;
	printf("mem_perused:%d\r\n",mem_perused());
    return 0;
}

__attribute__((unused)) static const struct luaL_Reg lua_lib[]=
{
	{"led_toggle",lua_led_toggle},
	{"print_mem_perused",lua_print_mem_perused},
    {NULL,NULL}
};

__attribute__((unused)) void myLua_register(lua_State *L)
{
	unsigned int index = 0;
	do{
		lua_register(L,(lua_lib[index].name),(lua_lib[index].func));
	}while(lua_lib[index++].name);
}

__attribute__((unused)) void Lua_load_string(const char * str)
{
	lua_State *L = luaL_newstate();
	if(L){
		luaL_openlibs(L);
		myLua_register(L);
		int ret = luaL_dostring(L, str);
		if(ret){
			if (ret == LUA_ERRSYNTAX)
				printf("Error: %s\r\n", lua_tostring(L, -1));
			else if (ret == LUA_ERRMEM)
				printf("Error: memory error\r\n");
			else
				printf("Error: syntax error:%s\r\n",lua_tostring(L, -1));
		}
		lua_close(L);
	}
}

__attribute__((unused)) void Lua_load_file(const char *path)
{
    lua_State *L = luaL_newstate();
    if(L){
		luaL_openlibs(L);
		myLua_register(L);
		int ret = luaL_dofile(L, path);
		if(ret){
			if (ret == LUA_ERRSYNTAX)
				printf("Error: %s\r\n", lua_tostring(L, -1));
			else if (ret == LUA_ERRMEM)
				printf("Error: memory error\r\n");
			else
				printf("Error: syntax error:%s\r\n",lua_tostring(L, -1));
		}
		lua_close(L);
    }
}

__attribute__((unused)) static void lua_test(void)
{
	while(f_mount(&g_fs[0],"0:",1)){ // mount SD card
		uart_send_str("mount SD card error\r\n");
		delay_ms(100);
	}
	ff_scan_files("0:");
	ff_exf_getfree("0:");
 	ff_unlink("0:/hello.lua");
	ff_write_file("0:/hello.lua",LUA_SCRIPT_GLOBAL);
	ff_read_file("0:/hello.lua");
	Lua_load_file("0:/hello.lua");
	//Lua_load_string(LUA_SCRIPT_GLOBAL);
}

void user_init()
{
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	serial_port_init();
	printf("\"Thank you for using Mars_B91_Model_B ^_^\"");
 	mem_init();
	while(SD_Card_init()){
		uart_send_str("SD_Card_init error\r\n");
		delay_ms(100);
	}
	lua_test();
	core_interrupt_enable();
	g_time0 = stimer_get_tick();
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

int putchar(int ch)
{
	uart_send_byte(UART0, ch);
	uart_rtx_pin_tx_trig(UART0);
    return ch;
}


#endif

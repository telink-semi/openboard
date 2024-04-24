#include "lua_extend.h"

lua_State *g_L;
//
int c_call_lua(const char * fun){
	lua_getglobal(g_L, fun);
	if (lua_pcall(g_L, 0, 0, 0) != LUA_OK) {
		printf("Error: %s\n", lua_tostring(g_L, -1));
		return -1;
	}
	return 0;
}
//
void luaL_mylibs(const struct luaL_Reg lua_lib[])
{
	unsigned int index = 0;
	do{
		lua_register(g_L,(lua_lib[index].name),(lua_lib[index].func));
	}while(lua_lib[index++].name);
}
//
__attribute__((unused)) void Lua_load_string(const char * str,const struct luaL_Reg lua_lib[])
{
	g_L = luaL_newstate();
	if(g_L){
		luaL_openlibs(g_L);
		luaL_mylibs(lua_lib);
		int ret = luaL_dostring(g_L, str);
		if(ret){
			if (ret == LUA_ERRSYNTAX)
				printf("Error: %s\r\n", lua_tostring(g_L, -1));
			else if (ret == LUA_ERRMEM)
				printf("Error: memory error\r\n");
			else
				printf("Error: syntax error:%s\r\n",lua_tostring(g_L, -1));
		}
		lua_close(g_L);
	}
}
//
int Lua_load_file(const char *path,const struct luaL_Reg lua_lib[])
{
	g_L = luaL_newstate();
    if(g_L){
		luaL_openlibs(g_L);
		luaL_mylibs(lua_lib);
		int ret = luaL_dofile(g_L, path);
		if(ret){
			if (ret == LUA_ERRSYNTAX)
				printf("Error: %s\r\n", lua_tostring(g_L, -1));
			else if (ret == LUA_ERRMEM)
				printf("Error: memory error\r\n");
			else
				printf("Error: syntax error:%s\r\n",lua_tostring(g_L, -1));
			return ret;
		}
		return 0;
    }
    return -1;
}
//
void Ymodem_File_Handle(void)
{
	Ymodem_send_str("reset");
	unsigned int read_size = 0;
	do{
		read_size = ymodem_lua_interface->Ymodem_read_data_wait(Ymodem_rec_buf,YMODEM_REC_LEN,YMODEM_RECV_TIMEOVER_MS*1000);
		Ymodem_HandShake_Handle(Ymodem_rec_buf,read_size);
		if(g_YmodemInfo.state == Ymode_Receive_HandShake)  Ymodem_receive();
		else if(g_YmodemInfo.state == Ymode_Send_FileName) Ymodem_send();
		g_YmodemInfo.state = Ymode_Idle;
	}while(read_size);
}
//
int telink_lua_lib_init(const struct luaL_Reg lua_lib[], _m_lua_interface* interface)
{
	Ymodem_pre(interface);
	Ymodem_File_Handle();
	char path[12];
	sprintf(path,"%s","0:/main.lua");
	path[0] = get_script_volNum() + '0';
	return Lua_load_file((const char *)path,lua_lib) | c_call_lua("setup");
}
//
int telink_lua_main_loop(void)
{
	return c_call_lua("loop");
}


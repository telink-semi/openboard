#include "../app_config.h"
#if(EXAMPLE_SELECT == EXAMPLE_LUA_FS)

#include "../lua_interface/my_lua_reg.h"
#include "../lua_interface/my_lua_interface.h"
#include "../telink_lua_lib/lua/lua_extend.h"

void user_init()
{
	telink_lua_lib_init(lua_lib,&lua_interface);
}

void main_loop(void)
{
	while(1){
		telink_lua_main_loop();
	}
}

#endif

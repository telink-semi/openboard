#ifndef _LUA_EXTEND_DEFINED
#define _LUA_EXTEND_DEFINED

#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"
#include "../ymodem/ymodem.h"

#ifdef __cplusplus
extern "C" {
#endif

int c_call_lua(const char * fun);

int c_call_lua_one_par(const char * fun,int num);

int telink_lua_lib_init(const struct luaL_Reg lua_lib[],_m_lua_interface* interface);

void telink_lua_main_loop(void);

#ifdef __cplusplus
}
#endif

#endif

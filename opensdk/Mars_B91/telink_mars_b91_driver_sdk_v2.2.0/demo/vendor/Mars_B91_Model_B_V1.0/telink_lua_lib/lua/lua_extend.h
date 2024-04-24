#ifndef _LUA_EXTEND_DEFINED
#define _LUA_EXTEND_DEFINED

#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"
#include "../ymodem/ymodem.h"

#ifdef __cplusplus
extern "C" {
#endif

int telink_lua_lib_init(const struct luaL_Reg lua_lib[],_m_lua_interface* interface);

int telink_lua_main_loop(void);

#ifdef __cplusplus
}
#endif

#endif

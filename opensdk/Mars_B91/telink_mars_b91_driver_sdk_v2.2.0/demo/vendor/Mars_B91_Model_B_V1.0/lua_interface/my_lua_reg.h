#ifndef _LUA_REG_DEFINED
#define _LUA_REG_DEFINED

#include "driver.h"
#include "../telink_lua_lib/lua/lua.h"
#include "../telink_lua_lib/lua/lauxlib.h"

#ifdef __cplusplus
extern "C" {
#endif

extern struct luaL_Reg lua_lib[];

#ifdef __cplusplus
}
#endif

#endif

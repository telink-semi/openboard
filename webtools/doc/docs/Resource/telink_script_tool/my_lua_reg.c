#include "my_lua_reg.h"
#include "my_malloc.h"

#define OPERATION  <	// <  OR !=

#define LUA_FUN(fun_name)  __attribute__((unused)) static int fun_name(lua_State *L)


LUA_FUN(lua_get_mem_perused)
{
	(void)L;
	unsigned char temp = mem_perused();
	lua_pushinteger(L,temp);
    return 1;
}

LUA_FUN(lua_gpio_function_en)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_output_en]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_function_en(gpio);
    return 0;
}

LUA_FUN(lua_gpio_function_dis)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_output_en]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_function_dis(gpio);
    return 0;
}

LUA_FUN(lua_gpio_output_en)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_output_en]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_output_en(gpio);
    return 0;
}

LUA_FUN(lua_gpio_output_dis)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_output_dis]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_function_en(gpio);
	gpio_output_dis(gpio);
    return 0;
}

LUA_FUN(lua_gpio_input_en)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_input_en]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_function_en(gpio);
	gpio_input_en(gpio);
    return 0;
}

LUA_FUN(lua_gpio_input_dis)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_input_dis]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_function_en(gpio);
	gpio_input_dis(gpio);
    return 0;
}

LUA_FUN(lua_gpio_toggle)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_toggle]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_toggle(gpio);
    return 0;
}

LUA_FUN(lua_gpio_set_high_level)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_high]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_set_high_level(gpio);
    return 0;
}

LUA_FUN(lua_gpio_set_low_level)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_low]\r\n");
		return 0;
	}
	int gpio = lua_tonumber(L, 1);
	gpio_set_low_level(gpio);
    return 0;
}

LUA_FUN(lua_delay_us)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_delay_us]\r\n");
		return 0;
   }
   int time = lua_tonumber(L, 1);
   delay_us(time);
   return 0;
}

LUA_FUN(lua_delay_ms)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_delay_ms]\r\n");
		return 0;
   }
   int time = lua_tonumber(L, 1);
   delay_ms(time);
   return 0;
}

LUA_FUN(lua_analog_write_reg8)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_analog_write_reg8]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1); //float
   unsigned char data = lua_tonumber(L, 2);
   analog_write_reg8(addr,data);
   return 0;
}

LUA_FUN(lua_analog_write_reg16)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_analog_write_reg16]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1); //float
   unsigned short data = lua_tonumber(L, 2);
   analog_write_reg16(addr,data);
   return 0;
}

LUA_FUN(lua_analog_write_reg32)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_analog_write_reg32]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1);
   unsigned int data = lua_tonumber(L, 2);
   analog_write_reg32(addr,data);
   return 0;
}

LUA_FUN(lua_analog_write_reg8_buff)
{
	int n = lua_gettop(L);

	if(n OPERATION 3)
	{
		printf("[PARAMETER ERROR: lua_analog_write_reg8_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tonumber(L, 3);
	if(len > table_len){
		printf("[PARAMETER ERROR: lua_analog_write_reg8_buff,table length is too short]\r\n");
	}
	else if(len > 1024){
		printf("[PARAMETER ERROR: lua_analog_write_reg8_buff,table length greater than 1024]\r\n");
	}
	else{
		unsigned char buf[1024];
		for (unsigned int i = 1; i <= len; i++) {
		   lua_pushinteger(L, i);
		   lua_gettable(L, 2);
		   if (lua_isnumber(L, -1)) {
			   buf[i - 1] =  lua_tonumber(L, -1);
		   } else {
			   printf("Element %d: %s\n", i, lua_tostring(L, -1));
		   }
		   lua_pop(L, 1);
		}
		analog_write_buff(addr,buf,len);
	}
	return 0;
}

LUA_FUN(lua_analog_read_reg8_buff)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_analog_read_reg8_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);
	unsigned int len  = lua_tonumber(L, 2);
	if(len > 1024){
		printf("[PARAMETER ERROR: lua_analog_write_buff,table length greater than 1024]\r\n");
	}
	unsigned char buf[1024];
	analog_read_buff(addr,buf,len);
	lua_newtable(L);
	for(unsigned int i = 0; i < len; i++){
		lua_pushinteger(L,buf[i]);
		lua_rawseti(L,-2,i+1);
	}
	lua_pushinteger(L,len);
	return 2;
}

LUA_FUN(lua_analog_read_reg8)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_analog_read_reg8]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1); //float
   unsigned char data = analog_read_reg8(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_analog_read_reg16)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_analog_read_reg16]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1); //float
   unsigned short data = analog_read_reg16(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_analog_read_reg32)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_analog_read_reg32]\r\n");
		return 0;
   }
   unsigned char addr = lua_tonumber(L, 1); //float
   unsigned int data = analog_read_reg32(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_write_sram8)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_sram8]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned char data = lua_tonumber(L, 2);
   write_sram8(addr,data);
   return 0;
}

LUA_FUN(lua_write_sram16)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_sram16]\r\n");
		return 0;
   }
   unsigned int addr  = lua_tonumber(L, 1);
   unsigned short data = lua_tonumber(L, 2);
   write_sram16(addr,data);
   return 0;
}

LUA_FUN(lua_write_sram32)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_sram32]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned int data  = lua_tonumber(L, 2);
   write_sram32(addr,data);
   return 0;
}

LUA_FUN(lua_write_sram8_buff)
{
	int n = lua_gettop(L);

	if(n OPERATION 3)
	{
		printf("[PARAMETER ERROR: lua_write_sram8_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tonumber(L, 3);
	if(len > table_len){
		printf("[PARAMETER ERROR: lua_write_sram8_buff,table length is too short]\r\n");
	}
	else{
		for (unsigned int i = 1; i <= len; i++) {
		   lua_pushinteger(L, i);
		   lua_gettable(L, 2);
		   if (lua_isnumber(L, -1)) {
			   write_sram8(addr + i - 1, lua_tonumber(L, -1));
		   } else {
			   printf("Element %d: %s\n", i, lua_tostring(L, -1));
		   }
		   lua_pop(L, 1);
		}
	}
	return 0;
}

LUA_FUN(lua_write_sram16_buff)
{
	int n = lua_gettop(L);

	if(n OPERATION 3)
	{
		printf("[PARAMETER ERROR: lua_write_sram16_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tonumber(L, 3);
	if(len > table_len){
		printf("[PARAMETER ERROR: lua_write_sram16_buff,table length is too short]\r\n");
	}
	else{
		for (unsigned int i = 1; i <= len; i++) {

		   lua_pushinteger(L, i);
		   lua_gettable(L, 2);
		   if (lua_isnumber(L, -1)) {
			   write_sram16(addr + (i - 1) *2, lua_tonumber(L, -1));
		   } else {
			   printf("Element %d: %s\n", i, lua_tostring(L, -1));
		   }
		   lua_pop(L, 1);
		}
	}
	return 0;
}

LUA_FUN(lua_write_sram32_buff)
{
	int n = lua_gettop(L);

	if(n OPERATION 3)
	{
		printf("[PARAMETER ERROR: lua_write_sram32_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tonumber(L, 3);
	if(len > table_len){
		printf("[PARAMETER ERROR: lua_write_sram32_buff,table length is too short]\r\n");
	}
	else{
		for (unsigned int i = 1; i <= len; i++) {
		   lua_pushinteger(L, i);
		   lua_gettable(L, 2);
		   if (lua_isnumber(L, -1)) {
			   write_sram32(addr + (i - 1) *4, lua_tonumber(L, -1));
		   } else {
			   printf("Element %d: %s\n", i, lua_tostring(L, -1));
		   }
		   lua_pop(L, 1);
		}
	}
	return 0;
}

LUA_FUN(lua_read_sram8)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_sram8]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned char data = read_sram8(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_read_sram16)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_sram16]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned short data = read_sram16(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_read_sram32)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_sram32]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned int data = read_sram32(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_read_sram8_buff)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_read_sram8_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);
	unsigned int len  = lua_tonumber(L, 2);
	lua_newtable(L);
	for(unsigned int i = 0; i < len; i++){
		unsigned char value = read_sram8(addr+i);
		lua_pushinteger(L,value);
		lua_rawseti(L,-2,i+1);
	}
	lua_pushinteger(L,len);
	return 2;
}

LUA_FUN(lua_read_sram16_buff)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_read_sram16_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);
	unsigned int len  = lua_tonumber(L, 2);
	lua_newtable(L);
	for(unsigned int i = 0; i < len; i++){
		unsigned short value = read_sram16(addr+i*2);
		lua_pushinteger(L,value);
		lua_rawseti(L,-2,i+1);
	}
	lua_pushinteger(L,len);
	return 2;
}

LUA_FUN(lua_read_sram32_buff)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_read_sram32_buff]\r\n");
		return 0;
	}
	unsigned int addr = lua_tonumber(L, 1);
	unsigned int len  = lua_tonumber(L, 2);
	lua_newtable(L);
	for(unsigned int i = 0; i < len; i++){
		unsigned int value = read_sram32(addr+i*4);
		lua_pushinteger(L,value);
		lua_rawseti(L,-2,i+1);
	}
	lua_pushinteger(L,len);
	return 2;
}

LUA_FUN(lua_write_reg8)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_reg8]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned char data = lua_tonumber(L, 2);
   write_reg8(addr,data);
   return 0;
}

LUA_FUN(lua_write_reg16)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_reg16]\r\n");
		return 0;
   }
   unsigned int addr  = lua_tonumber(L, 1);
   unsigned short data = lua_tonumber(L, 2);
   write_reg16(addr,data);
   return 0;
}

LUA_FUN(lua_write_reg32)
{
   int n = lua_gettop(L);
   if(n OPERATION 2)
   {
		printf("[PARAMETER ERROR: lua_write_reg32]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned int data  = lua_tonumber(L, 2);
   write_reg32(addr,data);
   return 0;
}

LUA_FUN(lua_read_reg8)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_reg8]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned char data = read_reg8(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_read_reg16)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_reg16]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned short data = read_reg16(addr);
   lua_pushinteger(L,data);
   return 1;
}

LUA_FUN(lua_read_reg32)
{
   int n = lua_gettop(L);
   if(n OPERATION 1)
   {
		printf("[PARAMETER ERROR: lua_read_reg32]\r\n");
		return 0;
   }
   unsigned int addr = lua_tonumber(L, 1);
   unsigned int data = read_reg32(addr);
   lua_pushinteger(L,data);
   return 1;
}


struct luaL_Reg lua_lib[]=
{
	{"get_mem_perused",lua_get_mem_perused},

	{"delay_us",lua_delay_us},
	{"delay_ms",lua_delay_ms},

	{"gpio_function_en",lua_gpio_function_en},
	{"gpio_function_dis",lua_gpio_function_dis},
	{"gpio_set_output_en",lua_gpio_output_en},
	{"gpio_set_output_dis",lua_gpio_output_dis},
	{"gpio_set_input_en",lua_gpio_input_en},
	{"gpio_set_input_dis",lua_gpio_input_dis},

	{"gpio_high",lua_gpio_set_high_level},
	{"gpio_low",lua_gpio_set_low_level},
	{"gpio_toggle",lua_gpio_toggle},

	{"analog_write_reg8",lua_analog_write_reg8},
	{"analog_write_reg16",lua_analog_write_reg16},
	{"analog_write_reg32",lua_analog_write_reg32},
	{"analog_write_reg8_buff",lua_analog_write_reg8_buff},
	{"analog_read_reg8",lua_analog_read_reg8},
	{"analog_read_reg16",lua_analog_read_reg16},
	{"analog_read_reg32",lua_analog_read_reg32},
	{"analog_read_reg8_buff",lua_analog_read_reg8_buff},

	{"write_sram8",lua_write_sram8},
	{"write_sram16",lua_write_sram16},
	{"write_sram32",lua_write_sram32},
	{"write_sram8_buff",lua_write_sram8_buff},
	{"write_sram16_buff",lua_write_sram16_buff},
	{"write_sram32_buff",lua_write_sram32_buff},

	{"read_sram8",lua_read_sram8},
	{"read_sram16",lua_read_sram16},
	{"read_sram32",lua_read_sram32},
	{"read_sram8_buff",lua_read_sram8_buff},
	{"read_sram16_buff",lua_read_sram16_buff},
	{"read_sram32_buff",lua_read_sram32_buff},

	{"write_reg8",lua_write_reg8},
	{"write_reg16",lua_write_reg16},
	{"write_reg32",lua_write_reg32},
	{"read_reg8",lua_read_reg8},
	{"read_reg16",lua_read_reg16},
	{"read_reg32",lua_read_reg32},

    {NULL,NULL}
};

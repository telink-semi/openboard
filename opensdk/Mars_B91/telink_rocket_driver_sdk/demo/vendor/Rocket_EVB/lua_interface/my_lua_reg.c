#include "my_lua_reg.h"
#include "../common/my_malloc.h"

_attribute_ram_code_sec_ void timer0_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		timer_clr_irq_status(TMR_STA_TMR0);
		c_call_lua_one_par("irq_entry",IRQ4_TIMER0);
	}
}

_attribute_ram_code_sec_ void timer1_irq_handler(void)
{
	if(timer_get_irq_status(TMR_STA_TMR1))
	{
		timer_clr_irq_status(TMR_STA_TMR1);
		c_call_lua_one_par("irq_entry",IRQ3_TIMER1);
	}
}

_attribute_ram_code_sec_noinline_ void gpio_irq_handler(void)
{
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
	c_call_lua_one_par("irq_entry",IRQ25_GPIO);
}

_attribute_ram_code_sec_noinline_ void gpio_risc0_irq_handler(void)
{
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
	c_call_lua_one_par("irq_entry",IRQ26_GPIO2RISC0);
}

_attribute_ram_code_sec_noinline_ void gpio_risc1_irq_handler(void)
{
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
	c_call_lua_one_par("irq_entry",IRQ27_GPIO2RISC1);
}

PLIC_ISR_REGISTER(timer0_irq_handler, IRQ4_TIMER0)
PLIC_ISR_REGISTER(timer1_irq_handler, IRQ3_TIMER1)
PLIC_ISR_REGISTER(gpio_irq_handler, IRQ25_GPIO)
PLIC_ISR_REGISTER(gpio_risc0_irq_handler, IRQ26_GPIO2RISC0)
PLIC_ISR_REGISTER(gpio_risc1_irq_handler, IRQ27_GPIO2RISC1)

#define OPERATION  <	// <  OR !=

#define LUA_FUN(fun_name)  __attribute__((unused)) static int fun_name(lua_State *L)

extern int g_sys_debug_en;

int string_to_gpio_num(const char* str)
{
	int gpio_num = 0;
	if('P' == str[0] || 'p' == str[0]){
		if(str[1]>='a' && str[1]<='f') gpio_num = (str[1] - 'a') << 8 | BIT((str[2] - '0'));
		else if(str[1]>='A' && str[1]<='F') gpio_num = (str[1] - 'A') << 8 | BIT((str[2] - '0'));
	}
	if(0 == gpio_num)	printf("gpio_num is error\r\n");
	return gpio_num;
}
LUA_FUN(lua_SYS_DEBUG_EN)
{
	(void)L;
	g_sys_debug_en = 1;
	return 0;
}

LUA_FUN(lua_core_interrupt_enable)
{
	(void)L;
	core_interrupt_enable();
    return 0;
}

LUA_FUN(lua_core_interrupt_disable)
{
	unsigned int ret = core_interrupt_disable();
	lua_pushinteger(L,ret);
    return 1;
}

LUA_FUN(lua_core_restore_interrupt)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_core_restore_interrupt]\r\n");
		return 0;
	}
	int ret = lua_tointeger(L, 1);
	core_restore_interrupt(ret);
    return 0;
}

LUA_FUN(lua_print_riscv_reg)
{
	(void)L;
	printf(" PC: 0x%lx\n", (long)core_get_current_pc());
	printf(" SP: 0x%lx\n", __nds__get_current_sp());
	printf(" NDS_MISA: 0x%lx\r\n", read_csr(NDS_MISA));				  // 指令集架构信息
	printf(" NDS_MSCRATCH: 0x%lx\r\n", read_csr(NDS_MSCRATCH));       // 用于保存M模式指向hart上下文的指针，并在进入M模式的处理程序时和用户寄存器交换
	printf(" NDS_MARCHID: 0x%lx\r\n", read_csr(NDS_MARCHID));		  // 体系结构id
	printf(" NDS_MHARTID: 0x%lx\r\n", read_csr(NDS_MHARTID));		  // 硬件线程id
	printf(" NDS_MSP_BASE: 0x%lx\r\n", read_csr(NDS_MSP_BASE));       // MSP基地址
	printf(" NDS_MSP_BOUND: 0x%lx\r\n", read_csr(NDS_MSP_BOUND)); 	  // MSP边界
	printf(" NDS_MSTATUS: 0x%lx\r\n", read_csr(NDS_MSTATUS));	  	  // 状态寄存器
	printf(" NDS_MCAUSE: 0x%lx\r\n", read_csr(NDS_MCAUSE));           // 异常/中断原因
	printf(" NDS_MEPC: 0x%lx\r\n", read_csr(NDS_MEPC)); 	 	      // 异常程序指针
	printf(" NDS_MTVAL: 0x%lx\r\n", read_csr(NDS_MTVAL));			  // 异常和中断的补充信息（如异常指令，非法地址）
	printf(" NDS_MTVEC: 0x%lx\r\n", read_csr(NDS_MTVEC)); 			  // 异常入口基地址寄存器
	printf(" NDS_MIE: 0x%lx\r\n", read_csr(NDS_MIE)); 	  			  // 中断使能
	printf(" NDS_MIP: 0x%lx\r\n", read_csr(NDS_MIP)); 				  // 当前中断中断Pending位
	return 0;
}

LUA_FUN(lua_set_timer0_cycle)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_set_timer0_cycle]\r\n");
		return 0;
	}
	int ms_value = lua_tointeger(L, 1);
	plic_interrupt_enable(IRQ4_TIMER0);
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,ms_value*sys_clk.pclk*1000);
	timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
	timer_start(TIMER0);
	return 0;
}

LUA_FUN(lua_set_timer1_cycle)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_set_timer1_cycle]\r\n");
		return 0;
	}
	int ms_value = lua_tointeger(L, 1);
	plic_interrupt_enable(IRQ3_TIMER1);
	timer_set_init_tick(TIMER1,0);
	timer_set_cap_tick(TIMER1,ms_value*sys_clk.pclk*1000);
	timer_set_mode(TIMER1, TIMER_MODE_SYSCLK);
	timer_start(TIMER1);
	return 0;
}

LUA_FUN(lua_get_mem_perused)
{
	(void)L;
	unsigned char temp = vPortPerused();
	lua_pushinteger(L,temp);
    return 1;
}

LUA_FUN(lua_gpio_function_en)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_function_en]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_function_en(gpio_num);
    return 0;
}

LUA_FUN(lua_gpio_function_dis)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_function_dis]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_function_dis(gpio_num);
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
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_output_en(gpio_num);
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
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_output_dis(gpio_num);
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
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_input_en(gpio_num);
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
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_input_dis(gpio_num);
    return 0;
}

LUA_FUN(lua_gpio_get_level)
{
	int n = lua_gettop(L);
	if(n  OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_get_level]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num){
		int level = gpio_get_level(gpio_num);
		lua_pushinteger(L,level);
		return 1;
	}
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
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_toggle(gpio_num);
    return 0;
}

LUA_FUN(lua_gpio_set_high)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_high]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_set_high_level(gpio_num);
    return 0;
}

LUA_FUN(lua_gpio_set_low)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_low]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num)gpio_set_low_level(gpio_num);
    return 0;
}

LUA_FUN(lua_gpio_set_up_down_res)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_up_down_res]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num){
		int up_down_res = lua_tointeger(L, 2);
		gpio_set_up_down_res(gpio_num, up_down_res);
	}
    return 0;
}

LUA_FUN(lua_gpio_set_irq)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_irq]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num){
		gpio_function_en(gpio_num);
		gpio_input_en(gpio_num);
		unsigned char trigger_type = lua_tointeger(L, 2);
		if(INTR_RISING_EDGE == trigger_type || INTR_HIGH_LEVEL == trigger_type){
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLDOWN_100K);
		}
		else{
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLUP_10K);
		}
		gpio_set_irq(gpio_num, trigger_type);
		plic_interrupt_enable(IRQ25_GPIO);
	}
    return 0;
}

LUA_FUN(lua_gpio_set_gpio2risc0_irq)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_gpio2risc0_irq]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num){
		gpio_function_en(gpio_num);
		gpio_input_en(gpio_num);
		unsigned char trigger_type = lua_tointeger(L, 2);
		if(INTR_RISING_EDGE == trigger_type || INTR_HIGH_LEVEL == trigger_type){
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLDOWN_100K);
		}
		else{
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLUP_10K);
		}
		gpio_set_gpio2risc0_irq(gpio_num, trigger_type);
		plic_interrupt_enable(IRQ26_GPIO2RISC0);
	}
    return 0;
}

LUA_FUN(lua_gpio_set_gpio2risc1_irq)
{
	int n = lua_gettop(L);
	if(n OPERATION 2)
	{
		printf("[PARAMETER ERROR: lua_gpio_set_gpio2risc1_irq]\r\n");
		return 0;
	}
	const char* gpio = lua_tostring(L, 1);
	int gpio_num = string_to_gpio_num(gpio);
	if(gpio_num){
		gpio_function_en(gpio_num);
		gpio_input_en(gpio_num);
		unsigned char trigger_type = lua_tointeger(L, 2);
		if(INTR_RISING_EDGE == trigger_type || INTR_HIGH_LEVEL == trigger_type){
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLDOWN_100K);
		}
		else{
			gpio_set_up_down_res(gpio_num, GPIO_PIN_PULLUP_10K);
		}
		gpio_set_gpio2risc1_irq(gpio_num, trigger_type);
		plic_interrupt_enable(IRQ27_GPIO2RISC1);
	}
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
   int time = lua_tointeger(L, 1);
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
   int time = lua_tointeger(L, 1);
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
   unsigned char addr = lua_tointeger(L, 1);
   unsigned char data = lua_tointeger(L, 2);
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
   unsigned char addr = lua_tointeger(L, 1);
   unsigned short data = lua_tointeger(L, 2);
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
   unsigned char addr = lua_tointeger(L, 1);
   unsigned int data = lua_tointeger(L, 2);
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
	unsigned int addr = lua_tointeger(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tointeger(L, 3);
	if(len > 1024){
		printf("[PARAMETER ERROR: lua_analog_write_reg8_buff,table length greater than 1024]\r\n");
		return 0;
	}
	if( 0 == table_len){
		printf("[PARAMETER ERROR: lua_analog_write_reg8_buff,table length equal zero]\r\n");
		return 0;
	}

	if(len > table_len){
		len = table_len;
	}

	unsigned char buf[1024];
	for (unsigned int i = 1; i <= len; i++) {
	   lua_pushinteger(L, i);
	   lua_gettable(L, 2);
	   if (lua_isnumber(L, -1)) {
		   buf[i - 1] =  lua_tointeger(L, -1);
	   } else {
		   printf("lua_analog_write_reg8_buff ERROR, Element %d: %s\n", i, lua_tostring(L, -1));
	   }
	   lua_pop(L, 1);
	}
	analog_write_buff(addr,buf,len);

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
	unsigned int addr = lua_tointeger(L, 1);
	unsigned int len  = lua_tointeger(L, 2);
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
   unsigned char addr = lua_tointeger(L, 1);
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
   unsigned char addr = lua_tointeger(L, 1);
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
   unsigned char addr = lua_tointeger(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
   unsigned char data = lua_tointeger(L, 2);
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
   unsigned int addr  = lua_tointeger(L, 1);
   unsigned short data = lua_tointeger(L, 2);
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
   unsigned int addr = lua_tointeger(L, 1);
   unsigned int data  = lua_tointeger(L, 2);
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
	unsigned int addr = lua_tointeger(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "lua_write_sram8_buff Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tointeger(L, 3);

	if(len > 1024){
		printf("[PARAMETER ERROR: lua_write_sram8_buff,table length greater than 1024]\r\n");
		return 0;
	}
	if( 0 == table_len){
		printf("[PARAMETER ERROR: lua_write_sram8_buff,table length equal zero]\r\n");
		return 0;
	}
	for (unsigned int i = 1; i <= len; i++) {
	   lua_pushinteger(L, i);
	   lua_gettable(L, 2);
	   if (lua_isnumber(L, -1)) {
		   write_sram8(addr + i - 1, lua_tointeger(L, -1));
	   } else {
		   printf("lua_write_sram8_buff ERROR,Element %d: %s\n", i, lua_tostring(L, -1));
	   }
	   lua_pop(L, 1);
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
	unsigned int addr = lua_tointeger(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "lua_write_sram16_buff Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tointeger(L, 3);
	if(len > 1024){
		printf("[PARAMETER ERROR: lua_write_sram16_buff,table length greater than 1024]\r\n");
		return 0;
	}
	if( 0 == table_len){
		printf("[PARAMETER ERROR: lua_write_sram16_buff,table length equal zero]\r\n");
		return 0;
	}
	for (unsigned int i = 1; i <= len; i++) {

	   lua_pushinteger(L, i);
	   lua_gettable(L, 2);
	   if (lua_isnumber(L, -1)) {
		   write_sram16(addr + (i - 1) *2, lua_tointeger(L, -1));
	   } else {
		   printf("lua_write_sram16_buff ERROR,Element %d: %s\n", i, lua_tostring(L, -1));
	   }
	   lua_pop(L, 1);
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
	unsigned int addr = lua_tointeger(L, 1);

	if (lua_type(L, 2) != LUA_TTABLE) {
	   lua_pushstring(L, "Expected a table");
	   lua_error(L);
	}
	unsigned int table_len = luaL_len(L, 2);
	unsigned int len  = lua_tointeger(L, 3);
	if(len > 1024){
		printf("[PARAMETER ERROR: lua_write_sram32_buff,table length greater than 1024]\r\n");
		return 0;
	}
	if( 0 == table_len){
		printf("[PARAMETER ERROR: lua_write_sram32_buff,table length equal zero]\r\n");
		return 0;
	}
	for (unsigned int i = 1; i <= len; i++) {
	   lua_pushinteger(L, i);
	   lua_gettable(L, 2);
	   if (lua_isnumber(L, -1)) {
		   write_sram32(addr + (i - 1) *4, lua_tointeger(L, -1));
	   } else {
		   printf("lua_write_sram32_buff ERROR,Element %d: %s\n", i, lua_tostring(L, -1));
	   }
	   lua_pop(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
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
	unsigned int addr = lua_tointeger(L, 1);
	unsigned int len  = lua_tointeger(L, 2);
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
	unsigned int addr = lua_tointeger(L, 1);
	unsigned int len  = lua_tointeger(L, 2);
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
	unsigned int addr = lua_tointeger(L, 1);
	unsigned int len  = lua_tointeger(L, 2);
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
   unsigned int addr = lua_tointeger(L, 1);
   unsigned char data = lua_tointeger(L, 2);
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
   unsigned int addr  = lua_tointeger(L, 1);
   unsigned short data = lua_tointeger(L, 2);
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
   unsigned int addr = lua_tointeger(L, 1);
   unsigned int data  = lua_tointeger(L, 2);
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
   unsigned int addr = lua_tointeger(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
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
   unsigned int addr = lua_tointeger(L, 1);
   unsigned int data = read_reg32(addr);
   lua_pushinteger(L,data);
   return 1;
}

/*********************************************************************/
#define  Rocket_EVB	 1

#ifdef	 Rocket_EVB
#include "../app_config.h"
#include "my_lua_interface.h"
#include "../device/oled.h"
#include "../device/lis2dh.h"
#include "../device/photoresistor.h"
#include "../device/sht30.h"
#include "../device/button.h"
#include "../device/ws2812b.h"
LUA_FUN(lua_rocket_init)
{
	(void)L;
	gpio_function_en(LED);
	gpio_output_en(LED);
	gpio_input_dis(LED);
	i2c_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);
	i2c_master_init();
	i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C_CLK_SPEED)));
	OLED_Init();
	OLED_CLS();
	Lis2dh_Init();
	ws2812b_init();
	photoresistor_init();
	init_button();
	return 0;
}

LUA_FUN(lua_rocket_oled_cls)
{
	(void)L;
	OLED_CLS();
	return 0;
}

LUA_FUN(lua_rocket_oled_cls_y)
{
    int n = lua_gettop(L);
    if(n OPERATION 1)
    {
		 printf("[PARAMETER ERROR: lua_oled_cls_y]\r\n");
		 return 0;
    }
    unsigned int y = lua_tointeger(L, 1);
	OLED_FillY(y);
	return 0;
}

LUA_FUN(lua_rocket_oled_show_str)
{
   int n = lua_gettop(L);
   if(n OPERATION 3)
   {
		printf("[PARAMETER ERROR: lua_oled_show_str]\r\n");
		return 0;
   }
   uint8_t x  = lua_tointeger(L, 1);
   uint8_t y  = lua_tointeger(L, 2);
   OLED_ShowStr(x,y,(uint8_t *)lua_tostring(L,3));
   return 0;
}

LUA_FUN(lua_rocket_sht30_read)
{
	float Temperature,Humidity;
	if(0 == ReadSHT3x(&Temperature,&Humidity)){
		lua_pushnumber(L,Temperature);
		lua_pushnumber(L,Humidity);
	}
	else{
		lua_pushnumber(L,0);
		lua_pushnumber(L,0);
	}
	return 2;
}

LUA_FUN(lua_rocket_adc_read)
{
	unsigned short adc_temp_val = adc_get_voltage_dma();
	unsigned int Vol;
	unsigned int R13;
	Vol = (adc_temp_val*3300)/4096;
	R13 = 47000*Vol/(3300-Vol);
	lua_pushinteger(L,Vol);
	lua_pushinteger(L,R13);
	return 2;
}

LUA_FUN(lua_rocket_lis2dh_read)
{
	axis_info_t sample;
	uint8_t reg = 0x28|0X80;
	uint8_t data[6];
	i2c_master_write(LIS2DH_ADDR, &reg, 1);
	i2c_master_read(LIS2DH_ADDR, data, 6);
	sample.x = (*(int16_t*)(data+0));
	sample.y = (*(int16_t*)(data+2));
	sample.z = (*(int16_t*)(data+4));
	lua_pushinteger(L,sample.x);
	lua_pushinteger(L,sample.y);
	lua_pushinteger(L,sample.z);
	return 3;
}

LUA_FUN(lua_rocket_button_read)
{
	if(0==gpio_get_level(BUTTON_LFT)){
		delay_ms(10);
		if(0== gpio_get_level(BUTTON_LFT)){
			lua_pushinteger(L,1);
		}
		while(0 == gpio_get_level(BUTTON_LFT));
	}
	else if(0 == gpio_get_level(BUTTON_MID)){
		delay_ms(10);
		if(0 == gpio_get_level(BUTTON_MID)){
			lua_pushinteger(L,2);
		}
		while(0==gpio_get_level(BUTTON_MID));
	}
	else if(0 == gpio_get_level(BUTTON_RHT)){
		delay_ms(10);
		if(0 == gpio_get_level(BUTTON_RHT)){
			lua_pushinteger(L,3);
		}
		while(0 == gpio_get_level(BUTTON_RHT));
	}
	else{
		lua_pushinteger(L,0);
	}
	return 1;
}

LUA_FUN(lua_rocket_ws2812b_set)
{
	int n = lua_gettop(L);
	if(n OPERATION 1)
	{
		printf("[PARAMETER ERROR: lua_rocket_ws2812b_set]\r\n");
		return 0;
	}
	unsigned int color = lua_tointeger(L, 1);
	ws2812b_set(color);
	return 0;
}
#endif
/*********************************************************************/
struct luaL_Reg lua_lib[]=
{
	/*********************************************************************/
#ifdef	 Rocket_EVB
	{"rocket_init",lua_rocket_init},
	{"rocket_oled_cls",lua_rocket_oled_cls},
	{"rocket_oled_cls_y",lua_rocket_oled_cls_y},
	{"rocket_oled_show_str",lua_rocket_oled_show_str},
	{"rocket_sht30_read",lua_rocket_sht30_read},
	{"rocket_adc_read",lua_rocket_adc_read},
	{"rocket_lis2dh_read",lua_rocket_lis2dh_read},
	{"rocket_button_read",lua_rocket_button_read},
	{"rocket_ws2812b_set",lua_rocket_ws2812b_set},
#endif
	/*********************************************************************/
	{"SYS_DEBUG_EN",lua_SYS_DEBUG_EN},
	{"print_riscv_reg",lua_print_riscv_reg},
	{"core_interrupt_enable",lua_core_interrupt_enable},
	{"core_interrupt_disable",lua_core_interrupt_disable},
	{"core_restore_interrupt",lua_core_restore_interrupt},
	{"delay_us",lua_delay_us},
	{"delay_ms",lua_delay_ms},
	{"get_mem_perused",lua_get_mem_perused},
	{"set_timer0_cycle",lua_set_timer0_cycle},
	{"set_timer1_cycle",lua_set_timer1_cycle},
	/*********************************************************************/
	{"gpio_function_en",lua_gpio_function_en},
	{"gpio_function_dis",lua_gpio_function_dis},
	{"gpio_output_en",lua_gpio_output_en},
	{"gpio_output_dis",lua_gpio_output_dis},
	{"gpio_input_en",lua_gpio_input_en},
	{"gpio_input_dis",lua_gpio_input_dis},
	{"gpio_get_level",lua_gpio_get_level},
	{"gpio_high",lua_gpio_set_high},
	{"gpio_low",lua_gpio_set_low},
	{"gpio_toggle",lua_gpio_toggle},
	{"gpio_set_up_down_res",lua_gpio_set_up_down_res},
	{"gpio_set_irq",lua_gpio_set_irq},
	{"gpio_set_gpio2risc0_irq",lua_gpio_set_gpio2risc0_irq},
	{"gpio_set_gpio2risc1_irq",lua_gpio_set_gpio2risc1_irq},
	/*********************************************************************/
	{"analog_write_reg8",lua_analog_write_reg8},
	{"analog_write_reg16",lua_analog_write_reg16},
	{"analog_write_reg32",lua_analog_write_reg32},
	{"analog_write_reg8_buff",lua_analog_write_reg8_buff},
	{"analog_read_reg8",lua_analog_read_reg8},
	{"analog_read_reg16",lua_analog_read_reg16},
	{"analog_read_reg32",lua_analog_read_reg32},
	{"analog_read_reg8_buff",lua_analog_read_reg8_buff},
	/*********************************************************************/
	{"write_sram8",lua_write_sram8},
	{"write_sram16",lua_write_sram16},
	{"write_sram32",lua_write_sram32},
	{"write_sram8_buff",lua_write_sram8_buff},
	{"write_sram16_buff",lua_write_sram16_buff},
	{"write_sram32_buff",lua_write_sram32_buff},
	/*********************************************************************/
	{"read_sram8",lua_read_sram8},
	{"read_sram16",lua_read_sram16},
	{"read_sram32",lua_read_sram32},
	{"read_sram8_buff",lua_read_sram8_buff},
	{"read_sram16_buff",lua_read_sram16_buff},
	{"read_sram32_buff",lua_read_sram32_buff},
	/*********************************************************************/
	{"write_reg8",lua_write_reg8},
	{"write_reg16",lua_write_reg16},
	{"write_reg32",lua_write_reg32},
	{"read_reg8",lua_read_reg8},
	{"read_reg16",lua_read_reg16},
	{"read_reg32",lua_read_reg32},
	/*********************************************************************/
    {NULL,NULL}
};

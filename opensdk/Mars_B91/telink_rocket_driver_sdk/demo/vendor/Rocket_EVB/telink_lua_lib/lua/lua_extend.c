#include "lua_extend.h"

lua_State *g_L;
lua_State *g_L_IRQ = NULL;
//
int c_call_lua(const char * fun){
	lua_getglobal(g_L, fun);
	if (lua_pcall(g_L, 0, 0, 0) != LUA_OK) {
		printf("Error: %s\n", lua_tostring(g_L, -1));
		return -1;
	}
	return 0;
}

int c_call_lua_one_par(const char * fun,int num){
	if(g_L_IRQ == NULL)return 0;
	lua_getglobal(g_L_IRQ, fun);
	lua_pushinteger(g_L_IRQ,num);
	if (lua_pcall(g_L_IRQ, 1, 0, 0) != LUA_OK) {
		printf("Error: %s\n", lua_tostring(g_L_IRQ, -1));
		return -1;
	}
	return 0;
}

//
void luaL_mylibs(lua_State *L,const struct luaL_Reg lua_lib[])
{
	unsigned int index = 0;
	do{
		lua_register(L,(lua_lib[index].name),(lua_lib[index].func));
	}while(lua_lib[index++].name);
}
//
__attribute__((unused)) void Lua_load_string(const char * str,const struct luaL_Reg lua_lib[])
{
	g_L = luaL_newstate();
	if(g_L){
		luaL_openlibs(g_L);
		luaL_mylibs(g_L,lua_lib);
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
__attribute__((unused)) void Lua_load_string2(const char * str)
{
	int ret = luaL_dostring(g_L, str);
	if(ret){
		if (ret == LUA_ERRSYNTAX)
			printf("Error: %s\r\n", lua_tostring(g_L, -1));
		else if (ret == LUA_ERRMEM)
			printf("Error: memory error\r\n");
		else
			printf("Error: syntax error:%s\r\n",lua_tostring(g_L, -1));
	}
}

//
int g_break_points_num = 0;

typedef struct{
	int  break_point;
	char source[128];
}lua_break_point_info;

lua_break_point_info g_lua_break_point_info[LUA_MAX_BREAK_POINTS]={0};
//
static int get_str_ele_first_index(const char *data,char element)
{
	int index = 0;
	int length = strlen(data);
	for(index=0; index < length -1; index++){
		if(element == data[index]){
			return index;
		}
	}
	return -1;
}
//
int telink_lua_lib_load_break_points(void)
{
    char buffer[200];
	char FileName[128];
	int ele_index = 0;
    sprintf(FileName,"%01d:/%s",get_script_volNum(),LUA_BREAK_POINT_CONFIG_FILE_NAME);

    if(f_open(&g_file,FileName,FA_OPEN_EXISTING|FA_READ)){
    	printf("can't open the break_points configuration file:%s\r\n",FileName);
    	return -1;
    }

    f_lseek(&g_file,0);

	#include <stdlib.h>

    if (f_gets(buffer, 200, &g_file)) {	// 读取第一行 获取断点的个数
    	ele_index = get_str_ele_first_index(buffer,LUA_BREAK_POINT_SPLIT_ELE);
    	g_break_points_num = atoi(&buffer[ele_index+1]);
    }

    if(g_break_points_num > LUA_MAX_BREAK_POINTS) g_break_points_num = LUA_MAX_BREAK_POINTS;

    for(int i= 0; i < g_break_points_num; i++){
		if (f_gets(buffer, 200, &g_file)) {	// 成功读取一行
			ele_index = get_str_ele_first_index(buffer,LUA_BREAK_POINT_SPLIT_ELE);
			if(-1 != ele_index){ // test,1
				g_lua_break_point_info[i].break_point = atoi(&buffer[ele_index+1]);
				strncpy(g_lua_break_point_info[i].source,buffer,ele_index);
				g_lua_break_point_info[i].source[ele_index]   = '.';
				g_lua_break_point_info[i].source[ele_index+1] = 'l';
				g_lua_break_point_info[i].source[ele_index+2] = 'u';
				g_lua_break_point_info[i].source[ele_index+3] = 'a';
				g_lua_break_point_info[i].source[ele_index+4] = '\0';
			}
			printf("short_src:%s	break_point:%d\r\n",g_lua_break_point_info[i].source,g_lua_break_point_info[i].break_point);
		}
		else{
			 g_break_points_num = i;
			 printf("can't read break_points in line :%d form the configuration file:%s\r\n",i+1,FileName);
			 f_close(&g_file);
			 return -2;
		}
    }
    printf("break points total:%d\r\n",g_break_points_num);
    f_close(&g_file);
    return 0;
}
//
typedef void (*lua_debug_break_read_data_cb)(unsigned int address,unsigned int length);
//
void lua_debug_break_flash_read_data_cb(unsigned int address,unsigned int length)
{
	flash_read_page(address, length, &Ymodem_rec_buf[30]);
}
//
void lua_debug_break_core_read_data_cb(unsigned int address,unsigned int length)
{
	if(address>=0x1000000&&address<=0x2000000){
		for(unsigned int z = 0; z < length; z++)
			Ymodem_rec_buf[30+z] = read_reg8(address+z);
	}
	else{
	//if((address <= 0x20000)||(address <= 0x80000 && address <= 0xA0000))
		for(unsigned int z = 0; z < length; z++)
			Ymodem_rec_buf[30+z] = read_sram8(address+z);
	}
}
//
void lua_debug_break_analog_read_data_cb(unsigned int address,unsigned int length)
{
	for(unsigned int z = 0; z < length; z++){
		if((address+z) > 0xff) printf("address should less or equal 0xff\r\n");
		else Ymodem_rec_buf[30+z] = analog_read_reg8(address+z);
	}
}
//
void lua_debug_break_read_data(lua_debug_break_read_data_cb cb)
{
	unsigned int address = 0;
	unsigned int length = 0;
	char buf[80];
	address = Ymodem_rec_buf[2]<<24|Ymodem_rec_buf[3]<<16|Ymodem_rec_buf[4]<<8|Ymodem_rec_buf[5];
	length = Ymodem_rec_buf[6]<<24|Ymodem_rec_buf[7]<<16|Ymodem_rec_buf[8]<<8|Ymodem_rec_buf[9];

	cb(address, length);

	Ymodem_rec_buf[length+30] = 0;
	unsigned int q = length/16;
	unsigned int y = length%16;
	buf[59] = 0;
	for(unsigned int j = 0; j < q; j++){
		sprintf(&buf[0],"%08x:  ",address+j*16);
		for(unsigned int w = 0; w < 16; w++){
			sprintf(&buf[11+w*3],"%02x ",Ymodem_rec_buf[30+j*16+w]);
		}
		printf("%s\r\n",buf);
	}
	if(y){
		sprintf(&buf[0],"%08x:  ",address+q*16);
		for(unsigned int w = 0; w < y; w++){
			sprintf(&buf[11+w*3],"%02x ",Ymodem_rec_buf[30+q*16+w]);
		}
		buf[11+y*3] = 0;
		printf("%s\r\n",buf);
	}
}

//
__attribute__((section(".ram_code"))) int lua_debug_break_cb(int break_point_index)
{
	// 因为是等待输入阶段 所以以下代码不需要高效率
	unsigned char poll_flag = 1;
	unsigned int read_size = 0;
	unsigned int address = 0;
	unsigned int length = 0;
	//lua_interface.lua_irq_uart_disable();
	while(poll_flag){
		do{
			read_size = Ymodem_read_data(Ymodem_rec_buf,YMODEM_REC_LEN);
		}while(0==read_size); // 等待一笔数据

		Ymodem_rec_buf[read_size] = 0;
		if(0 == strncmp((char*)Ymodem_rec_buf,"go",2)){		 	// 继续运行
			poll_flag = 0;
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"run",3)){	// 全速运行
			poll_flag = 0;
			g_break_points_num = 0;
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"info",4)){	// 打印所有断点号
			printf("break points total:%d\r\n",g_break_points_num);
			for(int i = 0; i < g_break_points_num; i++){
				printf("short_src:%s	break_point:%d\r\n",g_lua_break_point_info[i].source,g_lua_break_point_info[i].break_point);
			}
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"cancel",6)){// 临时取消此处断点
			poll_flag = 0;
			g_lua_break_point_info[break_point_index].break_point = 0;
			for(int i = break_point_index; i < g_break_points_num - 1; i++){// [break_point_index,g_break_points_num)
				g_lua_break_point_info[i].break_point = g_lua_break_point_info[i+1].break_point;
			}
			g_break_points_num -= 1;
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"do",2)){ 	// 执行脚本
			// do xxxxxxxx
			// eg: do print(1111)
			// do local cc = 123.456 local show = string.format("cc:%6.3f",cc)  lua_oled_cls_y(4) oled_show_str(0,4,show)
			Lua_load_string2((const char*)&Ymodem_rec_buf[3]);
		}

		else if(0 == strncmp((char*)Ymodem_rec_buf,"rr",2)){ // 打印通用寄存器 CSR寄存器 PC寄存器
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
		}
		//
		else if(0 == strncmp((char*)Ymodem_rec_buf,"rf",2)){ // 读取指定地址的FLASH数据
			lua_debug_break_read_data(lua_debug_break_flash_read_data_cb);
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"rc",2)){ // 读取指定地址的CORE数据
			lua_debug_break_read_data(lua_debug_break_core_read_data_cb);
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"ra",2)){ // 读取指定地址的ANALOG数据
			lua_debug_break_read_data(lua_debug_break_analog_read_data_cb);
		}

		else if(0 == strncmp((char*)Ymodem_rec_buf,"wf",2)){ // 写入指定地址的FLASH数据
			address = Ymodem_rec_buf[2]<<24|Ymodem_rec_buf[3]<<16|Ymodem_rec_buf[4]<<8|Ymodem_rec_buf[5];
			length = Ymodem_rec_buf[6]<<24|Ymodem_rec_buf[7]<<16|Ymodem_rec_buf[8]<<8|Ymodem_rec_buf[9];
			flash_write_page(address, length, &Ymodem_rec_buf[10]);
			printf("wf\r\n"); //70000
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"wc",2)){ // 写入指定地址的CORE数据
			address = Ymodem_rec_buf[2]<<24|Ymodem_rec_buf[3]<<16|Ymodem_rec_buf[4]<<8|Ymodem_rec_buf[5];
			length = Ymodem_rec_buf[6]<<24|Ymodem_rec_buf[7]<<16|Ymodem_rec_buf[8]<<8|Ymodem_rec_buf[9];
			if(address>=0x1000000&&address<=0x2000000){
				for(unsigned int z = 0; z < length; z++)
					write_reg8(address+z,Ymodem_rec_buf[10+z]);
			}
			else{
				for(unsigned int z = 0; z < length; z++)
					write_sram8(address+z,Ymodem_rec_buf[10+z]);
			}
			printf("wc\r\n");
		}
		else if(0 == strncmp((char*)Ymodem_rec_buf,"wa",2)){ // 写入指定地址的ANALOG数据
			address = Ymodem_rec_buf[2]<<24|Ymodem_rec_buf[3]<<16|Ymodem_rec_buf[4]<<8|Ymodem_rec_buf[5];
			length = Ymodem_rec_buf[6]<<24|Ymodem_rec_buf[7]<<16|Ymodem_rec_buf[8]<<8|Ymodem_rec_buf[9];
			if(address>0xff){
				printf("address should less or equal 0xff\r\n");
			}
			else{
				for(unsigned int z = 0; z < length; z++)
					analog_write_reg8(address+z,Ymodem_rec_buf[10+z]);
				printf("wa\r\n");
			}
		}
	}
	//lua_interface.lua_irq_uart_enable();
	return 0;
}
//
int g_sys_debug_en = 0;
void debug_break(lua_State *L, lua_Debug *ar)
{
	if(g_sys_debug_en){
		for(int i = 0; i < g_break_points_num; i++){
			if(g_lua_break_point_info[i].break_point == ar->currentline){
				lua_getinfo(L,"S",ar);
				if(0 == strncmp(g_lua_break_point_info[i].source,&(ar->short_src[3]),strlen(g_lua_break_point_info[i].source))){
					printf("file:%s line:%d\r\n",ar->short_src,ar->currentline);
					lua_debug_break_cb(i);
				}
			}
		}
	}
}
//
int Lua_load_file(const char *path,const struct luaL_Reg lua_lib[])
{
	g_L = luaL_newstate();
    if(g_L){
		luaL_openlibs(g_L);
		luaL_mylibs(g_L,lua_lib);
		if(g_break_points_num)	lua_sethook(g_L,debug_break,LUA_MASKLINE,1);
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
int Lua_load_irq_file(const char *path,const struct luaL_Reg lua_lib[])
{
	(void)lua_lib;
	g_L_IRQ = luaL_newstate();
    if(g_L_IRQ){
    	luaL_openlibs(g_L_IRQ);
		luaL_mylibs(g_L_IRQ,lua_lib);
		int ret = luaL_dofile(g_L_IRQ, path);
		if(ret){
			if (ret == LUA_ERRSYNTAX)
				printf("Error: %s\r\n", lua_tostring(g_L_IRQ, -1));
			else if (ret == LUA_ERRMEM)
				printf("Error: memory error\r\n");
			else
				printf("Error: syntax error:%s\r\n",lua_tostring(g_L_IRQ, -1));
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
	telink_lua_lib_load_break_points();
	char path[12];
	char irq_path[12];
	sprintf(path,"%s","0:/main.lua");
	path[0] = get_script_volNum() + '0';

	sprintf(irq_path,"%s","0:/irq.lua");
	irq_path[0] = get_script_volNum() + '0';

    if(f_open(&g_file,irq_path,FA_READ)){
    	printf("IRQ file:%s is not exit!\r\n",irq_path);
    }
    else{
    	Lua_load_irq_file((const char *)irq_path,lua_lib);
    	f_close(&g_file);
    }

	return Lua_load_file((const char *)path,lua_lib) | c_call_lua("setup");
}
//
void telink_lua_main_loop(void)
{
	 c_call_lua("loop");
}


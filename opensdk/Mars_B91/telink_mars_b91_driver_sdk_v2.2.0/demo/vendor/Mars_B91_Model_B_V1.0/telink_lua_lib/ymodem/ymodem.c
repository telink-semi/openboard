#include "ymodem.h"

unsigned char Ymodem_rec_buf[YMODEM_REC_LEN];

_m_lua_interface* ymodem_lua_interface;

YmodemInfo g_YmodemInfo;
//
unsigned short UpdateCRC16(unsigned short crc_in, unsigned char byte)
{
    unsigned int crc = crc_in;
    unsigned int in = byte | 0x100;
    do{
		crc <<= 1;
		in <<= 1;
		if(in & 0x100)
		  ++crc;
		if(crc & 0x10000)
		  crc ^= 0x1021;
    }
    while(!(in & 0x10000));
    return crc & 0xffffu;
}
//
unsigned short Cal_CRC16(const unsigned char* p_data, unsigned int size)
{
    unsigned short crc = 0;
    const unsigned char* dataEnd = p_data+size;
    while(p_data < dataEnd)
    	crc = UpdateCRC16(crc, *p_data++);
    crc = UpdateCRC16(crc, 0);
  	crc = UpdateCRC16(crc, 0);
  	return crc&0xffffu;
}
//  0x21000 -> 32 31 30 30 30 00 ("21000")
unsigned char hex_to_ascii(int hex_value,char *ascii_str) {
	return sprintf(ascii_str, "%X", hex_value);
}
/*-------------------------------------------------------------------------
    Function    :  tlkapi_ascii_char2val
    Return: -1=error
    Description:
        'a' -> 0xa  'A' -> 0xa
-------------------------------------------------------------------------*/
char ascii_char2val(char c)
{
    if(c>='0' && c<='9')
        return c-'0';
    if((c>='a' && c<='f') || (c>='A' && c<='F'))
        return (c&0x7)+9;

    return (char)(-1);
}
/*-------------------------------------------------------------------------
    Function    :  tlkapi_ascii_str2val
    Return: -1=error
    Description:
        str = "123" bas = 10 return 123
        str = "123" bas = 16 return 0x123
-------------------------------------------------------------------------*/
int ascii_str2val(const char str[], char base)
{
    int val = 0;
    signed char v;
    while(*str != 0){
        v = ascii_char2val(*str);
        if (v == -1 || v >= base)
        {
            if(val == 0) //to filter abormal beginning and ending
            {
                str ++;
                continue;
            }
            else
            {
                break;
            }
        }
        val = val*base + v;
        str++;
    }
    return val;
}
//
void Ymodem_send_array(unsigned char *array,uint32_t length)
{
	for(uint32_t i = 0; i < length; i++){
		ymodem_lua_interface -> Ymodem_send_byte(array[i]);
	}
}
//
void Ymodem_send_str(char *array)
{
	for(uint32_t i = 0; i < strlen(array); i++){
		ymodem_lua_interface -> Ymodem_send_byte(array[i]);
	}
}
//
void Ymodem_pre(_m_lua_interface* interface)
{
	ymodem_lua_interface = interface;
	ymodem_lua_interface->lua_uart_init();
	ymodem_lua_interface->mem_init();
	ymodem_lua_interface->init_fs();
	Ymodem_init();
}
//
void Ymodem_init(void)
{
	ymodem_lua_interface->lua_uart_irq_disable();
    g_YmodemInfo.frame_count  = 1;
    g_YmodemInfo.rec_len      = 0;
    g_YmodemInfo.frame_num 	  = 0;
    g_YmodemInfo.frame_remain = 0;
    g_YmodemInfo.frame_total  = 0;
    g_YmodemInfo.file_size 	  = 0;
    g_YmodemInfo.maxTryTimes  = 10;
    g_YmodemInfo.state = Ymode_Idle;
}

unsigned char g_script_volNum = 0;

unsigned char get_script_volNum(void)
{
	// 下载使用的盘号 >  传递来的设定值
	unsigned char ret;
	if( g_script_volNum > 9) ret = g_script_volNum - 10;
	else ret = ymodem_lua_interface->set_script_volNum();
	if(ret > 9) ret = 0;
	return ret;
}

//
unsigned char Ymodem_ReciveDataCallback(unsigned char* data,uint32_t start_index,uint32_t end_index)
{
	FRESULT res;
	res = f_lseek(&g_file,YMODEM_PACKET_BLOCK_SIZE * (g_YmodemInfo.frame_count - 2));
	if(res) return res;
	return f_write(&g_file,&data[start_index], end_index - start_index + 1, &g_bw);
}
//
unsigned char Ymodem_receive_one_byte(void)
{
	unsigned char data;
	if(1 == ymodem_lua_interface->Ymodem_read_data_wait(&data,1,YMODEM_RECV_TIMEOVER_MS*1000)){
		if(data == YMODEM_ACK || data == YMODEM_NAK || data == YMODEM_EOT)
			return data;
	}
	return YMODEM_EOT;
}
//
unsigned char Ymodem_receive_byte(unsigned char byte)
{
	unsigned char data;
	if(1 == ymodem_lua_interface->Ymodem_read_data_wait(&data,1,YMODEM_RECV_TIMEOVER_MS*1000)){
		if(data == byte)    return 0;
	}
	return 1;
}
//
unsigned char Ymodem_receive_c_frame(void)
{
	return Ymodem_receive_byte(YMODEM_C);
}
//
unsigned char Ymodem_receive_ack_frame(void)
{
	return Ymodem_receive_byte(YMODEM_ACK);
}
//
unsigned char Ymodem_receive_nak_frame(void)
{
	return Ymodem_receive_byte(YMODEM_NAK);
}
//
unsigned char Ymodem_receive_eot_frame(void)
{
	return Ymodem_receive_byte(YMODEM_EOT);
}
//
void Ymodem_send_one_byte(unsigned char byte)
{
	delay_ms(YMODEM_SEND_TIME_OUT_MS);
	Ymodem_send_array(&byte,1);
}
//
void Ymodem_send_c_frame(void)
{
	Ymodem_send_one_byte(YMODEM_C);
}
//
void Ymodem_send_ack_frame(void)
{
	Ymodem_send_one_byte(YMODEM_ACK);
}
//
void Ymodem_send_nack_frame(void)
{
	Ymodem_send_one_byte(YMODEM_NAK);
}
//
void Ymodem_send_eot_frame(void)
{
	Ymodem_send_one_byte(YMODEM_EOT);
}
//
void Ymodem_receive_start(void)
{
	Ymodem_init();
	Ymodem_send_c_frame();
}
//
void Ymodem_send_start(void)
{
	Ymodem_init();
}
//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
unsigned char Ymodem_send_start_frame(const char* FileName,unsigned int FileLength)
{
    char len_buf[12];
    memset(len_buf,0,12);
    memset((char*)Ymodem_rec_buf,0,YMODEM_PACKET_PATCH_SIZE + 5);
    unsigned int filename_length = strlen(FileName);
    hex_to_ascii(FileLength,len_buf);
    if(filename_length + strlen(len_buf) > (YMODEM_PACKET_PATCH_SIZE - 7)){
      	return 1;
    }
    Ymodem_rec_buf[0] = YMODEM_SOH;  Ymodem_rec_buf[1] = 0x00;  Ymodem_rec_buf[2] = 0xFF;
    for(unsigned int i = 3; i < 3 + filename_length; i++){
        Ymodem_rec_buf[i] = FileName[i - 3];
    }
    for(unsigned int j = 4 + filename_length; j < 4 + filename_length + strlen(len_buf); j++){
        Ymodem_rec_buf[j] = len_buf[j - 4 - filename_length];
    }
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
    Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE + 3] = (g_YmodemInfo.crc>>8)&0xff;
    Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE + 4] = g_YmodemInfo.crc&0xff;
	delay_ms(YMODEM_SEND_TIME_OUT_MS);
	Ymodem_send_array(Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 5);
	return 0;
}
// S:STX 01 FE data[1024/128] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>>
unsigned char Ymodem_send_data_frame(unsigned char frame_index, unsigned int len)
{
    unsigned int temp = len + 3;
    Ymodem_rec_buf[0] = len == YMODEM_PACKET_PATCH_SIZE ? YMODEM_SOH:  YMODEM_STX;
    Ymodem_rec_buf[1] = frame_index;  Ymodem_rec_buf[2] = frame_index^0xff;
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)Ymodem_rec_buf,temp);
    Ymodem_rec_buf[temp] = (g_YmodemInfo.crc>>8)&0xff;
    Ymodem_rec_buf[1 + temp] = g_YmodemInfo.crc&0xff;
	delay_ms(YMODEM_SEND_TIME_OUT_MS);
	Ymodem_send_array(Ymodem_rec_buf,2 + temp);
	return 0;
}
// SOH 00 FF NUL[128] CRCH CRCL
unsigned char Ymodem_send_end_frame(void)
{
    memset((char*)Ymodem_rec_buf,0,YMODEM_PACKET_PATCH_SIZE + 5);
    Ymodem_rec_buf[0] = YMODEM_SOH;Ymodem_rec_buf[1] = 0x00;  Ymodem_rec_buf[2] = 0xFF;
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
    Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE + 3] = (g_YmodemInfo.crc>>8)&0xff;
    Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE + 4] = g_YmodemInfo.crc&0xff;
    delay_ms(YMODEM_SEND_TIME_OUT_MS);
	Ymodem_send_array(Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 5);
	return 0;
}
//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
unsigned char Ymodem_receive_start_frame(void)
{
	unsigned int filename_index = 0;
	unsigned int filelength_index = 0;
	unsigned short crc16 = 0;
	char uart_rec_len[8];
	// char filepath[sizeof g_YmodemInfo.FileName  + 3];
	//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
	g_YmodemInfo.rec_len = ymodem_lua_interface->Ymodem_read_data_wait(Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 5,YMODEM_RECV_TIMEOVER_MS*1000);
	if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5){
		g_YmodemInfo.rec_len = 0;
		crc16 = Cal_CRC16((unsigned char*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
		if(Ymodem_rec_buf[0] == YMODEM_SOH && Ymodem_rec_buf[1] == 0x00 && Ymodem_rec_buf[2] == 0xff
				&&((Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+3]<<8 )| (Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+4])) == crc16){
			 for(filename_index = 3; filename_index < YMODEM_PACKET_PATCH_SIZE; filename_index++){
				 g_YmodemInfo.FileName[filename_index - 3] = Ymodem_rec_buf[filename_index];
				 if(0x00 == Ymodem_rec_buf[filename_index]) break;
			 }
			 if(filename_index < YMODEM_PACKET_PATCH_SIZE){
				 for(filelength_index = filename_index + 1; filelength_index < YMODEM_PACKET_PATCH_SIZE + 3 ; filelength_index++){
					 uart_rec_len[filelength_index - filename_index - 1] = Ymodem_rec_buf[filelength_index];
					 if(0x00 == Ymodem_rec_buf[filelength_index]) break;
				 }
			 }
			 else return 1;
			 if(filelength_index == YMODEM_PACKET_PATCH_SIZE + 3) return 1;
			 g_YmodemInfo.file_size    = ascii_str2val(uart_rec_len,16);
			 g_YmodemInfo.frame_num    = g_YmodemInfo.file_size/YMODEM_PACKET_BLOCK_SIZE;
			 g_YmodemInfo.frame_remain = g_YmodemInfo.file_size%YMODEM_PACKET_BLOCK_SIZE;
			 g_YmodemInfo.frame_total  = g_YmodemInfo.frame_num + (g_YmodemInfo.frame_remain?1:0);
			 g_YmodemInfo.frame_count = 1;
			 ff_unlink(g_YmodemInfo.FileName);

			// 下载使用的盘号
			g_script_volNum = g_YmodemInfo.FileName[0]  - '0';
			if(g_script_volNum > 9 || g_script_volNum > FF_VOLUMES) g_script_volNum = 0;
			g_script_volNum = g_script_volNum + 10;

			 return  0;
		}else return 1;
	}
	return 2;
}
// S:STX 01 FE data[1024/128] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>>
unsigned char Ymodem_receive_data_frame(YmodemReciveCallback cb)
{
	unsigned short crc16 = 0;
	unsigned int temp;
	g_YmodemInfo.rec_len = ymodem_lua_interface->Ymodem_read_data_wait(Ymodem_rec_buf,YMODEM_PACKET_BLOCK_SIZE + 5,YMODEM_RECV_TIMEOVER_MS*1000);
	if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5 || g_YmodemInfo.rec_len == YMODEM_PACKET_BLOCK_SIZE + 5){
		crc16 = Cal_CRC16((unsigned char*)Ymodem_rec_buf,g_YmodemInfo.rec_len - 2);
		if((Ymodem_rec_buf[0] == ((g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5) ? YMODEM_SOH :YMODEM_STX))&& (Ymodem_rec_buf[1] == g_YmodemInfo.frame_count)
				&& (Ymodem_rec_buf[2] == (g_YmodemInfo.frame_count^0xff))
				&&((Ymodem_rec_buf[g_YmodemInfo.rec_len - 2]<<8 )| (Ymodem_rec_buf[g_YmodemInfo.rec_len - 1])) == crc16){
			//Ymodem_rec_buf[g_YmodemInfo.rec_len - 3: 3]
			// 0 1 2  3456... 130 131 132
			if(Ymodem_rec_buf[g_YmodemInfo.rec_len - 3] == 0x1A){
				for(temp = g_YmodemInfo.rec_len - 3; temp > 3; temp--){
					if(Ymodem_rec_buf[temp] != 0x1A)break; // find the last 0x1A
				}
			}
			else temp = g_YmodemInfo.rec_len - 3;
			g_YmodemInfo.frame_count++;
			return cb((unsigned char*)Ymodem_rec_buf,3,temp);
		}
		else{
			return 1;
		}
	}
	return 2;
}
// SOH 00 FF NUL[128] CRCH CRCL
unsigned char Ymodem_receive_end_frame(void)
{
	unsigned short crc16 = 0;
	g_YmodemInfo.rec_len = ymodem_lua_interface->Ymodem_read_data_wait(Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 5,YMODEM_RECV_TIMEOVER_MS*1000);
	if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5){
		crc16 = Cal_CRC16((unsigned char*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
		if(Ymodem_rec_buf[0] == YMODEM_SOH && Ymodem_rec_buf[1] == 0x00 && Ymodem_rec_buf[2] == 0xff
		 &&((Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+3]<<8 )| (Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+4])) == crc16){
			 return 0;
		}else return 1;
	}
	return 2;
}
//
void Ymodem_HandShake_Handle(unsigned char* data,unsigned int len){
	if(len){
		if(0 == strncmp((char*)data,"Reset",5)){
	//		unsigned temp = reg_rst2;
	//		reg_rst2 = 0x00;
	//		delay_ms(1000);
	//		reg_rst2 = temp;
		}
		else if(0 == strncmp((char*)data,"Ready to send",13)){
			Ymodem_send_str("Ready");
			g_YmodemInfo.state = Ymode_Receive_HandShake;// 上位机准备发送数据  下位机准备接收数据

		}
		else if(0 == strncmp((char*)data,"Ready to receive",16)){
			Ymodem_send_str("Ready");
			g_YmodemInfo.state = Ymode_Send_HandShake; // 上位机发送了准备接收的握手包
		}
		else if(0 == strncmp((char*)data,"scan",4)){   // scan0.lua    scan0*
			char path[3];
			path[0] = data[4]; path[1]=':';path[2]=0;
			char suffix[8];
			suffix[len-5] = 0;
			for(unsigned char index = 5; index < len; index++)
				suffix[index-5] = data[index];
			ff_scan_root_lua_files(path,suffix);
			g_YmodemInfo.state = Ymode_Scan;
		}
		else if(0 == strncmp((char*)data,"Delete",6)){ // Delete0:/123.lua
			g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)data,len - 2);
			if(((data[len - 2]<<8)| data[len - 1]) == g_YmodemInfo.crc){
				memcpy(g_YmodemInfo.FileName,&data[6],len - 8);
				g_YmodemInfo.FileName[len-8] = 0;
				f_unlink(g_YmodemInfo.FileName);
				Ymodem_send_str("Ready");
				g_YmodemInfo.state = Ymode_Delte;
			}
		}
		else{//0:main.lua
			g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)data,len - 2);
			if(((g_script_volNum-10+'0')==data[0])&&((data[len - 2]<<8)| data[len - 1]) == g_YmodemInfo.crc){
				memcpy(g_YmodemInfo.FileName,data,len - 2);
				g_YmodemInfo.FileName[len-2] = 0;
				Ymodem_send_str("Ready");
				g_YmodemInfo.state = Ymode_Send_FileName;// 上位机准备接收数据  下位机准备发送数据
			}
		}
	}
}

unsigned char Ymodem_receive(void)
{
    unsigned char result = 0;
    Ymodem_receive_start();
    result = Ymodem_receive_start_frame();
    if(result){
    	return result;
    }

    Ymodem_send_ack_frame();
    Ymodem_send_c_frame();

    result = f_open(&g_file,g_YmodemInfo.FileName,FA_OPEN_ALWAYS|FA_WRITE);
    if(result){
    	goto Ymodem_Receive_ERROR;
    }

    do{
		result = Ymodem_receive_data_frame(Ymodem_ReciveDataCallback);
		if(result){
			if(0 == g_YmodemInfo.maxTryTimes--){ //结束
				Ymodem_send_eot_frame();
				goto Ymodem_Receive_ERROR;;
			}
			else{ // 重传
				g_YmodemInfo.frame_count--;
				Ymodem_send_nack_frame();
			}
		}
		else  Ymodem_send_ack_frame();
    }while(g_YmodemInfo.frame_count != g_YmodemInfo.frame_total + 1);

    result = Ymodem_receive_eot_frame();
    if(result){
    	goto Ymodem_Receive_ERROR;
    }
    Ymodem_send_nack_frame();
	Ymodem_rec_buf[0] = YMODEM_EOT^0xff;

    result = Ymodem_receive_eot_frame();
    if(result){
    	goto Ymodem_Receive_ERROR;
    }
    Ymodem_send_ack_frame();
    Ymodem_send_c_frame();

    result = Ymodem_receive_end_frame();
    if(result){
    	goto Ymodem_Receive_ERROR;
    }
    Ymodem_send_ack_frame();

Ymodem_Receive_ERROR:
    f_close(&g_file);
	return result;
}

unsigned char Ymodem_send(void)
{
	unsigned int read_size;
    unsigned char result = 0;

    Ymodem_send_start();
    result = Ymodem_receive_c_frame();
    if(result){
    	return result;
    }
    result =  f_open(&g_file,g_YmodemInfo.FileName,FA_OPEN_ALWAYS|FA_READ);
    if(FR_OK == result){
    	g_YmodemInfo.file_size    = f_size(&g_file);
		g_YmodemInfo.frame_num    = g_YmodemInfo.file_size/YMODEM_PACKET_BLOCK_SIZE;
		g_YmodemInfo.frame_remain = g_YmodemInfo.file_size%YMODEM_PACKET_BLOCK_SIZE;
		g_YmodemInfo.frame_total  = g_YmodemInfo.frame_num + (g_YmodemInfo.frame_remain?1:0);
		if(g_YmodemInfo.file_size > 255 * YMODEM_PACKET_BLOCK_SIZE){
			goto Ymodem_Send_ERROR;
		}
    }
    else{
    	goto Ymodem_Send_ERROR;
    }

    result = Ymodem_send_start_frame(g_YmodemInfo.FileName,g_YmodemInfo.file_size);
    if(result){
    	goto Ymodem_Send_ERROR;
    }

    result = Ymodem_receive_ack_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }

    result = Ymodem_receive_c_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }
    // send file
    for(g_YmodemInfo.frame_count = 1; g_YmodemInfo.frame_count < g_YmodemInfo.frame_total; g_YmodemInfo.frame_count++){
    	if(FR_OK == f_lseek(&g_file,YMODEM_PACKET_BLOCK_SIZE * (g_YmodemInfo.frame_count - 1))){
    		result = f_read(&g_file,(char*)(&Ymodem_rec_buf[3]),YMODEM_PACKET_BLOCK_SIZE,&read_size);
    		if(result || YMODEM_PACKET_BLOCK_SIZE != read_size) goto Ymodem_Send_ERROR;

    		g_YmodemInfo.rec_len = 0;
        	Ymodem_send_data_frame(g_YmodemInfo.frame_count,YMODEM_PACKET_BLOCK_SIZE);
        	result = Ymodem_receive_one_byte();
        	if(YMODEM_ACK == result) continue;
        	else if(YMODEM_NAK == result)  g_YmodemInfo.frame_count--;
        	else goto Ymodem_Send_ERROR;
    	}
    	else{
        	goto Ymodem_Send_ERROR;
    	}
    }

    while(g_YmodemInfo.frame_count == g_YmodemInfo.frame_total){ // last frame
    	memset((char*)Ymodem_rec_buf,0x1A,YMODEM_PACKET_BLOCK_SIZE + 5);
    	if(FR_OK == f_lseek(&g_file,YMODEM_PACKET_BLOCK_SIZE * (g_YmodemInfo.frame_count - 1))){
    		result = f_read(&g_file,(char*)(&Ymodem_rec_buf[3]),g_YmodemInfo.frame_remain,&read_size);
    		if(result || g_YmodemInfo.frame_remain != read_size) goto Ymodem_Send_ERROR;
        	Ymodem_send_data_frame(g_YmodemInfo.frame_count,g_YmodemInfo.frame_remain < YMODEM_PACKET_PATCH_SIZE ? YMODEM_PACKET_PATCH_SIZE:YMODEM_PACKET_BLOCK_SIZE);
        	result = Ymodem_receive_one_byte();
        	if(YMODEM_ACK == result) g_YmodemInfo.frame_count=0;
        	else if(YMODEM_NAK == result)continue;
        	else goto Ymodem_Send_ERROR;
    	}
    	else{
        	goto Ymodem_Send_ERROR;
    	}
    }

    Ymodem_send_eot_frame();
    result = Ymodem_receive_nak_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }

    Ymodem_send_eot_frame();
    result = Ymodem_receive_ack_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }

    result = Ymodem_receive_c_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }
    Ymodem_send_end_frame();
    result = Ymodem_receive_ack_frame();
    if(result){
    	goto Ymodem_Send_ERROR;
    }
Ymodem_Send_ERROR:
    f_close(&g_file);
	return result;
}

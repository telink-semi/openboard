#include "ymodem.h"

YmodemInfo g_YmodemInfo;

#define Ymodem_send_array		uart_send_array
#define Ymodem_rec_handle		uart_rec_handle
#define Ymodem_rec_buf			UART_RX_BUF

unsigned char Ymodem_ReciveDataCallback(unsigned char* data,uint32_t start_index,uint32_t end_index)
{
	FRESULT res;
	res = f_open(&g_file,g_YmodemInfo.FileName,FA_OPEN_ALWAYS|FA_WRITE);
	if(FR_OK == res){
		res = f_lseek(&g_file,YMODEM_PACKET_BLOCK_SIZE * (g_YmodemInfo.frame_count - 2));
		if(res) return res;
		res = f_write(&g_file,&data[start_index], end_index - start_index + 1, &g_bw);
		return f_close(&g_file); // todo
	}
	return res;
}

void Ymodem_ReciveCallback (__attribute__((unused)) char* data,u32 len)
{
    g_YmodemInfo.rec_len = len;
}

void Ymodem_init(Ymodem_initCallback Ymodem_init_pre)
{
	if(NULL != Ymodem_init_pre)	Ymodem_init_pre();
	memset(g_YmodemInfo.FileName, 0, sizeof g_YmodemInfo.FileName);
    g_YmodemInfo.ymodem_mode  = Idle_Mode;
    g_YmodemInfo.frame_count  = 1;
    g_YmodemInfo.rec_len      = 0;
    g_YmodemInfo.frame_num 	  = 0;
    g_YmodemInfo.frame_remain = 0;
    g_YmodemInfo.frame_total  = 0;
    g_YmodemInfo.file_size = 0;
}

void Ymodem_send_one_byte(unsigned char byte)
{
	char sendBuf[1] = {byte};
	Ymodem_send_array(sendBuf,1);
	delay_ms(YMODEM_SEND_DELAY_MS);
}

void Ymodem_send_c_frame(void)
{
	Ymodem_send_one_byte(YMODEM_C);
}

void Ymodem_send_ack_frame(void)
{
	Ymodem_send_one_byte(YMODEM_ACK);
}

void Ymodem_send_nack_frame(void)
{
	Ymodem_send_one_byte(YMODEM_NAK);
}

void Ymodem_send_eot_frame(void)
{
	Ymodem_send_one_byte(YMODEM_EOT);
}

//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
u8 Ymodem_send_start_frame(const char* FileName,u32 FileLength)
{
    char sendBuf[YMODEM_PACKET_PATCH_SIZE + 5];
    char len_buf[12];
    unsigned int filename_length = strlen(FileName);
    if(filename_length > (YMODEM_PACKET_PATCH_SIZE - 1)){
        // printf("error:File name exceeds %d bytes, please restart\r\n",YMODEM_PACKET_PATCH_SIZE - 1);
      	return TLK_ELENGTH;
    }
    memset(len_buf,0,12);
    memset(sendBuf,0,YMODEM_PACKET_PATCH_SIZE + 5);
    sendBuf[0] = YMODEM_SOH;  sendBuf[1] = 0x00;  sendBuf[2] = 0xFF;
    for(unsigned int i = 3; i < 3 + filename_length; i++){
        sendBuf[i] = FileName[i - 3];
    }
	hex_to_ascii(FileLength,len_buf);
    for(unsigned int j = 4 + filename_length; j < 4 + filename_length + strlen(len_buf); j++){
        sendBuf[j] = len_buf[j - 4 - filename_length];
    }
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)sendBuf,YMODEM_PACKET_PATCH_SIZE + 3);
    sendBuf[YMODEM_PACKET_PATCH_SIZE + 3] = (g_YmodemInfo.crc>>8)&0xff;
    sendBuf[YMODEM_PACKET_PATCH_SIZE + 4] = g_YmodemInfo.crc&0xff;
	Ymodem_send_array(sendBuf,YMODEM_PACKET_PATCH_SIZE + 5);
	delay_ms(YMODEM_SEND_DELAY_MS);
	return TLK_ENONE;
}
// S:STX 01 FE data[1024/128] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>>
u8 Ymodem_send_data_frame(const char* data,u8 frame_index, u32 len)
{
    char sendBuf[YMODEM_PACKET_BLOCK_SIZE + 5];
    unsigned int temp = len > YMODEM_PACKET_PATCH_SIZE ? YMODEM_PACKET_BLOCK_SIZE + 3 : YMODEM_PACKET_PATCH_SIZE + 3;
    memset(sendBuf,0x1A,temp + 2);

    sendBuf[0] = len > YMODEM_PACKET_PATCH_SIZE ? YMODEM_STX : YMODEM_SOH;  sendBuf[1] = frame_index;  sendBuf[2] = ~frame_index;
    for(unsigned int i = 0; i < len; i++){
        sendBuf[i + 3] = data[i];
    }
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)sendBuf,temp);
    sendBuf[temp] = (g_YmodemInfo.crc>>8)&0xff;
    sendBuf[1 + temp] = g_YmodemInfo.crc&0xff;
	Ymodem_send_array(sendBuf,2 + temp);
	delay_ms(YMODEM_SEND_DELAY_MS);
	return TLK_ENONE;
}
// SOH 00 FF NUL[128] CRCH CRCL
u8 Ymodem_send_end_frame(void)
{
    char sendBuf[YMODEM_PACKET_PATCH_SIZE + 5];
    memset(sendBuf,0,YMODEM_PACKET_PATCH_SIZE + 5);
    sendBuf[0] = YMODEM_EOT;sendBuf[1] = 0x00;  sendBuf[2] = 0xFF;
    g_YmodemInfo.crc = Cal_CRC16((const unsigned char*)sendBuf,YMODEM_PACKET_PATCH_SIZE + 3);
    sendBuf[YMODEM_PACKET_PATCH_SIZE + 3] = (g_YmodemInfo.crc>>8)&0xff;
    sendBuf[YMODEM_PACKET_PATCH_SIZE + 4] = g_YmodemInfo.crc&0xff;
	Ymodem_send_array(sendBuf,YMODEM_PACKET_PATCH_SIZE + 5);
	delay_ms(YMODEM_SEND_DELAY_MS);
	return TLK_ENONE;
}
//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
u8 Ymodem_receive_start_frame(void)
{
	unsigned int filename_index = 0;
	unsigned int filelength_index = 0;
	unsigned short crc16 = 0;
	char uart_rec_len[8];
	// char filepath[sizeof g_YmodemInfo.FileName  + 3];
	unsigned int stamp = stimer_get_tick();
	//S:SOH 00 FF [FileName 00]" "[FileLength 00]’’ NULL CRC CRC >>
	do{
		Ymodem_rec_handle(Ymodem_ReciveCallback);
		if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5){
			g_YmodemInfo.rec_len = 0;
			crc16 = Cal_CRC16((u8*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
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
				 else return TLK_ECHECK;
				 if(filelength_index == YMODEM_PACKET_PATCH_SIZE + 3) return TLK_ECHECK;
				 g_YmodemInfo.file_size = tlkapi_ascii_str2val(uart_rec_len,16);
				 g_YmodemInfo.frame_num    = g_YmodemInfo.file_size/YMODEM_PACKET_BLOCK_SIZE;
				 g_YmodemInfo.frame_remain = g_YmodemInfo.file_size%YMODEM_PACKET_BLOCK_SIZE;
				 g_YmodemInfo.frame_total  = g_YmodemInfo.frame_num + (g_YmodemInfo.frame_remain?1:0);
				 g_YmodemInfo.frame_count = 1;
				 ff_unlink(g_YmodemInfo.FileName);
//				 printf("file_size:%d  FileName: %s\r\n",g_YmodemInfo.file_size,g_YmodemInfo.FileName);
				 return  TLK_ENONE;
			}else return TLK_ECHECK;
		}
	}while(!clock_time_exceed(stamp,YMODEM_RECV_TIMEOVER_MS*1000));
	return TLK_TIMEOUT;
}
// S:STX 01 FE data[1024/128] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>>
u8 Ymodem_receive_data_frame(YmodemReciveCallback cb)
{
	unsigned short crc16 = 0;
	unsigned int temp;
	unsigned int stamp = stimer_get_tick();
	do{
		Ymodem_rec_handle(Ymodem_ReciveCallback);
		if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5 || g_YmodemInfo.rec_len == YMODEM_PACKET_BLOCK_SIZE + 5){
			crc16 = Cal_CRC16((u8*)Ymodem_rec_buf,g_YmodemInfo.rec_len - 2);
//			printf("crc16:%d\r\n",crc16 );
			if((Ymodem_rec_buf[0] == ((g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5) ? YMODEM_SOH :YMODEM_STX))&& (Ymodem_rec_buf[1] == g_YmodemInfo.frame_count)
					&& (Ymodem_rec_buf[2] == (g_YmodemInfo.frame_count^0xff))
					&&((Ymodem_rec_buf[g_YmodemInfo.rec_len - 2]<<8 )| (Ymodem_rec_buf[g_YmodemInfo.rec_len - 1])) == crc16){
				//Ymodem_rec_buf[g_YmodemInfo.rec_len - 3: 3]
				if(Ymodem_rec_buf[g_YmodemInfo.rec_len - 3] == 0x1A){
					for(temp = g_YmodemInfo.rec_len - 4; temp > 3; temp--){
						if(Ymodem_rec_buf[temp] != 0x1A)break; // finde the last 0x1A
					}
				}
				else temp = g_YmodemInfo.rec_len - 3;
//				printf("temp:%d\r\n",temp );
				g_YmodemInfo.rec_len = 0;
				g_YmodemInfo.frame_count++;
				return cb((u8*)Ymodem_rec_buf,3,temp);
			}
			else{
//				printf("\r\n\r\n");
//				printf("Ymodem_rec_buf[1]:%d\r\n",Ymodem_rec_buf[1] );
//				printf("g_YmodemInfo.frame_count:%d\r\n",g_YmodemInfo.frame_count );
				g_YmodemInfo.rec_len = 0;
				return TLK_ECHECK;
			}
		}
	}while(!clock_time_exceed(stamp,YMODEM_RECV_TIMEOVER_MS*1000));
	return TLK_TIMEOUT;
}
// S:EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
u8 Ymodem_receive_eot_frame(void)
{
	unsigned int stamp = stimer_get_tick();
	do{
		Ymodem_rec_handle(Ymodem_ReciveCallback);
		if(g_YmodemInfo.rec_len == 1 && Ymodem_rec_buf[0] == YMODEM_EOT) return TLK_ENONE;
	}while(!clock_time_exceed(stamp,YMODEM_RECV_TIMEOVER_MS*1000));
	return TLK_TIMEOUT;
}
// SOH 00 FF NUL[128] CRCH CRCL
u8 Ymodem_receive_end_frame(void)
{
	unsigned short crc16 = 0;
	unsigned int stamp = stimer_get_tick();
	do{
		Ymodem_rec_handle(Ymodem_ReciveCallback);
		if(g_YmodemInfo.rec_len == YMODEM_PACKET_PATCH_SIZE + 5){
			crc16 = Cal_CRC16((u8*)Ymodem_rec_buf,YMODEM_PACKET_PATCH_SIZE + 3);
			if(Ymodem_rec_buf[0] == YMODEM_SOH && Ymodem_rec_buf[1] == 0x00 && Ymodem_rec_buf[2] == 0xff
			 &&((Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+3]<<8 )| (Ymodem_rec_buf[YMODEM_PACKET_PATCH_SIZE+4])) == crc16){
				 return TLK_ENONE;
			}else return TLK_ECHECK;
		}
	}while(!clock_time_exceed(stamp,YMODEM_RECV_TIMEOVER_MS*1000));
	return TLK_TIMEOUT;
}

void Ymodem_receive_start(void)
{
	Ymodem_init(NULL);
	Ymodem_send_c_frame();
}

void Ymodem_receive_stop(void)
{
	Ymodem_init(NULL);
}

unsigned char Ymodem_receive(void)
{
    u8 result = TLK_ENONE;
    Ymodem_receive_start();
    result = Ymodem_receive_start_frame();
    if(result){
    	printf("Ymodem_receive_start_frame error\r\n");
    	printf("File:%s,Line:%d\r\n", __FILE__, __LINE__);
    	return result;
    }
    Ymodem_send_ack_frame();
    Ymodem_send_c_frame();

    do{
		result = Ymodem_receive_data_frame(Ymodem_ReciveDataCallback);
		if(result){
			printf("Ymodem_receive_data_frame error\r\n");
			printf("File:%s,Line:%d\r\n", __FILE__, __LINE__);
			return result; // g_YmodemInfo.frame_count--; todo
		}
		Ymodem_send_ack_frame();
    }while(g_YmodemInfo.frame_count != g_YmodemInfo.frame_total + 1);
	// printf("%d - %d\r\n",g_YmodemInfo.frame_count,g_YmodemInfo.frame_total);
    result = Ymodem_receive_eot_frame();
    if(result){
    	printf("Ymodem_receive_eot_frame error\r\n");
    	printf("File:%s,Line:%d\r\n", __FILE__, __LINE__);
    	return result;
    }
    Ymodem_send_nack_frame();
	Ymodem_rec_buf[0] = ~YMODEM_EOT;

    result = Ymodem_receive_eot_frame();
    if(result){
    	printf("Ymodem_receive_eot_frame error\r\n");
    	printf("File:%s,Line:%d\r\n", __FILE__, __LINE__);
    	return result;
    }
    Ymodem_send_ack_frame();
    Ymodem_send_c_frame();

    result = Ymodem_receive_end_frame();
    if(result){
    	printf("Ymodem_receive_end_frame error\r\n");
    	printf("File:%s,Line:%d\r\n", __FILE__, __LINE__);
    	return result;
    }
    Ymodem_send_ack_frame();
    f_close(&g_file);
    Ymodem_receive_stop();
	return result;
}






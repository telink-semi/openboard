#ifndef __YMODEM_H
#define __YMODEM_H

#include <stdio.h>
#include "../ff15/extend/fatfs_extend.h"
#include "../../lua_interface/my_lua_interface.h"

#define  LUA_BREAK_POINT_CONFIG_FILE_NAME           "break_points.config"
#define	 LUA_MAX_BREAK_POINTS						16
#define	 LUA_BREAK_POINT_SPLIT_ELE					(':')	// ascii字符


/*
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< C
S:SOH 00 FF [55 53…6E 00]" "[32…30 00]’’ NUL[96] CRC CRC >>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< C
S:STX 01 FE data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
S:STX 02 FD data[1024] CRC CRC>>>>>>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
S:SOH 05 FA data[100] 1A[28] CRC CRC>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
S:EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< NACK
S:EOT>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< C
S:SOH 00 FF NUL[128] CRCCRC >>>>>>>>>>>>>>>>>>>>>>
R:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< ACK
*/
#define YMODEM_RECV_TIMEOVER_MS   	           1000	 // 发送完数据后多久内得收到数据
#define YMODEM_REC_TIME_OUT_MS       			20   // UART接收两笔数据最小间隔
#define YMODEM_SEND_TIME_OUT_MS       			50   // 收到数据后延时多久发送
#define YMODEM_PACKET_PATCH_SIZE             (128)   // patch大小
#define YMODEM_PACKET_BLOCK_SIZE            (1024)   // 数据块大小

#define  YMODEM_REC_LEN  		(YMODEM_PACKET_BLOCK_SIZE + 32)

extern unsigned char Ymodem_rec_buf[YMODEM_REC_LEN];

extern  _m_lua_interface* ymodem_lua_interface;

typedef unsigned char (*YmodemReciveCallback)(unsigned char*,uint32_t,uint32_t);

#define YMODEM_SOH         (0x01)  /* start of 128-byte data packet */
#define YMODEM_STX         (0x02)  /* start of 1024-byte data packet */
#define YMODEM_EOT         (0x04)  /* end of transmission */
#define YMODEM_ACK         (0x06)  /* acknowledge */
#define YMODEM_NAK         (0x15)  /* negative acknowledge */
#define YMODEM_CA          (0x18)  /* two of these in succession aborts transfer */
#define YMODEM_C           (0x43)  /* 'C' == 0x43 */

typedef enum{
	Ymode_Idle = 0,
	Ymode_Scan = 1,
	Ymode_Send_HandShake = 2,
	Ymode_Send_FileName = 3,
	Ymode_Receive_HandShake = 4,
	Ymode_Delte = 5,
}ymode_state;
typedef struct{
    char FileName[128];  			// 待发送的文件名 min(YMODEM_PACKET_PATCH_SIZE,FF_MAX_LFN + 1)
    unsigned int file_size;  		// 文件大小
    unsigned int rec_len;			// 一笔数据接收到的大小
    unsigned int frame_remain;		// 帧余数
    unsigned char frame_num;		// 帧数
    unsigned char frame_total;		// 帧总数
    unsigned char frame_count;		// 帧计数
    unsigned char maxTryTimes;		// 最大重试次数
    unsigned short crc; 			// CRC
    ymode_state state;				//
}YmodemInfo;

extern YmodemInfo g_YmodemInfo;

void Ymodem_pre(_m_lua_interface* interface);
void Ymodem_init(void);
void Ymodem_send_str(char *array);
void Ymodem_HandShake_Handle(unsigned char* data,unsigned int len);
unsigned char Ymodem_receive(void);
unsigned char Ymodem_send(void);
unsigned char get_script_volNum(void);

#endif



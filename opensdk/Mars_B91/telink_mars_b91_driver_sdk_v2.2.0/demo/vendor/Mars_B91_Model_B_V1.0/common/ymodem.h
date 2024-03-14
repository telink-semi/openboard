#ifndef __YMODEM_H
#define __YMODEM_H

#include <stdio.h>
#include "util.h"
#include "driver.h"
#include "../device/sdcard.h"
#include "../common/malloc.h"
#include "../device/serial_port.h"
#include "../ff15/extend/fatfs_extend.h"

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
#define YMODEM_RECV_TIMEOVER_MS   	       500*1000
#define YMODEM_SEND_DELAY_MS       			  20
#define YMODEM_PACKET_PATCH_SIZE            (128)
#define YMODEM_PACKET_BLOCK_SIZE           (1024)

typedef unsigned char (*YmodemReciveCallback)(unsigned char*,uint32_t,uint32_t);

typedef void (*Ymodem_initCallback)(void);


#define YMODEM_SOH         (0x01)  /* start of 128-byte data packet */
#define YMODEM_STX         (0x02)  /* start of 1024-byte data packet */
#define YMODEM_EOT         (0x04)  /* end of transmission */
#define YMODEM_ACK         (0x06)  /* acknowledge */
#define YMODEM_NAK         (0x15)  /* negative acknowledge */
#define YMODEM_CA          (0x18)  /* two of these in succession aborts transfer */
#define YMODEM_C           (0x43)  /* 'C' == 0x43 */

typedef enum{
    COM_OK = 0,
    COM_LIMIT = 1,
    COM_DATA = 2,
    COM_ABORT = 3,
    COM_ERROR = 4,
    COM_CONTIUNE = 5,
}COM_StatusTypeDef;

typedef enum{
    Write_Mode,
    Read_Mode,
    Idle_Mode,
}Ymodem_Mode;

typedef struct{
    char FileName[max(FF_MAX_LFN,YMODEM_PACKET_PATCH_SIZE)];
    unsigned int file_size;
    unsigned char frame_num;
    unsigned char frame_remain;
    unsigned char frame_total;
    unsigned char frame_count;
    unsigned int rec_len;
    unsigned short crc;
    Ymodem_Mode ymodem_mode;
}YmodemInfo;

void Ymodem_init(Ymodem_initCallback Ymodem_init_pre);
unsigned char Ymodem_receive(void);

#endif



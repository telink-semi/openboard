#ifndef	__SDCARD_H
#define	__SDCARD_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define SD_TYPE_ERR     	0X00
#define SD_TYPE_MMC     	0X01
#define SD_TYPE_V1      	0X02
#define SD_TYPE_V2      	0X04
#define SD_TYPE_V2HC    	0X06

#define CMD0    			0       //reset card
#define CMD1    			1
#define CMD8    			8       //SEND_IF_COND
#define CMD9   			 	9       //read CSD
#define CMD10   			10      //read CID
#define CMD12   			12      //stop data transmission
#define CMD16   			16      //set SectorSize,should return 0x00
#define CMD17   			17      //read sector
#define CMD18   			18      //read multiple sector
#define CMD23   			23      //set up N blocks to be erased before writing to multiple sectors
#define CMD24   			24      //write sector
#define CMD25   			25      //write multiple sector
#define CMD41   			41      //should return 0x00
#define CMD55   			55      //should return 0x01
#define CMD58   			58      //read OCR
#define CMD59   			59      //enable/disable CRC，should return 0x00

#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF

void SD_CS_LOW(void);
void SD_CS_HIGH(void);

unsigned int  SD_GetSectorCount(void);
unsigned int SD_Card_init(void);
int SD_ReadDisk(unsigned char*buf,unsigned int sector,unsigned int cnt);

unsigned int SD_WaitReady(void);
int SD_WriteDisk(unsigned char*buf,unsigned int sector,unsigned int cnt);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif


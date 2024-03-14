#include "../app_config.h"
#include "sdcard.h"

unsigned char SD_Type = SD_TYPE_ERR;

pspi_pin_config_t pspi_sdnand_pin_config = {
	.pspi_clk_pin 		= PSPI_CK_PIN,
	.pspi_csn_pin   	= PSPI_NONE_PIN,
	.pspi_mosi_io0_pin  = PSPI_MOSI_PIN,
	.pspi_miso_io1_pin  = PSPI_MISO_PIN,
};

static void SD_SPI_Init(void)
{
	gpio_output_en(PSPI_CN_PIN);
	gpio_input_dis(PSPI_CN_PIN);
	SD_CS_HIGH();
	gpio_function_en(PSPI_CN_PIN);
	pspi_set_pin(&pspi_sdnand_pin_config);
	spi_master_init(PSPI_MODULE, sys_clk.pclk * 1000000 / (2 * SD_FOD_CLK) - 1, SD_SPI_MODE);
	spi_master_config(PSPI_MODULE, SPI_NORMAL);
}

static void SD_SPI_SpeedLow(void)
{
	spi_master_init(PSPI_MODULE, sys_clk.pclk * 1000000 / (2 * SD_FOD_CLK) - 1, SD_SPI_MODE);
}

static void SD_SPI_SpeedHigh(void)
{
	spi_master_init(PSPI_MODULE, sys_clk.pclk * 1000000 / (2 * SD_FPP_CLK) - 1, SD_SPI_MODE);
}

void SD_CS_HIGH(void)
{
	gpio_set_low_level(PSPI_CN_PIN);
}

void SD_CS_LOW(void)
{
	gpio_set_high_level(PSPI_CN_PIN);
}

static unsigned char SD_SPI_ReadWriteByte(unsigned char TxData)
{
	unsigned char RxData = 0;
	spi_master_write_read_full_duplex(PSPI_MODULE, &TxData, &RxData, 1);
	return RxData;
}

unsigned char SD_WaitReady(void)
{
	unsigned int Retry = 0xFF;
	do
	{
		if(SD_SPI_ReadWriteByte(0XFF) == 0XFF) return 0;
		delay_us(10);
		Retry--;
	}while(Retry);
	return 1;
}


static void SD_DisSelect(void)
{
	SD_CS_HIGH();
	SD_SPI_ReadWriteByte(0xff);
}

static unsigned char SD_Select(void)
{
	SD_CS_LOW();
	if(SD_WaitReady() == 0) return 0;
	SD_DisSelect();
	return 1;
}

unsigned char SD_SendCmd(unsigned char cmd, unsigned int arg, unsigned char crc)
{
	unsigned char r1;
	SD_DisSelect();
	if(SD_Select())return 0XFF;
	SD_SPI_ReadWriteByte(cmd | 0x40);
	SD_SPI_ReadWriteByte(arg >> 24);
	SD_SPI_ReadWriteByte(arg >> 16);
	SD_SPI_ReadWriteByte(arg >> 8);
	SD_SPI_ReadWriteByte(arg);
	SD_SPI_ReadWriteByte(crc);
	if(cmd == CMD12) SD_SPI_ReadWriteByte(0xff);
	unsigned char Retry = 0XFF;
	do
	{
		r1 = SD_SPI_ReadWriteByte(0xff);
		delay_us(10);
	}while((r1&0X80) && Retry--);
	return r1;
}

unsigned char SD_Card_init(void)
{
    unsigned char r1;
    unsigned short retry;
    unsigned char buf[10];
	unsigned short i;
	SD_SPI_Init();
 	SD_SPI_SpeedLow();
 	for(i = 0; i < 10; i++)	SD_SPI_ReadWriteByte(0XFF);
	retry = 20;
	do
	{
		r1 = SD_SendCmd(CMD0,0,0x95); // IDLE
	}while((r1 != 0X01) && retry--);

	if(r1 == 0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87) == 1)//SD V2.0
		{
			for(i = 0; i < 4; i++) buf[i] = SD_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2] == 0X01 && buf[3] == 0XAA)//support 2.7~3.6V
			{
				retry = 0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);
					r1 = SD_SendCmd(CMD41,0x40000000,0X01);
				}while(r1 && retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01) == 0)
				{
					for(i = 0; i < 4; i++) buf[i] = SD_SPI_ReadWriteByte(0XFF);//get OCR
					if(buf[0]&0x40) SD_Type = SD_TYPE_V2HC;
					else SD_Type = SD_TYPE_V2;
				}
			}
		}
		else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);
			r1=SD_SendCmd(CMD41,0,0X01);
			if(r1<=1)
			{
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);
					r1=SD_SendCmd(CMD41,0,0X01);
				}while(r1&&retry--);
			}else
			{
				SD_Type=SD_TYPE_MMC;
				retry=0XFFFE;
				do
				{
					r1=SD_SendCmd(CMD1,0,0X01);
				}while(r1&&retry--);
			}
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;
		}
	}
	SD_DisSelect();
	SD_SPI_SpeedHigh();
	if(SD_Type)return 0;
	else if(r1)return r1;
	return 0xaa;
}

unsigned char SD_GetResponse(unsigned char Response)
{
	unsigned short Count=0xFFFF;
	while ((SD_SPI_ReadWriteByte(0XFF)!=Response)&&Count)Count--;
	if (Count==0)return MSD_RESPONSE_FAILURE;
	else return MSD_RESPONSE_NO_ERROR;
}

unsigned char SD_RecvData(unsigned char*buf,unsigned short len)
{
	if(SD_GetResponse(0xFE)) return 1;
    while(len--)
    {
        *buf = SD_SPI_ReadWriteByte(0xFF);
        buf++;
    }
    // dummy CRC
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);
    return 0;
}

unsigned char SD_SendBlock(unsigned char*buf,unsigned char cmd)
{
	unsigned short t;
	if(SD_WaitReady())return 1;
	SD_SPI_ReadWriteByte(cmd);
	if(cmd != 0XFD)
	{
		for(t = 0; t < 512; t++)  SD_SPI_ReadWriteByte(buf[t]);
	    SD_SPI_ReadWriteByte(0xFF); // ignore CRC
	    SD_SPI_ReadWriteByte(0xFF);
		t=SD_SPI_ReadWriteByte(0xFF);
		if((t&0x1F)!=0x05)return 2;
	}
    return 0;
}

unsigned char SD_ReadDisk(unsigned char*buf,unsigned int sector,unsigned char cnt)
{
	unsigned char r1;
	if(SD_Type != SD_TYPE_V2HC) sector <<= 9; // convert to byte address
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD17,sector,0X01);
		if(r1 == 0)	r1 = SD_RecvData(buf,512);

	}else
	{
		r1 = SD_SendCmd(CMD18,sector,0X01);
		do
		{
			r1 = SD_RecvData(buf,512);
			buf += 512;
		}while(--cnt && r1==0);
		SD_SendCmd(CMD12,0,0X01);
	}
	SD_DisSelect();
	return r1;
}

unsigned char SD_WriteDisk(unsigned char*buf,unsigned int sector,unsigned char cnt)
{
	unsigned char r1;
	if(SD_Type != SD_TYPE_V2HC)sector *= 512; // convert to byte address
	if(cnt == 1)
	{
		r1 = SD_SendCmd(CMD24,sector,0X01);
		if(r1 == 0)
		{
			r1 = SD_SendBlock(buf,0xFE);
		}
	}else
	{
		if(SD_Type != SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);
			SD_SendCmd(CMD23,cnt,0X01);
		}
 		r1 = SD_SendCmd(CMD25,sector,0X01);
		if(r1 == 0)
		{
			do
			{
				r1 = SD_SendBlock(buf,0xFC);
				buf += 512;
			}while(--cnt && r1==0);
			r1 = SD_SendBlock(0,0xFD);
		}
	}
	SD_DisSelect();
	return r1;
}

unsigned char SD_GetCID(unsigned char *cid_data)
{
	unsigned char r1;
    r1 = SD_SendCmd(CMD10,0,0x01);
    if(r1 == 0x00)
	{
		r1 = SD_RecvData(cid_data,16);
    }
	SD_DisSelect();
	return r1;
}

unsigned char SD_GetCSD(unsigned char *csd_data)
{
	unsigned char r1;
    r1 = SD_SendCmd(CMD9,0,0x01);
    if(r1 == 0)	r1=SD_RecvData(csd_data, 16);
	SD_DisSelect();
	return r1;
}

unsigned int SD_GetSectorCount(void)
{
	unsigned char csd[16];
    unsigned int Capacity;
    unsigned char n;
	unsigned short csize;
    if(SD_GetCSD(csd)) return 0;
    if((csd[0]&0xC0) == 0x40) // V2.00
    {
		csize = csd[9] + ((unsigned short)csd[8] << 8) + 1;
		Capacity = (unsigned int)csize << 10;
    }else//V1.XX
    {
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((unsigned short)csd[7] << 2) + ((unsigned short)(csd[6] & 3) << 10) + 1;
		Capacity= (unsigned int)csize << (n - 9);
    }
    return Capacity;
}

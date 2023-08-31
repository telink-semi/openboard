/********************************************************************************************************
 * @file    sdcard.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "../../mass_storage/spi_sdnand_driver/sdcard.h"
#include "../../../app_config.h"
#include "driver.h"
#if(SD_NAND_FLASH_SUPPORT)
#define SD_SPI_DMA	    					  	1
#if   SD_SPI_DMA
#define SD_SPI_RTX_DMA							DMA6
#endif
#define SD_FPP_CLK	    					  	4000000
#define SD_FOD_CLK	    					  	240000

pspi_pin_config_t pspi_sdnand_pin_config = {
	.pspi_clk_pin		= PSPI_CLK_PC5,
	.pspi_csn_pin 		= PSPI_NONE_PIN,
	.pspi_mosi_io0_pin  = PSPI_MOSI_IO0_PC7,
	.pspi_miso_io1_pin  = PSPI_MISO_IO1_PC6,//3line mode set none
};
hspi_pin_config_t hspi_sdnand_pin_config = {
	.hspi_clk_pin		= HSPI_CLK_PB4,
	.hspi_csn_pin 		= HSPI_NONE_PIN,
	.hspi_mosi_io0_pin  = HSPI_MOSI_IO0_PB3,
	.hspi_miso_io1_pin  = HSPI_MISO_IO1_PB2,//3line mode set none
	.hspi_wp_io2_pin    = HSPI_NONE_PIN,//set quad mode otherwise set none
	.hspi_hold_io3_pin  = HSPI_NONE_PIN,//set quad mode otherwise set none
};

#define HSPI 1
#define PSPI 0
#define HPSPI_MODULE  PSPI


#if(HPSPI_MODULE == PSPI)
#define SD_CS_PIN                        GPIO_PC4

#else
#define SD_CS_PIN                        GPIO_PB5
#endif
//SD_CardInfo cardinfo;
static void GPIO_Configuration(void)
{
	gpio_output_en(SD_CS_PIN);
	gpio_input_dis(SD_CS_PIN);
	gpio_set_high_level(SD_CS_PIN);
	gpio_function_en(SD_CS_PIN);


#if(HPSPI_MODULE == PSPI)
	pspi_set_pin(&pspi_sdnand_pin_config);
	spi_master_init(HPSPI_MODULE, sys_clk.pclk * 1000000 / (2 * SD_FOD_CLK) - 1, SPI_MODE3);
#else
	hspi_set_pin(&hspi_sdnand_pin_config);
	spi_master_init(HPSPI_MODULE, sys_clk.hclk * 1000000 / (2 * SD_FOD_CLK) - 1, SPI_MODE3);
#endif

	spi_master_config(HPSPI_MODULE, SPI_NORMAL);

}
void SD_CS_HIGH(void)
{
	gpio_set_high_level(SD_CS_PIN);
}

void SD_CS_LOW(void)
{
	gpio_set_low_level(SD_CS_PIN);
}
unsigned char SD_SPI_ReadWriteByte(unsigned char TxData)
{
	unsigned char temp=0;
	spi_master_write_read_full_duplex(HPSPI_MODULE,&TxData, &temp, 1);
	return temp;
}
static void sd_write_data(unsigned char *data_buff, unsigned int length)
{
#if SD_SPI_DMA
	if(((length % 4) == 0) && (length>=4))
	{
		spi_rx_dma_dis(HPSPI_MODULE);

#if(HPSPI_MODULE == PSPI)
		pspi_set_tx_dma_config(SD_SPI_RTX_DMA);
#else
		hspi_set_tx_dma_config(SD_SPI_RTX_DMA);

#endif
		spi_master_write_dma(HPSPI_MODULE, data_buff, length);
	}
	else
#endif
	{
		spi_tx_dma_dis(HPSPI_MODULE);
		spi_master_write(HPSPI_MODULE, data_buff, length);
	}
	while (spi_is_busy(HPSPI_MODULE));
}

void sd_read_data(unsigned char *data_buff, unsigned int length)
{
#if SD_SPI_DMA
	if(((length % 4) == 0) && (length>=4))
	{
		spi_tx_dma_dis(HPSPI_MODULE);
#if(HPSPI_MODULE == PSPI)
		pspi_set_rx_dma_config(SD_SPI_RTX_DMA);
#else
		hspi_set_rx_dma_config(SD_SPI_RTX_DMA);

#endif
		spi_master_read_dma_plus(HPSPI_MODULE, 0x00, 0x00, data_buff, length, SPI_MODE_READ_ONLY);
	}
	else
#endif
	{
		spi_master_read(HPSPI_MODULE, data_buff, length);
	}
	while (spi_is_busy(HPSPI_MODULE));
}

unsigned char  SD_Type=0;


void SD_SPI_SpeedLow(void)
{
	spi_master_init(HPSPI_MODULE, sys_clk.pclk * 1000000 / (2 * SD_FOD_CLK) - 1, SPI_MODE3);
}

void SD_SPI_SpeedHigh(void)
{
	spi_master_init(HPSPI_MODULE,sys_clk.pclk * 1000000 / (2 * SD_FPP_CLK) - 1, SPI_MODE3);
}

void SD_DisSelect(void)
{
	SD_CS_HIGH();
	SD_SPI_ReadWriteByte(0xff);
}

unsigned char SD_Select(void)
{
	SD_CS_LOW();
	if(SD_WaitReady()==0)return 0;
	SD_DisSelect();
	return 1;
}


unsigned char SD_WaitReady(void)
{
	unsigned int t=0;
	unsigned int Count=0x2FF;
	do
	{
		if(SD_SPI_ReadWriteByte(0XFF)==0XFF)return 0;
		delay_us(100);
		t++;
	}while(t<Count);
	return 1;
}

unsigned char SD_GetResponse(unsigned char Response)
{
	unsigned int t=0;
	unsigned int Count=0x3F;
	do
	{
		if(SD_SPI_ReadWriteByte(0xFF)==Response)return MSD_RESPONSE_NO_ERROR;
		delay_us(100);
		t++;
	}while(t<Count);
	return MSD_RESPONSE_FAILURE;
}

unsigned char SD_RecvData(unsigned char*buf,unsigned short len)
{
	if(SD_GetResponse(0xFE))return 1;
	sd_read_data(buf,len);
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);
    return 0;
}

unsigned char SD_SendBlock(unsigned char*buf,unsigned char cmd)
{
	unsigned short t;
	if(SD_WaitReady())return 1;
	SD_SPI_ReadWriteByte(cmd);
	if(cmd!=0XFD)
	{
		sd_write_data(buf, 512);
		SD_SPI_ReadWriteByte(0xFF);
		SD_SPI_ReadWriteByte(0xFF);
		t=SD_SPI_ReadWriteByte(0xFF);
		if((t&0x1F)!=0x05)return 2;
	}
    return 0;
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
	if(cmd==CMD12)SD_SPI_ReadWriteByte(0xff);

	unsigned int t=0;
	unsigned int Count=0XF;
	do
	{
		r1=SD_SPI_ReadWriteByte(0xff);
		if(r1&0X80)
		{
			delay_us(100);
			t++;
		}
		else
		{
			return r1;
		}

	}while(t<Count);
	return r1;
}
extern unsigned char _mscd_buf[];
unsigned char SD_GetCID(unsigned char *cid_data)
{
    unsigned char r1;

    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(_mscd_buf,16);
		memcpy(cid_data,_mscd_buf,16);//in order to align(4) for dma
		//share _mscd_buf, In order to save ram occupation
    }
	SD_DisSelect();
	if(r1)return 1;
	else return 0;
}

unsigned char SD_GetCSD(unsigned char *csd_data)
{
    unsigned char r1;
    r1=SD_SendCmd(CMD9,0,0x01);
    if(r1==0)
	{
		r1=SD_RecvData(_mscd_buf,16);
		memcpy(csd_data,_mscd_buf,16);//in order to align(4) for dma
		//share _mscd_buf, In order to save ram occupation
    }
	SD_DisSelect();
	if(r1)return 1;
	else return 0;
}

unsigned int SD_GetSectorCount(void)
{
    unsigned char csd[16];
    unsigned int Capacity;
    unsigned char n;
	unsigned short csize;

    if(SD_GetCSD(csd)!=0) return 0;

    if((csd[0]&0xC0)==0x40)
    {
		csize = csd[9] + ((unsigned short)csd[8] << 8) + 1;
		Capacity = (unsigned int)csize << 10;
    }else
    {
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((unsigned short)csd[7] << 2) + ((unsigned short)(csd[6] & 3) << 10) + 1;
		Capacity= (unsigned int)csize << (n - 9);
    }
    return Capacity;
}

unsigned char sd_nand_flash_init(void)
{
    unsigned char r1;
    unsigned short retry;
    unsigned char buf[10];
	unsigned short i;

	GPIO_Configuration();
	delay_ms(250);
 	SD_SPI_SpeedLow();
 	for(i=0;i<10;i++)
 	{
 		SD_SPI_ReadWriteByte(0XFF);
 	}

	retry=0X10;
	//enable spi mode
	while(--retry)
	{
		r1=SD_SendCmd(CMD0,0,0x95);
		if(r1 == 0x01)
		{
			break;
		}
		else
		{
			delay_us(100);
		}
	}
	if(retry == 0)
	{
		return 0xaa;
	}

	//wait for busy
	SD_Type=0;
	retry=500;
	do
	{
		delay_ms(1);
		r1=SD_SendCmd(CMD41,0x40000000,0X01);
	}while(r1&&retry--);

	if(retry == 0)
	{
		return 0xbb;
	}

	//read type
	if(retry&&SD_SendCmd(CMD58,0,0X01)==0)
	{
		for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);
		if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;
		else SD_Type=SD_TYPE_V2;
	}
	SD_DisSelect();
	SD_SPI_SpeedHigh();
	if(SD_Type)
	{
		return 0;
	}
	else if(r1)
	{
		return r1;
	}

	return 0xaa;
}


unsigned char SD_ReadDisk(unsigned char*buf,unsigned int sector,unsigned char cnt)
{
	unsigned char r1;
	do
	{
		r1=SD_ReadDisk_lba512(buf,sector);
		buf+=512;
		sector ++;
	}while(--cnt && r1==0);
	return r1;
}
unsigned char SD_ReadDisk_lba512(unsigned char*buf,unsigned int sector)
{
	unsigned char r1;
	if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;

	r1=SD_SendCmd(CMD17,sector,0X01);
	if(r1==0)
	{
		r1=SD_RecvData(_mscd_buf,512);
		memcpy(buf,_mscd_buf,512);//in order to align(4) for dma
		//share _mscd_buf, In order to save ram occupation
	}
	SD_DisSelect();
	return r1;
}
unsigned char SD_WriteDisk(unsigned char*buf,unsigned int sector,unsigned char cnt)
{
	unsigned char r1;
	do
	{
		r1=SD_WriteDisk_lba512(buf,sector);
		buf+=512;
		sector ++;
	}while(--cnt && r1==0);
	return r1;
}

unsigned char SD_WriteDisk_lba512(unsigned char*buf,unsigned int sector)
{
	unsigned char r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;

	r1=SD_SendCmd(CMD24,sector,0X01);
	if(r1==0)
	{
		memcpy(_mscd_buf,buf,512);//in order to align(4) for dma
		//share _mscd_buf, In order to save ram occupation
		r1=SD_SendBlock(_mscd_buf,0xFE);
	}
	SD_DisSelect();
	return r1;
}
////beta function, do not use///////////!!!!!!!!!
unsigned char SD_WriteDisk_lba512_continue(unsigned char*buf,unsigned int sector,unsigned char cnt)
{
	unsigned char r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);
		if(r1==0)
		{
			memcpy(_mscd_buf,buf,512);//in order to align(4) for dma
			//share _mscd_buf, In order to save ram occupation
			r1=SD_SendBlock(_mscd_buf,0xFE);
		}
	}else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);
			SD_SendCmd(CMD23,cnt,0X01);
		}
		r1=SD_SendCmd(CMD25,sector,0X01);
		if(r1==0)
		{
			do
			{
				memcpy(_mscd_buf,buf,512);//in order to align(4) for dma
				//share _mscd_buf, In order to save ram occupation
				r1=SD_SendBlock(_mscd_buf,0xFC);
				buf+=512;
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);
		}
	}
	SD_DisSelect();
	return r1;
}
#endif

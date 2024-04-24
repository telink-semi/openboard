#include "../app_config.h"
#include "sht30.h"

uint8_t SHT3x_CRC_CAL(uint16_t DAT)
{
	uint8_t i,t,temp;
	uint8_t CRC_BYTE;
	CRC_BYTE = 0xFF;
	temp = (DAT>>8) & 0xFF;
	for(t = 0; t < 2; t++){
		CRC_BYTE ^= temp;
		for(i = 0;i < 8;i ++){
			if(CRC_BYTE & 0x80){
				CRC_BYTE <<= 1;
				CRC_BYTE ^= 0x31;
			}
			else{
				CRC_BYTE <<= 1;
			}
		}
		if(t == 0){
			temp = DAT & 0xFF;
		}
	}
	return CRC_BYTE;
}

uint8_t ReadSHT3x(float *Temp,float *Hum)
{
	uint16_t HumData,TempData;
	uint8_t write_buff[2] = {0x2c,0x06};
	uint8_t read_buff[6];
	i2c_master_write(SHT3X_ADDR,(uint8_t *)write_buff, 2);
	i2c_master_read(SHT3X_ADDR,(uint8_t *)read_buff, 6);
	TempData = read_buff[0]<<8 |read_buff[1];
	HumData  = read_buff[3]<<8 |read_buff[4];
    if((SHT3x_CRC_CAL(HumData) == read_buff[5]) && (SHT3x_CRC_CAL(TempData)==read_buff[2]) ){
        *Temp = ((175.0*TempData)/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
        *Hum  = ((100.0*HumData)/65535.0);       // RH = hum*100 / (2^16-1)
    	return 0;
    }
    *Temp = 0;
    *Hum  = 0;
    return -1;
}

void sht3x_test(void)
{
	float Temperature,Humidity;
    unsigned char uart_send_buf[80];
	if(0 == ReadSHT3x(&Temperature,&Humidity)){
		sprintf((char*)uart_send_buf,"Temperature:%.1f,Hum:%.1f\r\n",Temperature,Humidity);
		uart_send_str(uart_send_buf);
	}
	else{
		sprintf((char*)uart_send_buf,"read sht30 error!\r\n");
		uart_send_str(uart_send_buf);
	}
}

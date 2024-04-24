#include "../app_config.h"
#include "bh1750.h"

uint8_t bh1750_Init(void)
{
	unsigned char value = POWER_ON;
	i2c_master_write(BH1750_ADDR, &value, 1);
	value = Measure_Mode;
	i2c_master_write(BH1750_ADDR, &value, 1);
	delay_ms(180);
	return 0;
}

uint16_t bh1750_iic_read_byte(void)
{
	unsigned char read_buf[2]={0};
	i2c_master_read(BH1750_ADDR, read_buf, 2);
	return ((read_buf[0]<<8)|read_buf[1]);
}

float Med_Bh1750_GetLightIntensity (void)
{
	return (float)((bh1750_iic_read_byte() / 1.2f) * Resolurtion);
}

void get_LingtIntensity(void)
{
	float gLingtIntensity = Med_Bh1750_GetLightIntensity();
	unsigned char send_buf[60];
	sprintf((char*)send_buf,"Light:%.2f\r\n",gLingtIntensity);
	uart_send_str(send_buf);
    delay_ms(200);
}


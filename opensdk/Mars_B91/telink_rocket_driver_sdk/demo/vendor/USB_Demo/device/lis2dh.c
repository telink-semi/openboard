#include "../app_config.h"
#include "lis2dh.h"
#include "oled.h"
#include "serial_port.h"

axis_info_t g_axis_info;

uint8_t lis2dh12_iic_write_byte(uint8_t reg, uint8_t data)
{
	uint8_t write_buff[2] = {reg,data};
	return !i2c_master_write(LIS2DH_ADDR, write_buff, 2);
}

uint8_t lis2dh12_iic_read_byte(uint8_t reg)
{
	uint8_t read_data;
	i2c_master_write(LIS2DH_ADDR, &reg, 1);
	i2c_master_read(LIS2DH_ADDR, &read_data, 1);
	return read_data;
}

uint8_t Lis2dh_Init(void)
{
	lis2dh12_iic_write_byte(0x23, 0x08);    /* CTRL_REG4(23h): +/-2G��High resolution (12 bit data output) */
	lis2dh12_iic_write_byte(0x20, 0x77);	/* CTRL_REG1(20h): Normal mode  ODR 400HZ */
	return 0;
}

void get_acc_value(axis_info_t *sample)
{
	uint8_t reg = 0x28|0X80; // datasheet page.25
	uint8_t data[6];
	i2c_master_write(LIS2DH_ADDR, &reg, 1);
	i2c_master_read(LIS2DH_ADDR, data, 6);
	#define right_shift		0
	sample->x = (*(int16_t*)(data+0))>>right_shift;
	sample->y = (*(int16_t*)(data+2))>>right_shift;
	sample->z = (*(int16_t*)(data+4))>>right_shift;
 	char oled_show_buff[32];
	sprintf((char*)oled_show_buff,"x:%06d",sample->x);
	OLED_ShowStr(0,2,(unsigned char *)oled_show_buff);
	sprintf((char*)oled_show_buff,"y:%06d",sample->y);
	OLED_ShowStr(0,4,(unsigned char *)oled_show_buff);
	sprintf((char*)oled_show_buff,"z:%06d",sample->z);
	OLED_ShowStr(0,6,(unsigned char *)oled_show_buff);

	sprintf((char*)oled_show_buff,"x:%06d  y:%06d  z:%06d",sample->x,sample->y,sample->z);
	printf("%s\r\n",oled_show_buff);
}

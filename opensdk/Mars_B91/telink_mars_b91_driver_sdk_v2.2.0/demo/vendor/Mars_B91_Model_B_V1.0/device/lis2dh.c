#include "../app_config.h"
#include "lis2dh.h"

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
	lis2dh12_iic_write_byte(0x23, 0x08);    /* CTRL_REG4(23h): +/-2G High resolution (12 bit data output) */
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
	char send_buf[60];
	sprintf((char*)send_buf,"X:%06d	Y:%06d	Z:%06d\r\n",sample->x,sample->y,sample->z);
	uart_send_array(send_buf,strlen(send_buf));
}

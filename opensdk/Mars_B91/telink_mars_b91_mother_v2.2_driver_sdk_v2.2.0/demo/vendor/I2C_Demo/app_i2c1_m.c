/********************************************************************************************************
 * @file    app_i2c1_m.c
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
#include "app_config.h"
//For the i2c1_m module, there is only the master function.
#if(I2C_MASTER_WRITE_READ_MODE == I2C1_M_MASTER_WRITE_READ_NO_DMA)

#define     BUFF_DATA_LEN_NO_DMA			 16
#define     I2C1_M_CLK_SPEED				 100000

//for example:for b85m series chips, the address length is 3.
#define     SLAVE_DEVICE_ADDR			     0x48000
#define     SLAVE_DEVICE_ADDR_LEN		     3
//whether the master reads or writes, the slave address is treated as data.
typedef struct{
	unsigned char address[SLAVE_DEVICE_ADDR_LEN];
	unsigned char data[BUFF_DATA_LEN_NO_DMA];
} __attribute__((aligned(4))) i2c1_m_tx_b85m_slave_protocol_t;
i2c1_m_tx_b85m_slave_protocol_t i2c1_m_tx_buff ={
	.address = {0x04,0x80,0x00},
	.data = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff},
};
unsigned char i2c1_m_rx_buff[BUFF_DATA_LEN_NO_DMA] __attribute__((aligned(4)));

void user_init()
{
	gpio_function_en(LED1);
	gpio_output_en(LED1); 		//enable output
	gpio_input_dis(LED1);		//disable input
	gpio_function_en(LED2);
	gpio_output_en(LED2); 		//enable output
	gpio_input_dis(LED2);		//disable input
	gpio_function_en(LED3);
	gpio_output_en(LED3); 		//enable output
	gpio_input_dis(LED3);		//disable input
	gpio_function_en(LED4);
	gpio_output_en(LED4); 		//enable output
	gpio_input_dis(LED4);		//disable input
	i2c1_m_set_pin(I2C_GPIO_SDA_PIN,I2C_GPIO_SCL_PIN);
	i2c1_m_master_init();
	i2c1_m_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*I2C1_M_CLK_SPEED)));
}

void main_loop (void)
{
    delay_ms(10);
    i2c1_m_tx_buff.data[0]++;
    i2c1_m_master_send_stop(1);
	i2c1_m_master_write(0x5C,(unsigned char *)&i2c1_m_tx_buff, BUFF_DATA_LEN_NO_DMA+SLAVE_DEVICE_ADDR_LEN);
    delay_ms(10);
    i2c1_m_master_send_stop(0);
    i2c1_m_master_write_read(0x5C,(unsigned char *)(i2c1_m_tx_buff.address),SLAVE_DEVICE_ADDR_LEN,(unsigned char *)i2c1_m_rx_buff, BUFF_DATA_LEN_NO_DMA);
	gpio_toggle(LED1);

}

#endif

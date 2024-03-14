/********************************************************************************************************
 * @file    ext_codec_wm.c
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
#include "ext_codec_wm.h"
#define		WM8731_RESET_CTRL 					0x1e		//Reset Register
#define	    WM8731_ST_LINE_VOL                  0x00        //Set linmute volume
#define	    WM8731_ST_RINE_VOL                  0x02        //Set rinmute volume
#define		WM8731_ANA_AUDIO_PATH_CTRL			0x08		//Analogue Audio Path Control
#define		WM8731_DIG_AUDIO_PATH_CTRL			0x0a		//Digital Audio Path Control
#define		WM8731_POWER_DOWN_CTRL				0x0c		//Power Down Control
#define		WM8731_DIG_AUDIO_INTERFACE_FORMAT	0x0e		//Digital Audio Interface Format
#define		WM8731_SAMPLING_CTRL 				0x10		//Sampling Control
#define		WM8731_ACTIVE_CTRL 					0x12		//Active Control

unsigned char LineIn_To_I2S_CMD_TAB[9][2]={	{WM8731_RESET_CTRL, 				0x00},/*reset device*/
		                                    {WM8731_ST_LINE_VOL,			    (0x10<<0|0<<7)},/*0: LINVOL(max:0x1F);  0x7:LINMUTE */
		                                    {WM8731_ST_RINE_VOL,                (0x10<<0|0<<7)},/*0: RINVOL(max:0x1F);  0x7:LINMUTE */
											{WM8731_ANA_AUDIO_PATH_CTRL,        (1<<0 | 1<<1 | 0<<2 | 0<<3 | 1<<4)},/*0:MICBOOST; 1:MUTEMIC; 2:INSEL MIC; 3:BYPASS; 4:DACSEL*/
											{WM8731_DIG_AUDIO_PATH_CTRL,        (0<<3)},/*3:DACMUTE */
											{WM8731_POWER_DOWN_CTRL,            (0<<0 | 1<<1 | 0<<2 | 0<<3 | 0<<4 | 1<<5 | 0<<6 | 0<<7)},/*0:LINEINPD; 1:MICPD; 2:ADCPD; 3:DACPD; 4:OUTPD; 5:OSCPD; 6:CLKOUTPD; 7:POWEROFF*/
											{WM8731_DIG_AUDIO_INTERFACE_FORMAT, (2<<0 | 2<<2 | 0<<6)},/*0:I2S FORMAT; 2:DATA WDTH; 6:MASTER MODE*/
											{WM8731_SAMPLING_CTRL,              (1<<0 | 0<<1 | 6<<2 | 0<<6 | 0<<7)},/*0:USB MODE; 1:BOSR; 2:SR; 6:CLKDIV2; 7:CLKODIV2; SR:6--->32KHz,*/
											{WM8731_ACTIVE_CTRL,                0x01},/*activate device*/
};
/**
 * @brief     This function serves to config external codec for WM8731
 * @return    none
 */
void audio_set_ext_codec_wm(void)
{
	for (unsigned char i=0;i<9;i++)
	{
		i2c_master_write(0x34,&LineIn_To_I2S_CMD_TAB[i][0],2);
	}
}

/**
 * @brief      This function serves to set i2c master for codec i2c slave .
 * @param[in]  sda_pin - the pin port selected as I2C sda pin port.
 * @param[in]  scl_pin - the pin port selected as I2C scl pin port.
 * @param[in]  clock - the division factor of I2C clock,
 *             I2C frequency = System_clock / (4*DivClock).
 * @return     none.
 */
void audio_i2c_init_wm( gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin,unsigned char clock)
{
	i2c_master_init();
	i2c_set_master_clk(clock);
	i2c_set_pin(sda_pin,scl_pin);
	audio_set_ext_codec_wm();
}

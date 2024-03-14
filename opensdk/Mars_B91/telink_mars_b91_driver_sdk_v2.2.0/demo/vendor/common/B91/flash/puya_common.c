/********************************************************************************************************
 * @file    puya_common.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "puya_common.h"
extern audio_i2s_codec_config_t audio_i2s_codec_config;
extern audio_i2s_invert_config_t audio_i2s_invert_config;

#define	REBOOT		0
/**
 * @brief 		This function set GPIO_PF5 as gpio function.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void mspi_as_gpio(){

	gpio_set_high_level(GPIO_PF5);
	gpio_output_en(GPIO_PF5);
	gpio_function_en(GPIO_PF5);

}
/**
 * @brief 		This function set GPIO_PF5 as mspi function.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void mspi_as_mspi(){

	gpio_set_low_level(GPIO_PF5);
	gpio_output_dis(GPIO_PF5);
	gpio_function_dis(GPIO_PF5);
}
/**
 * @brief 		This function serves to enter flash test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_enter_test_mode(unsigned char *data)
{
	////////////enter test mode//////////////////////////
	mspi_fm_write_en();
	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(data[0]);
	mspi_wait();


	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(data[1]);
	mspi_wait();

	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(data[2]);
	mspi_wait();
	mspi_high();
}


/**
 * @brief 		This function serves to exit flash test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ void flash_exit_test_mode(void)
{
////////////////////////// Exit test mode //////////////////////////////
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(0x66);
	mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(0x99);
	mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_high();
}


/**
 * @brief 		This function serves to flash read in test mode.
 * @param[in]   cmd			- the read command.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_read_testmode(unsigned char cmd,unsigned int addr,unsigned long len, unsigned char *buf){
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(cmd);
	mspi_wait();
	mspi_write(addr>>16);
	mspi_wait();
	mspi_write(addr>>8);
	mspi_wait();
	mspi_write(addr);
	mspi_wait();

	mspi_write(0x00);			/* dummy,  to issue clock */
	mspi_wait();
	mspi_fm_rd_trig_en();			/* auto mode, mspi_get() automatically triggers mspi_write(0x00) once. */
	mspi_wait();
	/* get data */
	for(unsigned int i = 0; i < len; ++i){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_fm_rd_trig_dis();			/* off read auto mode */
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	CLOCK_DLY_5_CYC;
}

/**
 * @brief 		This function serves to flash write in test mode.
 * @param[in]   cmd			- the write command.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_  void flash_write_testmode(unsigned char cmd,unsigned int addr,unsigned long len, unsigned char *buf){
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(cmd);
	mspi_wait();

	mspi_write(addr>>16);
	mspi_wait();
	mspi_write(addr>>8);
	mspi_wait();
	mspi_write(addr);
	mspi_wait();

	for(unsigned int i = 0; i < len; ++i){
		mspi_write(buf[i]);			/* write data */
		mspi_wait();
	}
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
}



/**
 * @brief     This function serves to power off flash.
 * @return    none
 */
_attribute_ram_code_sec_noinline_ void flash_power_off_on_ram(unsigned int flash_off_time_ms)
{
	mspi_stop_xip();
#if REBOOT
#else
	unsigned char power_mode = analog_read_reg8(0x0a);
#endif
	//power off flash power supply. 1p4 change to DCDC mode. you can change it when power on.
	analog_write_reg8(0x0a, ((analog_read_reg8(0x0a) & 0xfc) | 0x02));

	analog_write_reg8(0x0b, (analog_read_reg8(0x0b) | 0x03));  //power down native 1.8V

	gpio_function_en(GPIO_PF0 | GPIO_PF1 | GPIO_PF2| GPIO_PF3| GPIO_PF4| GPIO_PF5);  //set mspi pin as gpio
	gpio_output_en(GPIO_PF0 | GPIO_PF1 | GPIO_PF2| GPIO_PF3| GPIO_PF4| GPIO_PF5);
	gpio_set_low_level(GPIO_PF0 | GPIO_PF1 | GPIO_PF2| GPIO_PF3| GPIO_PF4| GPIO_PF5);

	delay_ms(flash_off_time_ms);    // to wait flash power off done.

#if REBOOT
		sys_reboot();					//reboot will reset register(0x0a -> 0x90 ldo mode) to power on flash
		while(1);
#else
	gpio_output_dis(GPIO_PF0 | GPIO_PF1 | GPIO_PF2 | GPIO_PF3 | GPIO_PF4 | GPIO_PF5);    //set mspi pin as mspi function
	gpio_function_dis(GPIO_PF0 | GPIO_PF1 | GPIO_PF2 | GPIO_PF3 | GPIO_PF4 | GPIO_PF5);
	/*
	 * power on flash power supply:when power on the flash power(1.8V) will cause the 1V4 fluctuate,need to ensure the firmware is
	 * not run in here(add delay in here).
	 */
	analog_write_reg8(0x0a, power_mode);     //power mode restore.
	audio_set_codec_supply(CODEC_2P8V);
	delay_ms(1);
#endif
}

/**
 * @brief     This function serves to configuration the audio work on.
 * @param[in] flow_mode  - select input out flow mode
 * @param[in] rate       - audio sampling rate.
 * @param[in] channel_wl - word length and channel number.
 * @return    none
 * @note	  This function serve to initialize the audio module. copy from audio_init() function in audio.c file.
 * 			  but have some simplify to save running time. It is configure to make 1V8 power down more quickly.And
 * 			  it can not use as the normal audio init.
 */
void audio_config_on(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl)
{
	audio_set_chn_wl(channel_wl);
	audio_set_codec_clk(1,16);//from ppl 192/16=12M
	audio_mux_config(CODEC_I2S,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_out_mode);
	audio_i2s_config(I2S_I2S_MODE,audio_i2s_codec_config.i2s_data_select,audio_i2s_codec_config.i2s_codec_m_s_mode,&audio_i2s_invert_config);
	audio_set_i2s_clock(rate,AUDIO_RATE_EQUAL,0);
	audio_clk_en(1,1);
	reg_audio_codec_vic_ctr=FLD_AUDIO_CODEC_SLEEP_ANALOG;//active analog sleep mode
	while(!(reg_audio_codec_stat_ctr&FLD_AUDIO_CODEC_PON_ACK));//wait codec can be configured
	if(flow_mode<BUF_TO_LINE_OUT)
	{
		audio_codec_adc_config(audio_i2s_codec_config.i2s_codec_m_s_mode,(flow_mode%3),rate,audio_i2s_codec_config.codec_data_select,MCU_WREG);
	}
}


/**
 * @brief     This function serves to power off flash.
 * @return    none
 */
_attribute_text_sec_ void flash_power_off_on(unsigned int flash_off_time_ms)
{
	audio_set_codec_supply(CODEC_1P8V);    //set flash/audio power as 1.8V
	audio_config_on(LINE_IN_TO_BUF_TO_LINE_OUT,AUDIO_16K,MONO_BIT_16);  //audio on to take 1V8 decrease more quickly after power off.
	unsigned int r=core_interrupt_disable();
	__asm__("csrci 	mmisc_ctl,8");	//disable BTB
	flash_power_off_on_ram(flash_off_time_ms);
	__asm__("csrsi 	mmisc_ctl,8");	//enable BTB
	core_restore_interrupt(r);
}



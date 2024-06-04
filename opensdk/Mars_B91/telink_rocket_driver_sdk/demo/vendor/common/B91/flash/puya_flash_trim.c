/********************************************************************************************************
 * @file    puya_flash_trim.c
 *
 * @brief   This is the source file for B91
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "puya_flash_trim.h"
unsigned char enter_cmd_trim[3]={0xA8,0x8A,0x65};

/**
 * @brief 		This function serves to judge whether flash is in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static unsigned char flash_read_status_trim_testmode(void)
{
////////////////read status ///////////////////////////////
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(0x06);
	mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(0x05);
	mspi_wait();
    unsigned char status = mspi_read();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	return status;
}
/**
 * @brief 		This function serves to erase 0x1200 in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_erase_trim_testmode(void)
{
////////////////////// erase //////////////////////////////////////////////////////
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(0xD5);
	mspi_wait();
	mspi_write(0x00);
	mspi_wait();
	mspi_write(0x12);
	mspi_wait();
	mspi_write(0x00);
	mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_ms(25);
}

/**
 * @brief 		This function serves to read flash  in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_read_trim_testmode(unsigned int addr,unsigned long len, unsigned char *buf)
{
	flash_read_testmode(0xD7,addr,len,buf);
}
/**
 * @brief 		This function serves to write flash  in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_page_program_trim_testmode(unsigned int addr,unsigned long len, unsigned char *buf)
{
	flash_write_testmode(0xD6,addr,len,buf);
	delay_ms(5);
}
_attribute_ram_code_sec_noinline_ static unsigned char flash_trim_ram(void)
{
	unsigned char err_flag= 0;
	mspi_as_gpio();
	mspi_fm_data_line(MSPI_SINGLE_LINE);
	flash_enter_test_mode((unsigned char*)enter_cmd_trim);
	unsigned char status = flash_read_status_trim_testmode();
	if(status == 0x02)
	{
		unsigned char trim_buf[5]={0xEC, 0x9F, 0xEB, 0xFF ,0xCD} ;
		unsigned char read_buff[5]={0} ;
		flash_erase_trim_testmode();
		flash_page_program_trim_testmode(0x001200,5,trim_buf);
		flash_read_trim_testmode(0x001200,5,read_buff);
        for(int i=0;i<5;i++)
        {
        	if(trim_buf[i]!=read_buff[i])
        	{
        		err_flag=1;
        	}
        }
		flash_exit_test_mode();
		//recover GPIO_PF5
		mspi_as_mspi();
		delay_us(1);
		if(err_flag ==1){
			return 1;
		}
		else{
			return 0;
		}
	}
	//recover GPIO_PF5
	mspi_as_mspi();
	delay_us(1);
	return 2;
}
_attribute_ram_code_sec_noinline_ static unsigned char flash_trim_check_ram(void)
{
	unsigned char err_flag= 0;
	mspi_as_gpio();
	mspi_fm_data_line(MSPI_SINGLE_LINE);
	flash_enter_test_mode((unsigned char *)enter_cmd_trim);
	unsigned char status = flash_read_status_trim_testmode();
	if(status == 0x02)
	{
		unsigned char trim_buf[5]={0xEC, 0x9F, 0xEB, 0xFF ,0xCD} ;
		unsigned char read_buff[5]={0} ;
		flash_read_trim_testmode(0x001200,5,read_buff);
        for(int i=0;i<5;i++)
        {
        	if(trim_buf[i]!=read_buff[i])
        	{
        		err_flag=1;
        	}
        }
		flash_exit_test_mode();
		//recover GPIO_PF5
		mspi_as_mspi();
		delay_us(1);
		if(err_flag ==1){
			return 1;
		}
		else{
			return 0;
		}
	}
	//recover GPIO_PF5
	mspi_as_mspi();
	delay_us(1);
	return 2;
}


/**
 * @brief		This function serves to raise the flash reset voltage to 2.1V. If flash trim takes effect, the vbat voltage of chip can not be lower than 2.1V.
 * @return		0:OK; 1:fail to trim; 2: fail to enter test mode; 3:The chip mid is not 0x146085.
 * @note        Flash specification states that the default reset voltage of flash is 1.0V.
 *              Important description: The return value of this function is "0", which does not mean that flash trim is successful.Flash trim will take effect
 * 				after flash is powered off and powered on again.
 * 				Risk description:This function needs to use delay function and disable interrupt.The user must determine
 * 				how to use this function and better use it once,otherwise it may cause risks.Dont call this function anymore if flash trim takes effect.
 * 				Pay attention to the threshold of battery voltage detection and do not call this function in a low voltage state.
 * 				Flash cannot be powered off while calling this function.
 */
_attribute_text_sec_ unsigned char flash_trim(void)
{
	unsigned char ret =0x03;
	if(flash_read_mid() == 0x146085)
	{
		__asm__("csrci 	mmisc_ctl,8");	//disable BTB
		mspi_stop_xip();
		unsigned int r=core_interrupt_disable();
		ret = flash_trim_check_ram();
		if(ret==0)
		{
		   return ret;
		}
		ret = flash_trim_ram();
		core_restore_interrupt(r);
		__asm__("csrsi 	mmisc_ctl,8");	//enable BTB
		return ret;
	}
	return ret;
}
/**
 * @brief		This function serves to check whether flash trim is OK.
 * @return		0:OK; 1:fail to trim check; 2: fail to enter test mode; 3:The chip mid is not 0x146085.
 * @note        Risk description: This function needs to use delay function and disable interrupt.The user must determine
 * 				how to use this function,otherwise it may cause risks.
 * 				Attention description:Flash cannot be powered off while using this function.
 */
_attribute_text_sec_ unsigned char flash_trim_check(void)
{
	unsigned char ret =0x03;
	if(flash_read_mid() == 0x146085)
	{
		__asm__("csrci 	mmisc_ctl,8");	//disable BTB
		mspi_stop_xip();
		unsigned int r=core_interrupt_disable();
		ret = flash_trim_check_ram();
		core_restore_interrupt(r);
		__asm__("csrsi 	mmisc_ctl,8");	//enable BTB
		return ret;
	}
	return ret;
}


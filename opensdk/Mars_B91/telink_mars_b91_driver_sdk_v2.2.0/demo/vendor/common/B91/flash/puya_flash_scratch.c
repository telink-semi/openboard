/********************************************************************************************************
 * @file    puya_flash_scratch.c
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
#include "puya_flash_scratch.h"
unsigned char program_buff[256]={
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,
	0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,

	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
};
unsigned char read_buff[256]={0};
unsigned int plane_program_addr[16]={0x0000,0x0100,0x0200,0x0300,0x1000,0x1100,0x1200,0x1300,0x400,0x500,0x600,0x700,0x1400,0x1500,0x1600,0x1700};
unsigned int page_program_addr[8]={0x0000,0x0100,0x0200,0x0300,0x1000,0x1100,0x1200,0x1300};
unsigned char config_write_buff[4]={0x00,0x80,0x04,0x80};
unsigned int config_addr[4]={0x1300,0x1301,0x1302,0x1303};
unsigned char config_read_buff[4]={0};

unsigned char enter_cmd_scratch[3]={0x96,0x69,0xA8};

/**
 * @brief 		This function serves to flash write enable cmd in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_write_enable_cmd_testmode(){
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(FLASH_WRITE_ENABLE_CMD);
	mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
}

/**
 * @brief		This function servers to spi read.
 * @return		read result.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_read_testmode(void){
	mspi_write(0);		// dummy, issue clock
	mspi_wait();
	return reg_mspi_data;
}
/**
 * @brief		This function to determine whether the flash is busy..
 * @return		1:Indicates that the flash is busy. 0:Indicates that the flash is free
 */
_attribute_ram_code_sec_ static inline int flash_is_busy_testmode(void){
	return mspi_read_testmode() & 0x01;		//the busy bit, pls check flash spec
}
/**
 * @brief     This function serves to wait flash done in testmode.(make this a asynchronous version).
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ static void flash_wait_done_testomode(){
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
	mspi_write(FLASH_READ_STATUS_CMD_LOWBYTE);
	mspi_wait();
	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy_testmode()){
			break;
		}
	}
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
}
/**
 * @brief     This function serves to wait flash done.(make this a asynchronous version).
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ static void flash_wait_done_common(){
	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(FLASH_READ_STATUS_CMD_LOWBYTE);
	mspi_wait();
	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy_testmode()){
			break;
		}
	}
	mspi_high();
}

/**
 * @brief 		This function serves to flash erase chip in common mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_erase_chip_ram(void)
{
////////////////////// erase //////////////////////////////////////////////////////
	unsigned int r=core_interrupt_disable();
	mspi_stop_xip();
	mspi_fm_write_en();
	mspi_fm_data_line(MSPI_SINGLE_LINE);
	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(FLASH_WRITE_ENABLE_CMD);
	mspi_wait();


	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(0x60);
	mspi_wait();
	mspi_high();
	flash_wait_done_common();
	core_restore_interrupt(r);
}
///**
// * @brief 		This function serves to flash read config in test mode.
// * @param[in]   addr		- starting address.
// * @return 		none.
// */
//_attribute_ram_code_sec_noinline_ static unsigned char flash_read_config_scratch_testmode(unsigned int addr)
//{
//	unsigned char config_data=0;
//	flash_read_testmode(0xD9,addr,1,&config_data);
//	return config_data;
//}


/**
 * @brief 		This function serves to flash erase chip in test mode.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_erase_chip_scratch_testmode(void)
{
////////////////////// erase //////////////////////////////////////////////////////
	flash_write_enable_cmd_testmode();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	delay_us(1);
	BM_CLR(reg_gpio_out(GPIO_PF5), BIT(5));
    mspi_write(0x60);
    mspi_wait();
	BM_SET(reg_gpio_out(GPIO_PF5), BIT(5));
	flash_wait_done_testomode();
}




/**
 * @brief 		This function serves to flash write config in test mode.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_write_config_scratch_testmode(unsigned int addr,unsigned long len, unsigned char *buf){
	flash_write_testmode(0xD0,addr,len,buf);
	flash_wait_done_testomode();
}
/**
 * @brief 		This function serves to flash plane program in test mode.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_plane_program_scratch_testmode(unsigned int addr,unsigned long len, unsigned char *buf){
	flash_write_testmode(0xDA,addr,len,buf);
	flash_wait_done_testomode();
}
/**
 * @brief 		This function serves to flash page program in test mode.
 * @param[in]   addr		- starting address.
 * @param[in]   len	        - the length(in byte) of content needs to be written.
 * @param[out]  buf		    - the start address of the data buffer.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static void flash_page_program_scratch_testmode(unsigned int addr,unsigned long len, unsigned char *buf){
	flash_write_enable_cmd_testmode();
	flash_write_testmode(0x02,addr,len,buf);
	flash_wait_done_testomode();
}


/**
 * @brief 		This function serves to flash scratch test.
 * @return 		none.
 */
_attribute_ram_code_sec_noinline_ static unsigned char flash_scratch_ram(void)
{
	mspi_stop_xip();
	mspi_as_gpio();
	mspi_fm_data_line(MSPI_SINGLE_LINE);
	//enter test mode
	flash_enter_test_mode((unsigned char *)enter_cmd_scratch);
	//chip erase
	flash_erase_chip_scratch_testmode();
	//test mode program full chip
    for(unsigned char i=0;i<16;i++){
    	flash_plane_program_scratch_testmode(plane_program_addr[i],256,(unsigned char *)program_buff);
    }
    //set config
    for(unsigned char i=0;i<4;i++){
    	flash_write_config_scratch_testmode(config_addr[i],1,&(config_write_buff[i]));
    }
    //page program target memory address
    for(unsigned char i=0;i<8;i++){
    	flash_page_program_scratch_testmode(page_program_addr[i],256,(unsigned char*)program_buff);
    }
    //exit test mode
//  flash_exit_test_mode();
//  mspi_as_mspi();
//  delay_us(30);
		return 0;


}
/**
 * @brief 		This function serves to flash scratch test.
 * @return 		ret - return 1: The chip mid is not 0x156085.
 *                    return 0: test mode finish.
 */
_attribute_text_sec_ unsigned char flash_scratch(void)
{
	unsigned char ret =0x01;
	if(flash_read_mid()== 0x156085){
	    unsigned int r=core_interrupt_disable();
	    __asm__("csrci 	mmisc_ctl,8");	//disable BTB
	    ret=flash_scratch_ram();
	    __asm__("csrsi 	mmisc_ctl,8");	//enable BTB
	    core_restore_interrupt(r);
	    return ret;
	}
	return ret;
}

/**
 * @brief 		This function serves to flash scratch check.
 * @return 		ret - return 3: The chip mid is not 0x156085.
 *                    return 2: write err in testmode.
 *                    return 1: erase err in common.
 *                    return 0: ok.
 */
_attribute_text_sec_ unsigned char flash_check_scratch(void)
{
	unsigned char ret =0x03;
	if(flash_read_mid()== 0x156085){
		unsigned int flash_size=0;
		unsigned int flash_sector=0;
		unsigned int flash_page=0;
		flash_size = (flash_read_mid()&0xff0000)>>16;
		flash_sector = 16;
		for(unsigned int i=0;i<(flash_size-0x10);i++)
		{
			flash_sector = flash_sector*2;
		}
		flash_page = flash_sector*16;
		 for(unsigned int i=0;i<flash_page;i++){
			flash_dread(0x100*i, 256, (unsigned char *)read_buff);
			for(unsigned int j=0;j<256;j++){
				if(read_buff[j]!=program_buff[j]){
					return 1;
				}
			}
		}
		//erase chip
		flash_erase_chip();
		//read chip
		for(unsigned int i=0;i<flash_page;i++){
			flash_dread(0x100*i, 256, (unsigned char *)read_buff);
			for(unsigned int j=0;j<256;j++){
				if(read_buff[j]!=0xff){
					return 2;
				}
			}
		}
       return 0;
	}
	return ret;
}

/**
 * @brief 		This function serves to flash erase chip in common mode.
 * @return 		none.
 */
_attribute_text_sec_ void flash_erase_chip(void){
	__asm__("csrci 	mmisc_ctl,8");	//disable BTB
	flash_erase_chip_ram();
	__asm__("csrsi 	mmisc_ctl,8");	//enable BTB

}

/********************************************************************************************************
 * @file    app.c
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

#define FLASH_ADDR				0x00d000
#define FLASH_SECURITY_ADDR		0x001000
#define FLASH_BUFF_LEN			256

/*
 * @note    It is used to verify the write protection function of the security registers.
 *          The default value of FLASH_OTP_LOCK is 0 , which prevents the security registers from being write protected when the user verifies the functions,
 *          resulting in the inability to use the erase and write functions.
 *          (The driver will open this macro when verifying this function.)
 */
#define FLASH_OTP_LOCK			0

/*
 * @note    Because the flash enabled QE function is not compatible with each flash chip, when adding a new flash chip,
 *          the ram bin that enables QE needs to be stored in svn and used for factory enabling flash QE function of ATE
 *          and it also can be used for if the ATE does not receive the notification of enabling flash QE function,
 *          however it needs to be enabled by Lidong and the fixture department in the future.
 */
#define FLASH_QE_ENABLE         1

/*
 * @note    It is used to verify the read, erase and write functions, lock and unlock functions，read decrypt of and umid functions of Flash,
 *          and the read, erase and write functions of the security registers.
 *          You can judge which functions pass the test through structures err_status and check_status.
 */
#define FLASH_TEST              2
#define FLASH_FUNCTION_MODE     FLASH_TEST


volatile unsigned int  g_qe_status = 0;
volatile unsigned char Flash_Read_Buff[FLASH_BUFF_LEN]={0};
volatile unsigned char Flash_Write_Buff[FLASH_BUFF_LEN]=
{		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
		0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
		0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
		0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
		0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
		0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,


		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,
		0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
		0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,
		0x68,0x69,0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,
		0x78,0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,

		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
		0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,

		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,
		0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff,
};

typedef struct{
	unsigned char erase_err:1;
	unsigned char write_err:1;
	unsigned char lock_err:1;
	unsigned char unlock_err:1;
	unsigned char otp_erase_err:1;
	unsigned char otp_write_err:1;
	unsigned char otp_lock_err:1;
	unsigned char read_decrypt_check_err:1;
	unsigned char flash_vendor_add_err:1;
	unsigned char flash_lock_init_add_err:1;
	unsigned char flash_unlock_init_add_err:1;

}err_status_t;
typedef struct{
	unsigned char erase_check:1;
	unsigned char write_check:1;
	unsigned char lock_check:1;
	unsigned char unlock_check:1;
	unsigned char otp_erase_check:1;
	unsigned char otp_write_check:1;
	unsigned char otp_lock_check:1;
	unsigned char umid_check:1;
	unsigned char read_decrypt_check:1;
	unsigned char flash_vendor_add_check:1;
	unsigned char flash_lock_init_add_check:1;
	unsigned char flash_unlock_init_add_check:1;
}check_status_t;
volatile err_status_t err_status;
volatile check_status_t check_status;
volatile unsigned short status1=0;
volatile unsigned short status2=0;
volatile unsigned short status3=0;
volatile unsigned short status4=0;
volatile unsigned short status5=0;
volatile unsigned int  mid=0;
unsigned char uid[16]={0};

/*
 * the application has such requirements: in the flash demo, maintain two interface examples, flash_lock/flash_unlock, add all flash
 * (since there are multiple flash models for each chip, and flash may be added later, avoid missing).
 * flash_lock: judge whether the flash is locked, if the device is locked, it does not need to be locked again;
 * flash_unlock: judge whether the device is unlocked,if it is unlocked, there is no need to unlock it again;
 */
unsigned char flash_lock(unsigned int flash_mid){
	switch(flash_mid)
	{
	case  MID146085:
		if(0 == flash_get_lock_block_mid146085()){
			flash_lock_mid146085(FLASH_LOCK_LOW_64K_MID146085);
		}
		return 1;
		break;
	case MID156085:
		if(0 == flash_get_lock_block_mid156085()){
			flash_lock_mid156085(FLASH_LOCK_LOW_64K_MID156085);
		}
		return 1;
		break;
	case MID166085:
		if(0 == flash_get_lock_block_mid166085()){
			flash_lock_mid166085(FLASH_LOCK_LOW_64K_MID166085);
		}
	    return 1;
		break;
#if(MCU_CORE_B92)
	case MID1560c8:
		if(0 == flash_get_lock_block_mid1560c8()){
			flash_lock_mid1560c8(FLASH_LOCK_LOW_64K_MID1560c8);
		}
		return 1;
		break;
	case MID186085:
		if(0 == flash_get_lock_block_mid186085()){
			flash_lock_mid186085(FLASH_LOCK_LOW_256K_MID186085);
		}
		return 1;
		break;
#endif
	default:
		   return 0;
		break;
	}
}
unsigned char flash_unlock(unsigned int flash_mid){
	switch(flash_mid)
	{
	case MID146085:
		if(0 != flash_get_lock_block_mid146085()){
			flash_unlock_mid146085();
		}
		return 1;
		break;
	case MID156085:
		if(0 != flash_get_lock_block_mid156085()){
			flash_unlock_mid156085();
		}
		return 1;
		break;
	case MID166085:
		if(0 != flash_get_lock_block_mid166085()){
			flash_unlock_mid166085();
		}
		return 1;
		break;
#if(MCU_CORE_B92)
	case MID1560c8:
		if(0 != flash_get_lock_block_mid1560c8()){
			flash_unlock_mid1560c8();
		}
		return 1;
		break;
	case MID186085:
		if(0 != flash_get_lock_block_mid186085()){
			flash_unlock_mid186085();
		}
		return 1;
		break;
#endif
	default:
		   return 0;
		break;
	}
}



#if (MCU_CORE_B91 || MCU_CORE_B92)
void flash_mid146085_test(void)
{
	int i;

	status1 = flash_read_status_mid146085();
	flash_lock_mid146085(FLASH_LOCK_LOW_64K_MID146085);
	status2 = flash_read_status_mid146085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}
	check_status.lock_check = 1;

	flash_unlock_mid146085();
	status3 = flash_read_status_mid146085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	check_status.unlock_check = 1;

	flash_erase_otp_mid146085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid146085(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}
	check_status.otp_erase_check = 1;

	flash_write_otp_mid146085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid146085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}
	check_status.otp_write_check = 1;

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid146085();
	flash_lock_otp_mid146085(FLASH_LOCK_OTP_0x001000_512B_MID146085);
	status5 = flash_read_status_mid146085();
	flash_erase_otp_mid146085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid146085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
	check_status.otp_lock_check = 1;
#endif
}

void flash_mid156085_test(void)
{
	int i;

	status1 = flash_read_status_mid156085();
	flash_lock_mid156085(FLASH_LOCK_LOW_64K_MID156085);
	status2 = flash_read_status_mid156085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}
	check_status.lock_check = 1;

	flash_unlock_mid156085();
	status3 = flash_read_status_mid156085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	check_status.unlock_check = 1;

	flash_erase_otp_mid156085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid156085(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}
	check_status.otp_erase_check = 1;

	flash_write_otp_mid156085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid156085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}
	check_status.otp_write_check = 1;

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid156085();
	flash_lock_otp_mid156085(FLASH_LOCK_OTP_0x001000_1024B_MID156085);
	status5 = flash_read_status_mid156085();
	flash_erase_otp_mid156085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid156085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
	check_status.otp_lock_check = 1;
#endif
}
void flash_mid166085_test(void)
{
	int i;
	status1 = flash_read_status_mid166085();
	flash_lock_mid166085(FLASH_LOCK_LOW_64K_MID166085);
	status2 = flash_read_status_mid166085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}
	check_status.lock_check = 1;

	flash_unlock_mid166085();
	status3 = flash_read_status_mid166085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	check_status.unlock_check = 1;

	flash_erase_otp_mid166085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid166085(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}
	check_status.otp_erase_check = 1;

	flash_write_otp_mid166085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid166085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}
	check_status.otp_write_check = 1;

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid166085();
	flash_lock_otp_mid166085(FLASH_LOCK_OTP_0x001000_1024B_MID166085);
	status5 = flash_read_status_mid166085();
	flash_erase_otp_mid166085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid166085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
	check_status.otp_lock_check = 1;
#endif
}
#endif
#if(MCU_CORE_B92)
void flash_mid186085_test(void)
{
	int i;

	status1 = flash_read_status_mid186085();
	flash_lock_mid186085(FLASH_LOCK_LOW_256K_MID186085);
	status2 = flash_read_status_mid186085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}
	check_status.lock_check = 1;

	flash_unlock_mid186085();
	status3 = flash_read_status_mid186085();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	check_status.unlock_check = 1;

	flash_erase_otp_mid186085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid186085(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}
	check_status.otp_erase_check = 1;

	flash_write_otp_mid186085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid186085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}
	check_status.otp_write_check = 1;

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid186085();
	flash_lock_otp_mid186085(FLASH_LOCK_OTP_0x001000_1024B_MID186085);
	status5 = flash_read_status_mid186085();
	flash_erase_otp_mid186085(FLASH_SECURITY_ADDR);
	flash_read_otp_mid186085(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
	check_status.otp_lock_check = 1;
#endif
}

void flash_mid1560c8_test(void)
{
	int i;

	status1 = flash_read_status_mid1560c8();
	flash_lock_mid1560c8(FLASH_LOCK_LOW_64K_MID1560c8);
	status2 = flash_read_status_mid1560c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.lock_err = 1;
			while(1);
		}
	}
	check_status.lock_check = 1;

	flash_unlock_mid1560c8();
	status3 = flash_read_status_mid1560c8();
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.unlock_err = 1;
			while(1);
		}
	}
	check_status.unlock_check = 1;

	flash_erase_otp_mid1560c8(FLASH_SECURITY_ADDR);
	flash_read_otp_mid1560c8(FLASH_SECURITY_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.otp_erase_err = 1;
			while(1);
		}
	}
	check_status.otp_erase_check = 1;

	flash_write_otp_mid1560c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_otp_mid1560c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_write_err = 1;
			while(1);
		}
	}
	check_status.otp_write_check = 1;

#if FLASH_OTP_LOCK
	status4 = flash_read_status_mid1560c8();
	flash_lock_otp_mid1560c8(FLASH_LOCK_OTP_0x001000_1024B_MID1560c8);
	status5 = flash_read_status_mid1560c8();
	flash_erase_otp_mid1560c8(FLASH_SECURITY_ADDR);
	flash_read_otp_mid1560c8(FLASH_SECURITY_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.otp_lock_err = 1;
			while(1);
		}
	}
	check_status.otp_lock_check = 1;
#endif
}



#endif

void user_init()
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);

	delay_ms(1000);
	mid = flash_read_mid();
#if(FLASH_FUNCTION_MODE == FLASH_TEST)

	int i;
	flash_erase_sector(FLASH_ADDR);
	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != 0xff){
			err_status.erase_err = 1;
			while(1);
		}
	}
	check_status.erase_check = 1;

	flash_write_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_page(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);
	for(i=0; i<FLASH_BUFF_LEN; i++){
		if(Flash_Read_Buff[i] != Flash_Write_Buff[i]){
			err_status.write_err = 1;
			while(1);
		}
	}
	check_status.write_check = 1;

#if (MCU_CORE_B91)
	switch(mid)
	{
	case MID146085:
		flash_mid146085_test();
		break;
	case MID156085:
		flash_mid156085_test();
		break;
	case MID166085:
		flash_mid166085_test();
		break;
	default:
		break;
	}
#elif (MCU_CORE_B92)
	switch(mid)
	{
	case MID146085:
		flash_mid146085_test();
		break;
	case MID156085:
		flash_mid156085_test();
		break;
	case MID166085:
		flash_mid166085_test();
		break;
	case MID186085:
		flash_mid186085_test();
		break;
	case MID1560c8:
		flash_mid1560c8_test();
		break;
	default:
		break;
	}
	flash_erase_sector(FLASH_ADDR);
	flash_write_page_encrypt(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	unsigned char check_result = flash_read_page_decrypt_check(FLASH_ADDR+0x80,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	if(check_result == 0){
		check_status.read_decrypt_check = 1;
	}else{
		err_status.read_decrypt_check_err = 1;
	}
#endif
    check_status.umid_check = flash_read_mid_uid_with_check((unsigned int *)&mid, uid);
    if(flash_get_vendor(mid)){
	   check_status.flash_vendor_add_check =1;
    }else{
	   err_status.flash_vendor_add_err =1;
    }
    if(flash_lock(mid)){
    	check_status.flash_lock_init_add_check =1;
    }else{
    	err_status.flash_lock_init_add_err =1;
    }
    if(flash_unlock(mid)){
		check_status.flash_unlock_init_add_check =1;
	}else{
		err_status.flash_unlock_init_add_err =1;
	}
#endif


#if(FLASH_FUNCTION_MODE == FLASH_QE_ENABLE)
#if (MCU_CORE_B91)
	switch(mid)
	{
	case MID146085:
		flash_write_status_mid146085(0x200, FLASH_WRITE_STATUS_QE_MID146085);
		g_qe_status = flash_read_status_mid146085();
		break;
	case MID156085:
		flash_write_status_mid156085(0x200, FLASH_WRITE_STATUS_QE_MID156085);
		g_qe_status = flash_read_status_mid156085();
		break;
	case MID166085:
		flash_write_status_mid166085(0x200, FLASH_WRITE_STATUS_QE_MID166085);
		g_qe_status = flash_read_status_mid166085();
		break;
	default:
		break;
	}
#elif (MCU_CORE_B92)
	switch(mid)
	{
	case MID146085:
		flash_write_status_mid146085(0x200, FLASH_WRITE_STATUS_QE_MID146085);
		g_qe_status = flash_read_status_mid146085();
		break;
	case MID156085:
		flash_write_status_mid156085(0x200, FLASH_WRITE_STATUS_QE_MID156085);
		g_qe_status = flash_read_status_mid156085();
		break;
	case MID166085:
		flash_write_status_mid166085(0x200, FLASH_WRITE_STATUS_QE_MID166085);
		g_qe_status = flash_read_status_mid166085();
		break;
	case MID186085:
		flash_write_status_mid186085(0x200, FLASH_WRITE_STATUS_QE_MID186085);
		g_qe_status = flash_read_status_mid186085();
		break;
	case MID1560c8:
		flash_write_status_mid1560c8(0x200, FLASH_WRITE_STATUS_QE_MID1560c8);
		g_qe_status = flash_read_status_mid1560c8();
		break;
	default:
		break;
	}
#endif
	if(g_qe_status & BIT(9)){
		gpio_toggle(LED1);
	}
#endif
}

void main_loop (void)
{
	delay_ms(1000);
}


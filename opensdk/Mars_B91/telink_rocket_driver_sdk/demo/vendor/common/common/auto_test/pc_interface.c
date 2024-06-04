/********************************************************************************************************
 * @file    pc_interface.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "pc_interface.h"
#include "printf.h"

unsigned char command_buff[COMMAND_BUFF_CNT][COMMAND_BUFF_SIZE];
unsigned char result_buff[RESULT_BUFF_CNT][RESULT_BUFF_SIZE];
unsigned char para_buff[COMMAND_BUFF_SIZE];

volatile unsigned char par_addr[20]__attribute__((aligned(4)));

#if(MCU_CORE_B91 || MCU_CORE_B92)
/**
 * @brief      	This function is used to convert cpu address to bus address.
 * @param[in]  	addr - the addr need to be converted.
 * @return     	bus_addr
 */
unsigned int convert_addr2bus (unsigned int addr)
{
	unsigned int bus_addr = convert_ram_addr_cpu2bus((unsigned int)addr);
	bus_addr = (bus_addr & (~(BIT_RNG(17, 20))));
	return bus_addr;
}
#endif

/**
 * @brief      	This function is used to initiate the buffer of command.
 * @param[in]  	none
 * @return     	none
 */
void para_buff_init(void)
{
#if(MCU_CORE_B91 || MCU_CORE_B92)
	write_sram32(PARA_BUF_CHECK, convert_addr2bus((unsigned int)PARA_BUF_CHECK));
	write_sram32(PARA_BUF_ADDR, convert_addr2bus((unsigned int )command_buff));
#else
	write_sram32(PARA_BUF_ADDR, (unsigned int )command_buff);
	write_sram32(PARA_BUF_CHECK,(unsigned int)PARA_BUF_CHECK);
#endif
	write_sram8(PARA_BUF_SIZE, COMMAND_BUFF_SIZE);
	write_sram8(PARA_BUF_CNT, COMMAND_BUFF_CNT);
	write_sram8(PARA_BUF_WPTR, 0);
	write_sram8(PARA_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to get the command from the upper monitor
 * @param[in]  	none
 * @return     	none
 */
unsigned int para_buff_have_data(void)
{
    unsigned char wptr = read_sram8(PARA_BUF_WPTR);
    unsigned char rptr = read_sram8(PARA_BUF_RPTR);
    if (wptr == rptr) { // if buffer is empty, return 0
        return 0;
    }
    else {
    	return 1;
    }
}
/**
 * @brief      	This function is used to read command from the upper monitor.
 * @param[in]  	*dest_buf - save the command to the specified location waiting to be executed.
 * @param[in]  	len       - set the length of command waiting to read.
 * @return     	crc_flag  - 1(CRC correct)  -1(CRC error)
 */
int para_buff_read(unsigned char *dest_buf, unsigned char len)
{
    unsigned char rptr = read_sram8(PARA_BUF_RPTR);
    unsigned int  buf_addr = read_sram32(PARA_BUF_ADDR);
    unsigned char buf_size = read_sram8(PARA_BUF_SIZE);
    unsigned char buf_cnt = read_sram8(PARA_BUF_CNT);
    unsigned int  read_addr = buf_addr + buf_size*rptr;
    unsigned char i = 0;
    unsigned short crc;

    if(len > buf_size) len = buf_size;
    for (i = 0; i < len; i++) {
    	dest_buf[i] = read_sram8(read_addr+i);
     }
    autotest_package_t_ptr para = (autotest_package_t_ptr)dest_buf;
    write_sram8(PARA_BUF_RPTR, (rptr+1)%buf_cnt);
    if((para->payload_length+4) <= COMMAND_BUFF_SIZE){
    	crc = crc_16(dest_buf,para->payload_length);
    	if((para->crc[0] == (crc&0xff) && (para->crc[1] == ((crc>>8)&0xff)))) {
    		return 1;//CRC correct
    	}
    	else{
    		return -1;//CRC error
    	}
    }
    return 0;
}

/**
 * @brief      	This function is used to initiate the result buffer.
 * @param[in]  	none
 * @return     	none
 */
void result_buff_init(void)
{
#if(MCU_CORE_B91|MCU_CORE_B92)
	write_sram32(PARA_BASE_ADDR,convert_addr2bus((unsigned int)par_addr));
	write_sram32(RESU_BUF_ADDR, convert_addr2bus((unsigned int)result_buff));
#else
	write_sram32(PARA_BASE_ADDR,(unsigned int)par_addr);
	write_sram32(RESU_BUF_ADDR, (unsigned int)result_buff);
#endif
    write_sram8(RESU_BUF_SIZE, RESULT_BUFF_SIZE);
    write_sram8(RESU_BUF_CNT, RESULT_BUFF_CNT);
    write_sram8(RESU_BUF_WPTR, 0);
    write_sram8(RESU_BUF_RPTR, 0);
}
/**
 * @brief      	This function is used to clear the result buffer
 * @param[in]  	none
 * @return     	none
 */
void result_buff_clear(void)
{
    write_sram8(RESU_BUF_WPTR, 0);
    write_sram8(RESU_BUF_RPTR, 0);
}

/**
 * @brief     This function performs to calculate cyclic redundancy check code.
 * @param[in] data   - data.
 * @param[in] length - the length of data.
 * @return    crc    - calculate cyclic redundancy
 */
unsigned short crc_16(unsigned char *data,unsigned int length)
{
	unsigned int pos = 0;
    unsigned short crc = 0xFFFF;
    for (pos = 0; pos < length; pos++)
    {
        crc ^= (unsigned short)data[pos];
        for (int i = 8; i != 0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief   This function performs to write result and send command to automatic test tool.
 * @param[in] *data     - the data need to be sent.
 * @param[in] length    - the length of data.
 * @return    none
 */

int result_buff_write(unsigned char* data, unsigned char length)
{
	unsigned char wptr = read_sram8(RESU_BUF_WPTR);
	unsigned char rptr = read_sram8(RESU_BUF_RPTR);
	unsigned char buf_cnt = read_sram8(RESU_BUF_CNT);
	unsigned char result_buff_size = read_sram8(RESU_BUF_SIZE);
	unsigned char temp_buff[result_buff_size];
	if(length > (result_buff_size - 4)) return -1;

	autotest_package_t_ptr para = (autotest_package_t_ptr)temp_buff;
	memcpy(para->payload,data,length);
	para->payload_length = length;

	unsigned short crc16 =  crc_16(temp_buff,length);
	para->crc[0] = crc16&0xff;
	para->crc[1] = crc16>>8;

    length = RESULT_BUFF_SIZE;

    if (((wptr+1)%buf_cnt) == rptr) {
        return 0;
    }
    else {
        unsigned int buf_addr    = read_sram32(RESU_BUF_ADDR);
        unsigned char buf_size   = read_sram8(RESU_BUF_SIZE);
        unsigned int write_addr  = buf_addr + buf_size*wptr;
        unsigned char i = 0;
	    for (i = 0; i < length; i++) {
	    	write_sram8(write_addr+i, temp_buff[i]);
	    }
	    write_sram8(RESU_BUF_WPTR, (wptr+1)%buf_cnt);
        return i;
    }
}


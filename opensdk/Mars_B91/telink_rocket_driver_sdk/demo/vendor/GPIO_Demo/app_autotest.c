/********************************************************************************************************
 * @file    app_autotest.c
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
#include "app_config.h"

#if(DEMO_MODE == AUTO_TEST_MODE)

volatile unsigned int gpio_irq_cnt=0,gpio_irq_risc0_cnt=0,gpio_irq_risc1_cnt=0;

void gpio_input_test(void);
void gpio_output_low_test(void);
void gpio_output_high_test(void);
void gpio_digital_pull_up_test(void);
void gpio_analog_pull_up_1M_test(void);
void gpio_analog_pull_up_10K_test(void);
void gpio_analog_pull_down_100K_test(void);
void gpio_irq_test(void);
void gpio_irq_risc0_test(void);
void gpio_irq_risc1_test(void);
void gpio_irq_report_test(void);
#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
void gpio_irq_sel_test(void);
void gpio_irq_sel_report_test(void);
#endif

struct PC_CMD_FUNC{
	unsigned short cmd_name;
	void (*func)(void);
};

struct PC_CMD_FUNC pc_cmd_func[] = {
    {DUTCMD_GPIO_INPUT, gpio_input_test},
	{DUTCMD_GPIO_OUTPUT_LOW, gpio_output_low_test},
    {DUTCMD_GPIO_OUTPUT_HIGH, gpio_output_high_test},

    {DUTCMD_GPIO_DIGITAL_PULL_UP, gpio_digital_pull_up_test},
	{DUTCMD_GPIO_ANALOG_PULL_UP_1M, gpio_analog_pull_up_1M_test},
	{DUTCMD_GPIO_ANALOG_PULL_UP_10K, gpio_analog_pull_up_10K_test},
	{DUTCMD_GPIO_ANALOG_PULL_DOWN_100K, gpio_analog_pull_down_100K_test},

	{DUTCMD_GPIO_IRQ, gpio_irq_test},
	{DUTCMD_GPIO_IRQ_RISC0, gpio_irq_risc0_test},
	{DUTCMD_GPIO_IRQ_RISC1,	gpio_irq_risc1_test},
	{DUTCMD_GPIO_IRQ_REPORT, gpio_irq_report_test},

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
	{DUTCMD_GPIO_IRQ_SEL, gpio_irq_sel_test},
	{DUTCMD_GPIO_IRQ_SEL_REPORT, gpio_irq_sel_report_test},
#endif
};

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
	volatile unsigned int gpio_src_irq0_cnt=0;
	volatile unsigned int gpio_src_irq1_cnt=0;
	volatile unsigned int gpio_src_irq2_cnt=0;
	volatile unsigned int gpio_src_irq3_cnt=0;
	volatile unsigned int gpio_src_irq4_cnt=0;
	volatile unsigned int gpio_src_irq5_cnt=0;
	volatile unsigned int gpio_src_irq6_cnt=0;
	volatile unsigned int gpio_src_irq7_cnt=0;
#endif

_attribute_ram_code_sec_noinline_ void gpio_irq_handler(void)
{
	gpio_irq_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
}
PLIC_ISR_REGISTER(gpio_irq_handler, IRQ25_GPIO)

_attribute_ram_code_sec_noinline_ void gpio_risc0_irq_handler(void)
{
	gpio_irq_risc0_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
}
PLIC_ISR_REGISTER(gpio_risc0_irq_handler, IRQ26_GPIO2RISC0)

_attribute_ram_code_sec_noinline_ void gpio_risc1_irq_handler(void)
{
	gpio_irq_risc1_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
}
PLIC_ISR_REGISTER(gpio_risc1_irq_handler, IRQ27_GPIO2RISC1)

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
_attribute_ram_code_sec_noinline_ void gpio_src0_irq_handler(void)
{
	gpio_src_irq0_cnt++;
 	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ0);
}
PLIC_ISR_REGISTER(gpio_src0_irq_handler, IRQ34_GPIO_SRC0)

_attribute_ram_code_sec_noinline_ void gpio_src1_irq_handler(void)
{
	gpio_src_irq1_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ1);
}
PLIC_ISR_REGISTER(gpio_src1_irq_handler, IRQ35_GPIO_SRC1)

_attribute_ram_code_sec_noinline_ void gpio_src2_irq_handler(void)
{
	gpio_src_irq2_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ2);
}
PLIC_ISR_REGISTER(gpio_src2_irq_handler, IRQ36_GPIO_SRC2)

_attribute_ram_code_sec_noinline_ void gpio_src3_irq_handler(void)
{
	gpio_src_irq3_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ3);
}
PLIC_ISR_REGISTER(gpio_src3_irq_handler, IRQ37_GPIO_SRC3)

_attribute_ram_code_sec_noinline_ void gpio_src4_irq_handler(void)
{
	gpio_src_irq4_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ4);
}
PLIC_ISR_REGISTER(gpio_src4_irq_handler, IRQ38_GPIO_SRC4)

_attribute_ram_code_sec_noinline_ void gpio_src5_irq_handler(void)
{
	gpio_src_irq5_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ5);
}
PLIC_ISR_REGISTER(gpio_src5_irq_handler, IRQ39_GPIO_SRC5)

_attribute_ram_code_sec_noinline_ void gpio_src6_irq_handler(void)
{
	gpio_src_irq6_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ6);
}
PLIC_ISR_REGISTER(gpio_src6_irq_handler, IRQ40_GPIO_SRC6)

_attribute_ram_code_sec_noinline_ void gpio_src7_irq_handler(void)
{
	gpio_src_irq7_cnt++;
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ7);
}
PLIC_ISR_REGISTER(gpio_src7_irq_handler, IRQ41_GPIO_SRC7)
#endif

void gpio_input_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_output_dis(pin);
	gpio_input_en(pin);
	unsigned char pin_level = gpio_get_level(pin);
	unsigned char send_data[4];
	send_data[0] = DUTCMD_GPIO_INPUT&0xff;
	send_data[1] = (DUTCMD_GPIO_INPUT>>8)&0xff;
	send_data[2] = 4;
	send_data[3] = pin_level;
	result_buff_write(send_data, send_data[2]);
}

void gpio_output_low_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_output_en(pin);
	gpio_input_dis(pin);
	gpio_set_low_level(pin);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_OUTPUT_LOW&0xff;
	send_data[1] = (DUTCMD_GPIO_OUTPUT_LOW>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
}

void gpio_output_high_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_output_en(pin);
	gpio_input_dis(pin);
	gpio_set_high_level(pin);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_OUTPUT_HIGH&0xff;
	send_data[1] = (DUTCMD_GPIO_OUTPUT_HIGH>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
}

void gpio_digital_pull_up_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
#if(MCU_CORE_B91|MCU_CORE_B92)
	gpio_set_pullup_res_30k(pin);
#else
	gpio_set_digital_pullup(pin);
#endif
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_DIGITAL_PULL_UP&0xff;
	send_data[1] = (DUTCMD_GPIO_DIGITAL_PULL_UP>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
}

void gpio_analog_pull_up_1M_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_1M);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_ANALOG_PULL_UP_1M&0xff;
	send_data[1] = (DUTCMD_GPIO_ANALOG_PULL_UP_1M>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);

}

void gpio_analog_pull_up_10K_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_ANALOG_PULL_UP_10K&0xff;
	send_data[1] = (DUTCMD_GPIO_ANALOG_PULL_UP_10K>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
}

void gpio_analog_pull_down_100K_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	gpio_function_en(pin);
	gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_ANALOG_PULL_DOWN_100K&0xff;
	send_data[1] = (DUTCMD_GPIO_ANALOG_PULL_DOWN_100K>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
}

void gpio_irq_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	unsigned char value = para->payload[4];
	gpio_function_en(pin);
	gpio_output_dis(pin);
	gpio_input_en(pin);
	if(0 == value)
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
		gpio_set_irq(pin, INTR_RISING_EDGE);
	}
	else
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
		gpio_set_irq(pin, INTR_FALLING_EDGE);
	}
	core_interrupt_enable();
	plic_interrupt_enable(IRQ25_GPIO);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_IRQ&0xff;
	send_data[1] = (DUTCMD_GPIO_IRQ>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
	gpio_irq_cnt = 0;
}

void gpio_irq_risc0_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	unsigned char value = para->payload[4];
	gpio_function_en(pin);
	gpio_output_dis(pin);
	gpio_input_en(pin);
	if(0 == value)
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
		gpio_set_gpio2risc0_irq(pin, INTR_RISING_EDGE);
	}
	else
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
		gpio_set_gpio2risc0_irq(pin, INTR_FALLING_EDGE);
	}
	core_interrupt_enable();
	plic_interrupt_enable(IRQ26_GPIO2RISC0);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_IRQ_RISC0&0xff;
	send_data[1] = (DUTCMD_GPIO_IRQ_RISC0>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
	gpio_irq_risc0_cnt = 0;
}

void gpio_irq_risc1_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	unsigned char value = para->payload[4];
	gpio_function_en(pin);
	gpio_output_dis(pin);
	gpio_input_en(pin);
	if(0 == value)
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
		gpio_set_gpio2risc1_irq(pin, INTR_RISING_EDGE);
	}
	else
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
		gpio_set_gpio2risc1_irq(pin, INTR_FALLING_EDGE);
	}
	core_interrupt_enable();
	plic_interrupt_enable(IRQ27_GPIO2RISC1);
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_IRQ_RISC1&0xff;
	send_data[1] = (DUTCMD_GPIO_IRQ_RISC1>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
	gpio_irq_risc1_cnt = 0;
}

void gpio_irq_report_test(void)
{
	unsigned char send_data[6];
	send_data[0] =  (DUTCMD_GPIO_IRQ_REPORT>>8)&0xff;
	send_data[1] =  DUTCMD_GPIO_IRQ_REPORT&0xff;
	send_data[2] =  6;
	send_data[3] =  gpio_irq_cnt;
	send_data[4] =  gpio_irq_risc0_cnt;
	send_data[5] =  gpio_irq_risc1_cnt;
	result_buff_write(send_data, send_data[2]);
}

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
void gpio_irq_sel_test(void)
{
	autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
	gpio_pin_e pin = (para->payload[2]<< 8 | para->payload[3]) & 0xffff;
	unsigned char value = para->payload[4];
	gpio_function_en(pin);
	gpio_output_dis(pin);
	gpio_input_en(pin);
	if(0 == value)
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLDOWN_100K);
		gpio_set_src_irq(pin,INTR_RISING_EDGE);
	}
	else
	{
		gpio_set_up_down_res(pin, GPIO_PIN_PULLUP_10K);
		gpio_set_src_irq(pin,INTR_FALLING_EDGE);
	}
	core_interrupt_enable();
	gpio_set_src_irq_group(pin>>8);
    plic_interrupt_enable(IRQ34_GPIO_SRC0 + BIT_LOW_BIT(pin&0xff));
	unsigned char send_data[3];
	send_data[0] = DUTCMD_GPIO_IRQ_SEL&0xff;
	send_data[1] = (DUTCMD_GPIO_IRQ_SEL>>8)&0xff;
	send_data[2] = 2;
	result_buff_write(send_data,send_data[2]);
	gpio_src_irq0_cnt = 0;
	gpio_src_irq1_cnt = 0;
	gpio_src_irq2_cnt = 0;
	gpio_src_irq3_cnt = 0;
	gpio_src_irq4_cnt = 0;
	gpio_src_irq5_cnt = 0;
	gpio_src_irq6_cnt = 0;
	gpio_src_irq7_cnt = 0;
}

void gpio_irq_sel_report_test(void)
{
	unsigned char send_data[11];
	send_data[0]  =  (DUTCMD_GPIO_IRQ_SEL_REPORT>>8)&0xff;
	send_data[1]  =  DUTCMD_GPIO_IRQ_SEL_REPORT&0xff;
	send_data[2]  =  11;
	send_data[3]  =  gpio_src_irq0_cnt;
	send_data[4]  =  gpio_src_irq1_cnt;
	send_data[5]  =  gpio_src_irq2_cnt;
	send_data[6]  =  gpio_src_irq3_cnt;
	send_data[7]  =  gpio_src_irq4_cnt;
	send_data[8]  =  gpio_src_irq5_cnt;
	send_data[9]  =  gpio_src_irq6_cnt;
	send_data[10] =  gpio_src_irq7_cnt;
	result_buff_write(send_data,send_data[2]);
}
#endif


void user_init(void)
{
	// initiate parameter buffer to receive CMD and parameter
	para_buff_init();
	// initiate result buffer to send the result
	result_buff_init();
}

volatile unsigned int correct_cmd_count = 0;
volatile unsigned int cmd_count = 0;

void pc_data_handle(void)
{
	unsigned char  i;
	int crc_flag;
	if(para_buff_have_data() != 0){
		cmd_count++;
		crc_flag = para_buff_read(para_buff,COMMAND_BUFF_SIZE); // 1:CRC correct  -1:CRC error  0:buffer size overflow
		autotest_package_t_ptr para = (autotest_package_t_ptr)para_buff;
		unsigned int test_cmd = (para->payload[1]<<8 | para->payload[0]);
		if(crc_flag == 1){
			for(i = 0; i < sizeof(pc_cmd_func)/sizeof(struct PC_CMD_FUNC); i++) {
				if(pc_cmd_func[i].cmd_name == test_cmd) {
					correct_cmd_count++;
					pc_cmd_func[i].func();
					break;
				}
			}
		}
	}
}
/*********************************************************************************************************
 *												main loop
 *********************************************************************************************************/
void main_loop(void)
{
	while(1){
		pc_data_handle();
	}
}

/*
 * MANUAL_TEST_MODE only for internal testing when AUTO_TEST_MODE is inconvenient.
 * Currently, USB PINS or SWS PINS that communicate with Tscript can only be tested under MANUAL_TEST_MODE
 */
#elif(DEMO_MODE == MANUAL_TEST_MODE)

volatile unsigned int gpio_irq_cnt = 0;
volatile unsigned int gpio_irq_risc0_cnt = 0;
volatile unsigned int gpio_irq_risc1_cnt = 0;

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
volatile unsigned int gpio_src_irq0_cnt = 0;
volatile unsigned int gpio_src_irq1_cnt = 0;
volatile unsigned int gpio_src_irq2_cnt = 0;
volatile unsigned int gpio_src_irq3_cnt = 0;
volatile unsigned int gpio_src_irq4_cnt = 0;
volatile unsigned int gpio_src_irq5_cnt = 0;
volatile unsigned int gpio_src_irq6_cnt = 0;
volatile unsigned int gpio_src_irq7_cnt = 0;
#endif

#define SWS_PIN			 		GPIO_PA7
#define LED						GPIO_PD1

gpio_pin_e g_led_irq = LED;// sws input and interrupt test

gpio_pin_e GPIO_PIN[] = {
#if 1
		GPIO_PA0,GPIO_PB0,GPIO_PC5,GPIO_PC6,GPIO_PC7,GPIO_PF0,GPIO_PF5
#else
	GPIO_PA0,GPIO_PA1,GPIO_PA2,GPIO_PA3,GPIO_PA4,GPIO_PA5,GPIO_PA6,GPIO_PA7,
	GPIO_PB0,GPIO_PB1,GPIO_PB2,GPIO_PB3,GPIO_PB4,GPIO_PB5,GPIO_PB6,GPIO_PB7,
	GPIO_PC0,GPIO_PC1,GPIO_PC2,GPIO_PC3,GPIO_PC4,GPIO_PC5,GPIO_PC6,GPIO_PC7,
	GPIO_PD0,GPIO_PD1,GPIO_PD2,GPIO_PD3,GPIO_PD4,GPIO_PD5,GPIO_PD6,GPIO_PD7,
	GPIO_PE0,GPIO_PE1,GPIO_PE2,GPIO_PE3,GPIO_PE4,GPIO_PE5,GPIO_PE6,GPIO_PE7,
	GPIO_PF0,GPIO_PF1,GPIO_PF2,GPIO_PF3,GPIO_PF4,GPIO_PF5,GPIO_PF6,GPIO_PF7,
	GPIO_PG0,GPIO_PG1,GPIO_PG2,GPIO_PG3,GPIO_PG4,GPIO_PG5,GPIO_PG6,GPIO_PG7,
	GPIO_PH0,GPIO_PH1,GPIO_PH2,GPIO_PH3,GPIO_PH4,GPIO_PH5,GPIO_PH6,
	GPIO_ANA0,GPIO_ANA1
#endif
};

#define GPIO_SRC 				0
gpio_pin_e GPIO_SRC_PIN[] = {
#if(GPIO_SRC == 0 || GPIO_SRC == 0xff)
	GPIO_PA0,GPIO_PA1,GPIO_PA2,GPIO_PA3,GPIO_PA4,GPIO_PA5,GPIO_PA6,
	GPIO_PA7,
#endif

#if(GPIO_SRC == 1 || GPIO_SRC == 0xff)
	GPIO_PB0,GPIO_PB1,GPIO_PB2,GPIO_PB3,GPIO_PB4,GPIO_PB5,GPIO_PB6,GPIO_PB7,
#endif

#if(GPIO_SRC == 2 || GPIO_SRC == 0xff)
	GPIO_PC0,GPIO_PC1,GPIO_PC2,GPIO_PC3,GPIO_PC4,GPIO_PC5,GPIO_PC6,GPIO_PC7,
#endif

#if(GPIO_SRC == 3 || GPIO_SRC == 0xff)
	GPIO_PD0,
	GPIO_PD1,GPIO_PD2,GPIO_PD3,GPIO_PD4,GPIO_PD5,GPIO_PD6,GPIO_PD7,
#endif

#if(GPIO_SRC == 4 || GPIO_SRC == 0xff)
	GPIO_PE0,GPIO_PE1,GPIO_PE2,GPIO_PE3,GPIO_PE4,GPIO_PE5,GPIO_PE6,GPIO_PE7,
#endif

#if(GPIO_SRC == 5 || GPIO_SRC == 0xff)
	GPIO_PF0,GPIO_PF1,GPIO_PF2,GPIO_PF3,GPIO_PF4,GPIO_PF5,GPIO_PF6,GPIO_PF7
#endif

#if(GPIO_SRC == 6 || GPIO_SRC == 0xff)
	GPIO_PG0,GPIO_PG1,GPIO_PG2,GPIO_PG3,GPIO_PG4,GPIO_PG5,GPIO_PG6,GPIO_PG7
#endif

#if(GPIO_SRC == 7 || GPIO_SRC == 0xff)
	GPIO_PH0,GPIO_PH1,GPIO_PH2,GPIO_PH3,GPIO_PH4,GPIO_PH5,GPIO_PH6
#endif
};

typedef enum{
	test_item_output_low = 0,
	test_item_output_high = 1,
	test_item_output_toggle,
	test_item_input_low,
	test_item_input_high,
	test_item_input_dropdown_sws,
	test_item_input_pullup_sws,

	test_item_analog_drop_down_100k,
	test_item_analog_pull_up_10k,
	test_item_analog_pull_up_1M,
	test_item_digital_pull_up,

	test_item_irq_rising_edge,
	test_item_irq_falling_edge,

	test_item_irq_risc0_rising_edge,
	test_item_irq_risc0_falling_edge,

	test_item_irq_risc1_rising_edge,
	test_item_irq_risc1_falling_edge,

	test_item_sel_irq_src_rising_edge,
	test_item_sel_irq_src_falling_edge,
}test_item_e;

const test_item_e g_test_item_e = test_item_analog_pull_up_1M;

_attribute_ram_code_sec_noinline_ void gpio_irq_handler(void)
{
	gpio_toggle(g_led_irq);
	gpio_irq_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_CLR);
}
PLIC_ISR_REGISTER(gpio_irq_handler, IRQ25_GPIO)

_attribute_ram_code_sec_noinline_ void gpio_risc0_irq_handler(void)
{
	gpio_toggle(g_led_irq);
	gpio_irq_risc0_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC0_CLR);
}
PLIC_ISR_REGISTER(gpio_risc0_irq_handler, IRQ26_GPIO2RISC0)

_attribute_ram_code_sec_noinline_ void gpio_risc1_irq_handler(void)
{
	gpio_toggle(g_led_irq);
	gpio_irq_risc1_cnt++;
	gpio_clr_irq_status(FLD_GPIO_IRQ_GPIO2RISC1_CLR);
}
PLIC_ISR_REGISTER(gpio_risc1_irq_handler, IRQ27_GPIO2RISC1)

#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
_attribute_ram_code_sec_noinline_ void gpio_src0_irq_handler(void)
{
	gpio_src_irq0_cnt++;
	gpio_toggle(g_led_irq);
 	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ0);
}
PLIC_ISR_REGISTER(gpio_src0_irq_handler, IRQ34_GPIO_SRC0)

_attribute_ram_code_sec_noinline_ void gpio_src1_irq_handler(void)
{
	gpio_src_irq1_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ1);
}
PLIC_ISR_REGISTER(gpio_src1_irq_handler, IRQ35_GPIO_SRC1)

_attribute_ram_code_sec_noinline_ void gpio_src2_irq_handler(void)
{
	gpio_src_irq2_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ2);
}
PLIC_ISR_REGISTER(gpio_src2_irq_handler, IRQ36_GPIO_SRC2)

_attribute_ram_code_sec_noinline_ void gpio_src3_irq_handler(void)
{
	gpio_src_irq3_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ3);
}
PLIC_ISR_REGISTER(gpio_src3_irq_handler, IRQ37_GPIO_SRC3)

_attribute_ram_code_sec_noinline_ void gpio_src4_irq_handler(void)
{
	gpio_src_irq4_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ4);
}
PLIC_ISR_REGISTER(gpio_src4_irq_handler, IRQ38_GPIO_SRC4)

_attribute_ram_code_sec_noinline_ void gpio_src5_irq_handler(void)
{
	gpio_src_irq5_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ5);
}
PLIC_ISR_REGISTER(gpio_src5_irq_handler, IRQ39_GPIO_SRC5)

_attribute_ram_code_sec_noinline_ void gpio_src6_irq_handler(void)
{
	gpio_src_irq6_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ6);
}
PLIC_ISR_REGISTER(gpio_src6_irq_handler, IRQ40_GPIO_SRC6)

_attribute_ram_code_sec_noinline_ void gpio_src7_irq_handler(void)
{
	gpio_src_irq7_cnt++;
	gpio_toggle(g_led_irq);
	gpio_clr_group_irq_status(FLD_GPIO_GROUP_IRQ7);
}
PLIC_ISR_REGISTER(gpio_src7_irq_handler, IRQ41_GPIO_SRC7)
#endif

void user_init(void)
{
	delay_ms(1000);
	switch((unsigned char)(*(&g_test_item_e))){
	 	case test_item_output_low:
	 	case test_item_output_toggle:
	 		for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
	 			gpio_function_en(GPIO_PIN[i]);
	 			gpio_output_en(GPIO_PIN[i]);
	 			gpio_input_dis(GPIO_PIN[i]);
	 			gpio_set_low_level(GPIO_PIN[i]);
	 		}
	 	break;
	 	case test_item_output_high:
	 		for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
	 			gpio_function_en(GPIO_PIN[i]);
	 			gpio_output_en(GPIO_PIN[i]);
	 			gpio_input_dis(GPIO_PIN[i]);
	 			gpio_set_high_level(GPIO_PIN[i]);
	 		}
	 	break;
	 	case test_item_input_low:
	 		for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
	 			if(GPIO_PIN[i] != SWS_PIN){
					gpio_function_en(GPIO_PIN[i]);
					gpio_output_dis(GPIO_PIN[i]);
					gpio_input_en(GPIO_PIN[i]);
	 			}
	 		}
	 	break;
	 	case test_item_input_high:
	 		for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
	 			if(GPIO_PIN[i] != SWS_PIN){
					gpio_function_en(GPIO_PIN[i]);
					gpio_output_dis(GPIO_PIN[i]);
					gpio_input_en(GPIO_PIN[i]);
	 			}
	 		}
	 	break;
	 	case test_item_input_dropdown_sws:
				gpio_function_en(SWS_PIN);
				gpio_output_dis(SWS_PIN);
				gpio_input_en(SWS_PIN);
				gpio_set_up_down_res(SWS_PIN, GPIO_PIN_PULLDOWN_100K);
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);
	 	break;
	 	case test_item_input_pullup_sws:
				gpio_function_en(SWS_PIN);
				gpio_output_dis(SWS_PIN);
				gpio_input_en(SWS_PIN);
				gpio_set_up_down_res(SWS_PIN, GPIO_PIN_PULLUP_10K);
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);
	 	break;
        case test_item_analog_drop_down_100k:
			 for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
				gpio_function_en(GPIO_PIN[i]);
				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLDOWN_100K);
			 }
		break;
        case test_item_analog_pull_up_10k:
			 for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
				gpio_function_en(GPIO_PIN[i]);
				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLUP_10K);
			 }
		break;
        case test_item_analog_pull_up_1M:
			 for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
				gpio_function_en(GPIO_PIN[i]);
				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLUP_1M);
			 }
		break;
        case test_item_digital_pull_up:
			 for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
				gpio_function_en(GPIO_PIN[i]);
				#if(MCU_CORE_B91|MCU_CORE_B92)
					gpio_set_pullup_res_30k(GPIO_PIN[i]);
				#else
					gpio_set_digital_pullup(GPIO_PIN[i]);
				#endif
			 }
		break;
        case test_item_irq_rising_edge:
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);

 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 		 				gpio_function_en(GPIO_PIN[i]);
 		 				gpio_output_dis(GPIO_PIN[i]);
 		 				gpio_input_en(GPIO_PIN[i]);
 		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLDOWN_100K);
 					}
 				}
				core_interrupt_enable();
 				plic_interrupt_enable(IRQ25_GPIO);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					gpio_set_irq(GPIO_PIN[i], INTR_RISING_EDGE);
 				}
 		        gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO);

        break;
        case test_item_irq_falling_edge:
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 		 				gpio_function_en(GPIO_PIN[i]);
 		 				gpio_output_dis(GPIO_PIN[i]);
 		 				gpio_input_en(GPIO_PIN[i]);
 		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLUP_10K);
 					}
 				}
				core_interrupt_enable();
 				plic_interrupt_enable(IRQ25_GPIO);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					gpio_set_irq(GPIO_PIN[i], INTR_FALLING_EDGE);
 				}
 		        gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO);

        break;
        case test_item_irq_risc0_rising_edge:
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 		 				gpio_function_en(GPIO_PIN[i]);
 		 				gpio_output_dis(GPIO_PIN[i]);
 		 				gpio_input_en(GPIO_PIN[i]);
		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLDOWN_100K);
 					}
 				}
 				core_interrupt_enable();
 				plic_interrupt_enable(IRQ26_GPIO2RISC0);

 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 		 				gpio_set_gpio2risc0_irq(GPIO_PIN[i], INTR_RISING_EDGE);
 					}
 				}
 				gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC0);
        break;
        case test_item_irq_risc0_falling_edge:
 				gpio_function_en(g_led_irq);
 				gpio_output_en(g_led_irq);
 				gpio_set_low_level(g_led_irq);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 		 				gpio_function_en(GPIO_PIN[i]);
 		 				gpio_output_dis(GPIO_PIN[i]);
 		 				gpio_input_en(GPIO_PIN[i]);
		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLUP_10K);
 					}
 				}
 				core_interrupt_enable();
 				plic_interrupt_enable(IRQ26_GPIO2RISC0);
 				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
 					if(GPIO_PIN[i] != g_led_irq){
 						gpio_set_gpio2risc0_irq(GPIO_PIN[i], INTR_FALLING_EDGE);
 					}
 				 }
 				gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC0);
        break;
        case test_item_irq_risc1_rising_edge:
				gpio_function_en(g_led_irq);
				gpio_output_en(g_led_irq);
				gpio_set_low_level(g_led_irq);
				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
					if(GPIO_PIN[i] != g_led_irq){
		 				gpio_function_en(GPIO_PIN[i]);
		 				gpio_output_dis(GPIO_PIN[i]);
		 				gpio_input_en(GPIO_PIN[i]);
		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLDOWN_100K);
					}
				}
				core_interrupt_enable();
				plic_interrupt_enable(IRQ27_GPIO2RISC1);

				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
					if(GPIO_PIN[i] != g_led_irq){
						gpio_set_gpio2risc1_irq(GPIO_PIN[i], INTR_RISING_EDGE);
					}
				}
				gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC1);
        break;
        case test_item_irq_risc1_falling_edge:
				gpio_function_en(g_led_irq);
				gpio_output_en(g_led_irq);
				gpio_set_low_level(g_led_irq);
				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
					if(GPIO_PIN[i] != g_led_irq){
		 				gpio_function_en(GPIO_PIN[i]);
		 				gpio_output_dis(GPIO_PIN[i]);
		 				gpio_input_en(GPIO_PIN[i]);
		 				gpio_set_up_down_res(GPIO_PIN[i], GPIO_PIN_PULLUP_10K);
					}
				}
				core_interrupt_enable();
				plic_interrupt_enable(IRQ27_GPIO2RISC1);
				for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
					if(GPIO_PIN[i] != g_led_irq){
						gpio_set_gpio2risc1_irq(GPIO_PIN[i], INTR_FALLING_EDGE);
					}
				 }
				gpio_set_irq_mask(GPIO_IRQ_MASK_GPIO2RISC1);
        break;
#if(MCU_CORE_B92|MCU_CORE_B93|MCU_CORE_B95)
        case test_item_sel_irq_src_rising_edge:
			 gpio_function_en(g_led_irq);
			 gpio_output_en(g_led_irq);
			 gpio_set_low_level(g_led_irq);
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
			     if(GPIO_SRC_PIN[i] != g_led_irq){
					 gpio_function_en(GPIO_SRC_PIN[i]);
					 gpio_output_dis(GPIO_SRC_PIN[i]);
					 gpio_input_en(GPIO_SRC_PIN[i]);
					 gpio_set_up_down_res(GPIO_SRC_PIN[i], GPIO_PIN_PULLDOWN_100K);
					 gpio_set_src_irq_group((GPIO_SRC_PIN[i]>>8)&0xf);

				 }
			 }
			 core_interrupt_enable();
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
				if(GPIO_SRC_PIN[i] != g_led_irq){
					 plic_interrupt_enable(IRQ34_GPIO_SRC0 + BIT_LOW_BIT(GPIO_SRC_PIN[i]&0xff));
				}
			 }
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
				if(GPIO_SRC_PIN[i] != g_led_irq){
					 gpio_set_src_irq(GPIO_SRC_PIN[i],INTR_RISING_EDGE);
				}
			 }

		break;
        case test_item_sel_irq_src_falling_edge:
			 gpio_function_en(g_led_irq);
			 gpio_output_en(g_led_irq);
			 gpio_set_low_level(g_led_irq);
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
			     if(GPIO_SRC_PIN[i] != g_led_irq){
					 gpio_function_en(GPIO_SRC_PIN[i]);
					 gpio_output_dis(GPIO_SRC_PIN[i]);
					 gpio_input_en(GPIO_SRC_PIN[i]);
					 gpio_set_up_down_res(GPIO_SRC_PIN[i], GPIO_PIN_PULLUP_10K);
					 gpio_set_src_irq_group((GPIO_SRC_PIN[i]>>8)&0xf);
				 }
			 }
			 core_interrupt_enable();
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
				if(GPIO_SRC_PIN[i] != g_led_irq){
					 plic_interrupt_enable(IRQ34_GPIO_SRC0 + BIT_LOW_BIT(GPIO_SRC_PIN[i]&0xff));
				}
			 }
			 for(unsigned char i = 0; i < sizeof(GPIO_SRC_PIN)/sizeof(GPIO_SRC_PIN[0]); i++){
				if(GPIO_SRC_PIN[i] != g_led_irq){
					 gpio_set_src_irq(GPIO_SRC_PIN[i],INTR_FALLING_EDGE);
				}
			 }
		break;
#endif
		default :break;
	}
}

/*********************************************************************************************************
 *												main loop
 *********************************************************************************************************/
unsigned char g_input_value[10];
volatile unsigned int index = 0;

void main_loop (void)
{
	switch((unsigned char)(*(&g_test_item_e))){
		case test_item_output_toggle:
			for(unsigned char i = 0; i < sizeof(GPIO_PIN)/sizeof(GPIO_PIN[0]); i++){
				gpio_toggle(GPIO_PIN[i]);
			}
			index++;
			delay_ms(1000);
		break;
		case test_item_input_low:
        case test_item_input_high:
			 gpio_get_level_all((unsigned char *)g_input_value);
			 //if(g_input_value[4] == 0x03) gpio_set_high_level(g_led_irq);
			 delay_ms(100);
		break;
        case test_item_input_dropdown_sws:
        case test_item_input_pullup_sws:
        	if(gpio_get_level(SWS_PIN)){
        		gpio_set_high_level(g_led_irq);
        	}
        	else{
        		gpio_set_low_level(g_led_irq);
        	}
        	delay_ms(100);
        break;
        default:delay_ms(100);break;
	}
}

#endif

/********************************************************************************************************
 * @file     rc_ir.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
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
 *******************************************************************************************************/

#ifndef RC_IR_H_
#define RC_IR_H_
#if (FEATURE_TEST_MODE == TEST_IR)

#define PWM_CLK_SPEED					1000000 //pwm clock 1M.
#define IR_CARRIER_FREQ					38000  	// 1 frame -> 1/38k -> 1000/38 = 26 us
#define PWM_CARRIER_CYCLE_TICK			( PWM_CLK_SPEED/IR_CARRIER_FREQ )  //16M: 421 tick, f = 16000000/421 = 38004,T = 421/16=26.3125 us
#define PWM_CARRIER_HIGH_TICK			( PWM_CARRIER_CYCLE_TICK/3 )   // 1/3 duty

#define PWM_CARRIER_HALF_CYCLE_TICK		(PWM_CARRIER_CYCLE_TICK>>1)

///////////////////PWM Clock  /////////////////////////////////
enum{
	CLOCK_PWM_CLOCK_1S  = PWM_CLK_SPEED,
	CLOCK_PWM_CLOCK_1MS = (CLOCK_PWM_CLOCK_1S / 1000),
	CLOCK_PWM_CLOCK_1US = (CLOCK_PWM_CLOCK_1S / 1000000),
};

#define IR_HIGH_CARR_TIME			565			// in us
#define IR_HIGH_NO_CARR_TIME		1685
#define IR_LOW_CARR_TIME			560
#define IR_LOW_NO_CARR_TIME			565
#define IR_INTRO_CARR_TIME			9000
#define IR_INTRO_NO_CARR_TIME		4500

#define IR_SWITCH_CODE              0x0d
#define IR_ADDR_CODE                0x00
#define IR_CMD_CODE                 0xbf

#define IR_REPEAT_INTERVAL_TIME     40500
#define IR_REPEAT_NO_CARR_TIME      2250
#define IR_END_TRANS_TIME			563

#define IR_CARRIER_DUTY				3
#define IR_LEARN_SERIES_CNT     	160



enum{
	IR_SEND_TYPE_TIME_SERIES,
	IR_SEND_TYPE_BYTE,
	IR_SEND_TYPE_HALF_TIME_SERIES,
};



typedef struct{
	u32 cycle;
	u16 hich;
	u16 cnt;
}ir_ctrl_t;


typedef struct{
	ir_ctrl_t *ptr_irCtl;
	u8 type;
	u8 start_high;
	u8 ir_number;
	u8 code;
}ir_send_ctrl_data_t;


#define IR_GROUP_MAX		8

#define IR_SENDING_NONE  		0
#define IR_SENDING_DATA			1
#define IR_SENDING_REPEAT		2

typedef struct{
	u8 is_sending;
	u8 repeat_enable;
	u8 last_cmd;
	u8 rsvd;

	u32 sending_start_time;
}ir_send_ctrl_t;

/////////////  NEC  protocol  /////////////////////////////////////////////
//start
#define IR_INTRO_CARR_TIME_NEC          9000
#define IR_INTRO_NO_CARR_TIME_NEC       4500
//stop
#define IR_END_TRANS_TIME_NEC           563  // user define
//repeat
#define IR_REPEAT_CARR_TIME_NEC         9000
#define IR_REPEAT_NO_CARR_TIME_NEC      2250
#define IR_REPEAT_LOW_CARR_TIME_NEC		560
//data "1"
#define IR_HIGH_CARR_TIME_NEC	        560
#define IR_HIGH_NO_CARR_TIME_NEC		1690
//data "0"
#define IR_LOW_CARR_TIME_NEC			560
#define IR_LOW_NO_CARR_TIME_NEC         560



#define     PWM_IR_MAX_NUM              80     //user can define this max number according to application

#define		PWM0_PULSE_NORMAL   		0

#define     PWM_DMA_CHN                 DMA3

#define 	PWM_PIN						(PWM_PWM0_PE3)
#define 	PWM_ID						(get_pwmid(PWM_PIN))

typedef struct{
    unsigned short data[PWM_IR_MAX_NUM];
    unsigned int   data_num;
    unsigned int   dma_len;        // dma len
}pwm_dma_data_t;

enum{
	CLOCK_16M_SYS_TIMER_CLK_1US = 16,
	CLOCK_16M_SYS_TIMER_CLK_1MS = 16*1000,
	CLOCK_16M_SYS_TIMER_CLK_1S =  16*1000*1000,
};

void ir_nec_send(u8 addr1, u8 addr2, u8 cmd);
void pwm_stop_dma_ir_sending(void);
int ir_is_sending(void);
int ir_sending_check(void);
void ir_send_release(void);
void rc_ir_irq_prc(void);
void rc_ir_init(void);

#endif
#endif

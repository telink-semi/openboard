/********************************************************************************************************
 * @file    app_ui.h
 *
 * @brief   This is the header file for app_ui
 *
 * @author  Zigbee Group
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
 *******************************************************************************************************/
#ifndef _APP_UI_H_
#define _APP_UI_H_
/**********************************************************************
 * CONSTANT
 */

/**********************************************************************
 * TYPEDEFS
 */
enum{
	APP_STATE_NORMAL,
	APP_FACTORY_NEW_SET_CHECK
};

#define 	SEND_DATA	        	0x00   // 指定短地址发送数据
#define 	OPEN_ECHO 	  	   	 	0x01   // 打开回显
#define 	CLOSE_ECHO 		    	0x02   // 关闭回显
#define 	SET_BAUDRATE 	    	0x03   // 设置波特率 固定几个
#define 	SET_PERMIT_TIME     	0x04   // 限定入网时间设定
#define 	SET_ID              	0x05   // 设定ID
#define 	SET_PER_BRO_TIME    	0x06   // 设定广播ID的周期
#define 	SET_SHORT_TIME			0x07   // 设置短按检测时间 ms  复位后起作用
#define 	SET_LONG_TIME			0x08   // 设置长按检测时间 ms  复位后起作用
#define 	SEND_SHORT_ADDR			0x09   // RF发送自身短地址
#define 	SEND_LONG_ADDR			0x0A   // RF发送自身长地址
#define 	SEND_ID					0x0B   // RF发送自身ID
#define 	SET_GPIO_LEVEL			0x0C   // 设置某个GPIO引脚高低电平状态
#define 	READ_GPIO_LEVEL			0x0D   // 读某个GPIO引脚高低电平状态
#define 	SET_GPIO_STATE			0x0E   // 设置设备入网/允许其他设备入网时的某个IO及其电平状态
#define 	SET_GPIO_SYS_EXP		0x0F   // 设置设备死机时的某个IO及其电平状态
#define 	SET_GPIO_PWM_OPEN	    0x10   // 设置GPIO  PWM 周期 高电平时间
#define 	SET_GPIO_PWM_CLOSE	    0x11   // 关闭GPIO  PWM 功能
#define 	SET_GPIO_PWM_RESTART    0x12   // 重启 PWM 功能
#define 	SHOW_MODE				0x13   // 显示当前模式
#define 	SHOW_DEVICE_ID 			0x14   // 显示DEVICE_ID
#define 	SHOW_PER_BRO_TIME   	0x15   // 显示广播DEVICE_ID的周期
#define 	SHOW_PERMIT_TIME    	0x16   // 显示限定入网时间
#define 	SHOW_ECHO           	0x17   // 显示是否有回显
#define 	SHOW_PAN_ID      		0x18   // 显示PAN_ID
#define 	SHOW_C_LONG_ADDR    	0x19   // 显示coordinator长地址
#define 	SHOW_LONG_ADDR      	0x1A   // 显示长地址
#define 	SHOW_SHORT_ADDR      	0x1B   // 显示短地址
#define 	SHOW_DEVICE_KIND    	0x1C   // 显示设备类型
#define 	SHOW_BUTTON_TICK	    0x1D   // 显示长按和短按时间
#define 	SHOW_BUTTON_SELECT	    0x1E   // 显示状态按键的选择和电平
#define 	SHOW_PWM_SELECT	   	    0x1F   // 显示PWM 周期 和 占空比
#define 	SHOW_ALL				0x20   // 显示所有数据
#define 	OPEN_FEEDBACK		    0x21   // 打开设置后的反馈
#define 	CLOSE_FEEDBACK	    	0x22   // 关闭设置后的反馈
#define 	SET_DEFAULT 	    	0x30   // 设置默认
#define 	SET_RESET 	   	    	0x31   // 复位芯片
#define 	SET_FACTORY		    	0x32   // 恢复出场设置

#define LED_ON						1
#define LED_OFF						0
#define TICKS_INTERVAL    	 		10	// 单位：毫秒
#define DEBOUNCE_TICKS    	 		2	// MAX 8

#define EVENT_CB(ev)   if(handle->cb[ev]) handle->cb[ev]((Button*)handle)

typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS_START,
	LONG_PRESS_HOLD,
	number_of_event,
	NONE_PRESS
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  l_pre_s: 1;
	uint8_t  repeat : 3;
	uint8_t  event  : 4;
	uint8_t  state  : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  (*hal_button_Level)(void);
	BtnCallback cb[number_of_event];
	struct Button* next;
}Button;

typedef enum{
    D_COORDINATOR = 0x0,
	D_ROUTER 	  = 0x1,
	D_END_DEV     = 0x2,
}device_type_e;

typedef enum{
    NV_FACTORY = 0xff,
    NV_RECODE  = 2,
}nvram_state_e;

typedef struct PWM_INFO {
	unsigned char num;
	unsigned char dutycycle;
	unsigned char temp[2];
	unsigned int tick_max;
}PWM_INFO;

typedef struct Module_Col {
	volatile nvram_state_e  NvramState;
	volatile uint8_t   module_function; // 模式选择
	volatile uint8_t   device_id; 	    // 设备ID : 0 - 255
	volatile uint8_t   permit_time; 	// 允许入网窗口时间 单位:秒
	volatile uint8_t   per_bro_time;    // 周期广播自身ID时间 单位:秒
	volatile uint32_t  baudrate; 	    // 波特率
	volatile uint8_t   feedback_open:1;			// 是否打开设置后的反馈
	volatile uint8_t   echo_open:1; 	    	// 是否开启回显
	volatile uint8_t   join_net_io_level:1; 	// 入网触发IO触发电平
	volatile uint8_t   sys_exception_io_level:1;// 死机触发IO触发电平
	volatile uint8_t   join_net_io; 	        // 入网触发IO
	volatile uint8_t   sys_exception_io; 	    // 死机触发IO
	volatile PWM_INFO  pwm_info; 	    		// PWM
	volatile uint8_t short_ticks;  // ( 0 - 255 /TICKS_INTERVAL)
	volatile uint16_t long_ticks;  // ( 3000 /TICKS_INTERVAL)
	volatile uint8_t gpio_group_num;
	volatile uint8_t pwm_group_num;

}Module_Col;

extern volatile Module_Col g_config_st;

void CFG_Load(void);
void CFG_Save(void);
void CFG_Default(void);

void light_init(void);
void light_on(void);
void light_off(void);
void led_on(u32 pin);
void led_off(u32 pin);

void button_init(struct Button* handle, uint8_t(*pin_level)());
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
int button_ticks(void *par);
void Key_Init(void);

unsigned char  my_set_gpio_level(unsigned char num, unsigned char value);
unsigned char  my_read_gpio_level(unsigned char num);
unsigned char  my_set_pwm_run(unsigned char num, unsigned int frequency, unsigned int dutycycle);
unsigned char  my_set_pwm_stop(void);
unsigned char  my_set_pwm_restart(void);

void app_handler(void);
#endif	/* _APP_UI_H_ */

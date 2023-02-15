/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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

#if (__PROJECT_TL_GW__)

#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "sampleGateway.h"
#include "app_ui.h"

volatile Module_Col g_config_st = {0};

unsigned char  	SHORT_TICKS = (200 /TICKS_INTERVAL);
unsigned short  LONG_TICKS  = (2000 /TICKS_INTERVAL);

const gpio_pin_e gpio_group[] = {
    GPIO_PD3,GPIO_PE0,GPIO_PA5,GPIO_PA6,
	GPIO_PA7,GPIO_PB2,GPIO_PB3,GPIO_PB4,
	GPIO_PB5
};

const pwm_pin_e pwm_group[][2] = {
    {PWM_PWM0_PB4,0},
	{PWM_PWM1_PB5,1},
	{PWM_PWM3_N_PD3,3},
	{PWM_PWM3_PE0,3}
};


void led_on(u32 pin){
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin){
	drv_gpio_write(pin, LED_OFF);
}

void light_on(void){
	led_on(LED_PERMIT);
}

void light_off(void){
	led_off(LED_PERMIT);
}

void light_init(void){
	gpio_function_en(LED_PERMIT);
	gpio_input_dis(LED_PERMIT);
	gpio_output_en(LED_PERMIT);
	gpio_set_up_down_res(LED_PERMIT, GPIO_PIN_PULLDOWN_100K);
	led_off(LED_PERMIT);
}

unsigned char  my_set_gpio_level(unsigned char num, unsigned char value){
	if(num >= sizeof(gpio_group)/sizeof(gpio_group[0])){
		return 0xff;
	}
	gpio_function_en(gpio_group[num]);
	gpio_input_dis(gpio_group[num]);
	gpio_output_en(gpio_group[num]);
	gpio_set_level(gpio_group[num],value);
	return 0;
}

unsigned char my_read_gpio_level(unsigned char num){
	if(num >= sizeof(gpio_group)/sizeof(gpio_group[0])){
		return 0xff;
	}
	gpio_function_en(gpio_group[num]);
	gpio_input_en(gpio_group[num]);
	gpio_output_dis(gpio_group[num]);
	gpio_set_up_down_res(gpio_group[num], GPIO_PIN_PULLDOWN_100K);
	delay_us(10);
	return gpio_get_level(gpio_group[num]);
}

unsigned char my_set_pwm_run(unsigned char num, unsigned int frequency, unsigned int dutycycle){
	if(num >= sizeof(pwm_group)/sizeof(pwm_group[0]) || dutycycle > 100 || frequency == 0){
		return 0xff;
	}
	unsigned int tick_max = (sys_clk.pclk * 1000 * 1000)/frequency;
	g_config_st.pwm_info.num = num;
	g_config_st.pwm_info.tick_max = tick_max;
	g_config_st.pwm_info.dutycycle = dutycycle;

	pwm_set_pin(pwm_group[num][0]);
	pwm_set_clk(0);
	pwm_set_tmax(pwm_group[num][1], tick_max);
	pwm_set_tcmp(pwm_group[num][1], (dutycycle*tick_max)/100);
	pwm_start(pwm_group[num][1]);
	CFG_Save();
	return 0;
}

unsigned char my_set_pwm_stop(void){
	g_config_st.pwm_info.num = 0xff;
	pwm_stop(0);
	pwm_stop(1);
	pwm_stop(2);
	pwm_stop(3);
	pwm_stop(4);
	pwm_stop(5);
	CFG_Save();
	return 0;
}

unsigned char  my_set_pwm_restart(void){
	return my_set_pwm_run(g_config_st.pwm_info.num,g_config_st.pwm_info.tick_max,g_config_st.pwm_info.dutycycle);
}

static struct Button* head_handle = NULL;
struct Button btn1;
struct Button btn2;
struct Button btn3;

uint8_t read_button1_GPIO(){
	return gpio_get_level(BUTTON1);
}

uint8_t read_button2_GPIO(){
	return gpio_get_level(BUTTON2);
}

uint8_t read_button3_GPIO(){
	return gpio_get_level(BUTTON3);
}

void BTN_SINGLE_Click_Handler(void* btn){
	zb_mgmtPermitJoinReq(0xffff,g_config_st.permit_time,1,NULL,NULL);
}

void BTN_DOUBLE_Click_Handler(void* btn){
	zb_mgmtPermitJoinReq(0xffff,0,1,NULL,NULL);
}

void BTN_LONG_PRESS_HOLD_Handler(void* btn){
	if(btn != &btn3){ // 模式发生了改变
		struct Button *handle = btn;
		handle->l_pre_s = 0;
		handle->active_level = !handle->active_level;
		handle->state = 0; //reset
		g_config_st.module_function = btn1.hal_button_Level()*2 + btn2.hal_button_Level();
	}
	else{
		// gLightCtx.state = APP_FACTORY_NEW_DOING;
		// zb_factoryReset();  // 0x80000 - 0x96000
	}
}

void Key_Init(void){
	gpio_function_en(BUTTON1);
	gpio_input_en(BUTTON1);
	gpio_output_dis(BUTTON1);
	gpio_set_up_down_res(BUTTON1, GPIO_PIN_PULLUP_10K);

	gpio_function_en(BUTTON2);
	gpio_input_en(BUTTON2);
	gpio_output_dis(BUTTON2);
	gpio_set_up_down_res(BUTTON2, GPIO_PIN_PULLUP_10K);

	gpio_function_en(BUTTON3);
	gpio_input_en(BUTTON3);
	gpio_output_dis(BUTTON3);
	gpio_set_up_down_res(BUTTON3, GPIO_PIN_PULLUP_10K);

 	button_init(&btn1, read_button1_GPIO);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN_LONG_PRESS_HOLD_Handler);
	button_start(&btn1);

	button_init(&btn2, read_button2_GPIO);
	button_attach(&btn2, LONG_PRESS_HOLD,  BTN_LONG_PRESS_HOLD_Handler);
	button_start(&btn2);

	button_init(&btn3, read_button3_GPIO);
	button_attach(&btn3, SINGLE_CLICK,     BTN_SINGLE_Click_Handler);
	button_attach(&btn3, DOUBLE_CLICK,     BTN_DOUBLE_Click_Handler);
	button_attach(&btn3, LONG_PRESS_HOLD,  BTN_LONG_PRESS_HOLD_Handler);
	button_start(&btn3);

	g_config_st.module_function = btn1.hal_button_Level()*2 + btn2.hal_button_Level();

    TL_ZB_TIMER_SCHEDULE(button_ticks,NULL,TICKS_INTERVAL);
}

void button_init(struct Button* handle, uint8_t(*pin_level)()){
    memset(handle, 0, sizeof(struct Button));
	handle->event = (uint8_t)NONE_PRESS;//标记为未按下
	handle->hal_button_Level = pin_level;//读函数
	handle->button_level = handle->hal_button_Level();//读电平
	handle->active_level = !pin_level();//触发电平
}

//添加按钮回调函数
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb){
    handle->cb[event] = cb;
}

//返回按钮状态
PressEvent get_button_event(struct Button* handle){
	return (PressEvent)(handle->event);
}

//按钮驱动状态机
void button_handler(struct Button* handle){
	uint8_t read_gpio_level = handle->hal_button_Level();
	if((handle->state) > 0) handle->ticks++;
	//按钮去抖
	if(read_gpio_level != handle->button_level){//电平发生改变
		//连续读 DEBOUNCE_TICKS 次一样
		if(++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	}
	else {
	    handle->debounce_cnt = 0;
	}

	switch (handle->state) {
	    case 0:
			if(handle->button_level == handle->active_level) {	//检测到按下
			    handle->event = (uint8_t)PRESS_DOWN;//标记状态为按下
				EVENT_CB(PRESS_DOWN);
				handle->ticks  = 0;//清零
				handle->repeat = 1;//
				handle->state  = 1;//状态标记为1
			}
			else {
			    handle->event = (uint8_t)NONE_PRESS;
			}
			break;

		case 1:
			if(handle->button_level != handle->active_level) { //松开
			    handle->event = (uint8_t)PRESS_UP;
				EVENT_CB(PRESS_UP);
				handle->ticks = 0;
				handle->state = 2;//下一个状态
			}
			else if(handle->ticks > LONG_TICKS){//长按
			    handle->event = (uint8_t)LONG_PRESS_START;
				EVENT_CB(LONG_PRESS_START);
				handle->state = 4;
			}
			break;

		case 2:
			if(handle->button_level == handle->active_level) { //又一次按下
			    handle->event = (uint8_t)PRESS_DOWN;
				EVENT_CB(PRESS_DOWN);
				handle->repeat++;
				EVENT_CB(PRESS_REPEAT);
				handle->ticks = 0;
				handle->state = 3;
			}
			else if(handle->ticks > SHORT_TICKS){
			    if(handle->repeat == 1) {
				    handle->event = (uint8_t)SINGLE_CLICK;
					EVENT_CB(SINGLE_CLICK);
				}
				else if(handle->repeat == 2) {
					handle->event = (uint8_t)DOUBLE_CLICK;
					EVENT_CB(DOUBLE_CLICK);
				}
				handle->state = 0;
			}
			break;

		case 3:
			if(handle->button_level != handle->active_level){ //松开
			    handle->event = (uint8_t)PRESS_UP;
				EVENT_CB(PRESS_UP);
				if(handle->ticks < SHORT_TICKS) {
					handle->ticks = 0;
				    handle->state = 2;
				}
				else {
				    handle->state = 0;
				}
			}
			else if(handle->ticks > SHORT_TICKS){
			    handle->state = 0;
			}
			break;

		case 4:
			if(handle->button_level == handle->active_level) {//长按
			    handle->event = (uint8_t)LONG_PRESS_HOLD;
			    if(handle->l_pre_s == 0){
			    	handle->l_pre_s = 1;
			    	EVENT_CB(LONG_PRESS_HOLD);
			    }
			}
			else{
				handle->event = (uint8_t)PRESS_UP;
				EVENT_CB(PRESS_UP);
				handle->state = 0; //reset
				handle->l_pre_s = 0;
			}
			break;
	}
}

int button_start(struct Button* handle){
	struct Button* target = head_handle;
	while(target){ // 遍历到最后一个
		if(target == handle) return -1;	// 已经存在
		target = target->next;
	}
	handle->next = head_handle;  // handle下一个指向NULL
	head_handle  = handle;	     // 插入
	return 0;
}

void button_stop(struct Button* handle){
	struct Button** curr;
	for(curr = &head_handle; *curr; ){
		struct Button* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;
			// 释放内存
			return;
		}
		else
			curr = &entry->next;
	}
}

int button_ticks(void *par){
	struct Button* target;
	for(target = head_handle; target; target = target->next){
	    button_handler(target);
	}
	return 0;
}

void app_handler(void){
	/*
	if(2 == g_config_st.module_function){

	}
	else if(3 == g_config_st.module_function){

	}
	*/
}

void CFG_Load(void){
	nv_flashReadNew(1, NV_MODULE_APP, NV_ITEM_ZB_INFO, sizeof(g_config_st), (u8*)&g_config_st);

	if(NV_RECODE != g_config_st.NvramState){
	    CFG_Default();
	}
 	//CFG_Default();
	if(g_config_st.baudrate != 9600 && g_config_st.baudrate !=19200  && g_config_st.baudrate != 115200){
		g_config_st.baudrate = 115200;
	}

	if(0xff != g_config_st.pwm_info.num){
		my_set_pwm_restart();
	}

	g_config_st.gpio_group_num =  sizeof(gpio_group)/sizeof(gpio_group[0]);
	g_config_st.pwm_group_num  =  sizeof(pwm_group)/sizeof(pwm_group[0]);

	g_config_st.long_ticks  = g_config_st.long_ticks > 3*g_config_st.short_ticks ? g_config_st.long_ticks : 3*g_config_st.short_ticks;

	SHORT_TICKS = g_config_st.short_ticks;
	LONG_TICKS  = g_config_st.long_ticks;
	my_set_gpio_level(g_config_st.join_net_io,!g_config_st.join_net_io_level);
	my_set_gpio_level(g_config_st.sys_exception_io,!g_config_st.sys_exception_io_level);
}

void CFG_Save(void){
	g_config_st.NvramState = NV_RECODE;
	nv_flashWriteNew(1, NV_MODULE_APP, NV_ITEM_ZB_INFO, sizeof(g_config_st), (u8*)&g_config_st);
}

void CFG_Default(void){
	g_config_st.NvramState   =  NV_FACTORY;
	g_config_st.echo_open    =  0;
	g_config_st.device_id    =  0;
	g_config_st.permit_time  =  180;
	g_config_st.per_bro_time =  2;
	g_config_st.baudrate     =  115200;

	g_config_st.feedback_open 		   = 0;         // 是否打开设置后的反馈
	g_config_st.join_net_io_level      = 0; 	    // 入网触发IO触发电平
	g_config_st.sys_exception_io_level = 0;		    // 死机触发IO触发电平
	g_config_st.join_net_io			   = 2;  		// 入网触发IO
	g_config_st.sys_exception_io	   = 3;  		// 死机触发IO

	g_config_st.pwm_info.num = 0xff;
	g_config_st.pwm_info.tick_max = 0;
	g_config_st.pwm_info.dutycycle = 0;

	g_config_st.short_ticks = (200 /TICKS_INTERVAL);
	g_config_st.long_ticks  = (2000 /TICKS_INTERVAL);
	g_config_st.long_ticks  = g_config_st.long_ticks > 3*g_config_st.short_ticks ? g_config_st.long_ticks : 3*g_config_st.short_ticks;
}

#endif  /* __PROJECT_TL_GW__ */

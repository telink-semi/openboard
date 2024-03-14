#include "../app_config.h"
#include "button.h"

static struct Button* head_handle = NULL;
struct Button btn_left;
struct Button btn_mid;
struct Button btn_right;

void button_init(struct Button* handle, uint8_t(*pin_level)()){
    memset(handle, 0, sizeof(struct Button));
	handle->event = (uint8_t)NONE_PRESS;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level();
	handle->active_level = !pin_level();
}

void button_attach(struct Button* handle, PressEvent event, BtnCallback cb){
    handle->cb[event] = cb;
}

PressEvent get_button_event(struct Button* handle){
	return (PressEvent)(handle->event);
}

void button_handler(struct Button* handle){
	uint8_t read_gpio_level = handle->hal_button_Level();
	if((handle->state) > 0) handle->ticks++;
	if(read_gpio_level != handle->button_level){
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
			if(handle->button_level == handle->active_level) {
			    handle->event = (uint8_t)PRESS_DOWN;
				//EVENT_CB(PRESS_DOWN);
				handle->ticks  = 0;
				handle->repeat = 1;
				handle->state  = 1;
			}
			else {
			    handle->event = (uint8_t)NONE_PRESS;
			}
			break;

		case 1:
			if(handle->button_level != handle->active_level) {
			    handle->event = (uint8_t)PRESS_UP;
				//EVENT_CB(PRESS_UP);
				handle->ticks = 0;
				handle->state = 2;
			}
			else if(handle->ticks > LONG_TICKS){
			    handle->event = (uint8_t)LONG_PRESS_START;
				//EVENT_CB(LONG_PRESS_START);
				handle->state = 4;
			}
			break;

		case 2:
			if(handle->button_level == handle->active_level) {
			    handle->event = (uint8_t)PRESS_DOWN;
				//EVENT_CB(PRESS_DOWN);
				handle->repeat++;
				//EVENT_CB(PRESS_REPEAT);
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
			if(handle->button_level != handle->active_level){
			    handle->event = (uint8_t)PRESS_UP;
				//EVENT_CB(PRESS_UP);
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
			if(handle->button_level == handle->active_level) {
			    handle->event = (uint8_t)LONG_PRESS_HOLD;
			    if(handle->l_pre_s == 0){
			    	handle->l_pre_s = 1;
			    	EVENT_CB(LONG_PRESS_HOLD);
			    }
			}
			else{
				handle->event = (uint8_t)PRESS_UP;
				// EVENT_CB(PRESS_UP);
				handle->state = 0; //reset
				handle->l_pre_s = 0;
			}
			break;
	}
}

uint8_t button_start(struct Button* handle){
	struct Button* target = head_handle;
	while(target){
		if(target == handle) return -1;
		target = target->next;
	}
	handle->next = head_handle;
	head_handle  = handle;
	return 0;
}

void button_stop(struct Button* handle){
	struct Button** curr;
	for(curr = &head_handle; *curr; ){
		struct Button* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;
			return;
		}
		else
			curr = &entry->next;
	}
}

uint8_t button_ticks(void){
	struct Button* target;
	for(target = head_handle; target; target = target->next){
	    button_handler(target);
	}
	return 0;
}

uint8_t read_btn_left(void){
	return gpio_get_level(BUTTON_LFT);
}

uint8_t read_btn_mid(void){
	return gpio_get_level(BUTTON_MID);
}

uint8_t read_btn_right(void){
	return gpio_get_level(BUTTON_RHT);
}

__attribute__((weak)) void BTN_SINGLE_CLICK_Handler(void* btn){
	if(btn == &btn_left){
		gpio_toggle(LED);
	}
	else if(btn == &btn_mid){
		gpio_toggle(LED);
	}
	else if(btn == &btn_right){
		gpio_toggle(LED);
	}
}

__attribute__((weak)) void BTN_DOUBLE_CLICK_Handler(void* btn){
	if(btn == &btn_left){
		uart_send_array("btn_left DOUBLE_CLICK\r\n",strlen("btn_left DOUBLE_CLICK\r\n"));
	}
	else if(btn == &btn_mid){
		uart_send_array("btn_mid DOUBLE_CLICK\r\n",strlen("btn_mid DOUBLE_CLICK\r\n"));
	}
	else if(btn == &btn_right){
		uart_send_array("btn_right DOUBLE_CLICK\r\n",strlen("btn_right DOUBLE_CLICK\r\n"));
	}
}

void init_button(void)
{
	gpio_function_en(BUTTON_LFT);
	gpio_input_en(BUTTON_LFT);
	gpio_output_dis(BUTTON_LFT);
	gpio_set_up_down_res(BUTTON_LFT, GPIO_PIN_PULLUP_10K);

	gpio_function_en(BUTTON_MID);
	gpio_input_en(BUTTON_MID);
	gpio_output_dis(BUTTON_MID);
	gpio_set_up_down_res(BUTTON_MID, GPIO_PIN_PULLUP_10K);

	gpio_function_en(BUTTON_RHT);
	gpio_input_en(BUTTON_RHT);
	gpio_output_dis(BUTTON_RHT);
	gpio_set_up_down_res(BUTTON_RHT, GPIO_PIN_PULLUP_10K);
}

void binding_button(void)
{
	button_init(&btn_left, read_btn_left);
	button_attach(&btn_left, SINGLE_CLICK,  BTN_SINGLE_CLICK_Handler);
	button_attach(&btn_left, DOUBLE_CLICK,  BTN_DOUBLE_CLICK_Handler);
	button_start(&btn_left);

	button_init(&btn_mid, read_btn_mid);
	button_attach(&btn_mid, SINGLE_CLICK,  BTN_SINGLE_CLICK_Handler);
	button_attach(&btn_mid, DOUBLE_CLICK,  BTN_DOUBLE_CLICK_Handler);
	button_start(&btn_mid);

	button_init(&btn_right, read_btn_right);
	button_attach(&btn_right, SINGLE_CLICK,  BTN_SINGLE_CLICK_Handler);
	button_attach(&btn_right, DOUBLE_CLICK,  BTN_DOUBLE_CLICK_Handler);
	button_start(&btn_right);
}

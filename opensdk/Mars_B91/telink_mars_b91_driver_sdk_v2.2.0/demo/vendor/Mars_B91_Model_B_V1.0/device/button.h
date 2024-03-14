#ifndef	__BUTTON_H
#define	__BUTTON_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define TICKS_INTERVAL    	 		10	// Unit: milliseconds
#define DEBOUNCE_TICKS    	 		2	// Range: 1 to 7
#define LONG_TICKS    	 			(2000 /TICKS_INTERVAL)
#define SHORT_TICKS    	 			(200 /TICKS_INTERVAL)

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

void init_button(void);
uint8_t read_btn_left(void);
uint8_t read_btn_mid(void);
uint8_t read_btn_right(void);
void binding_button(void);
uint8_t button_ticks(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

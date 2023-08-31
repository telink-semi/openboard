#ifndef	__WS2812B_H
#define	__WS2812B_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define     PWM_PCLK_SPEED				12000000
#define 	PWM_PIN						(PWM_PWM0_PB4)
#define 	PWM_ID						(get_pwmid(PWM_PIN))
#define 	DMA_CHN   					DMA2
#define 	WS2812B_NUM					1

void ws2812b_init(void);
void ws2812b_test(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

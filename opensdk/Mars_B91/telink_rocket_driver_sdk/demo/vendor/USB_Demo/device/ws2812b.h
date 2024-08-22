#ifndef	__WS2812B_H
#define	__WS2812B_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define 	WS2812B_NUM					1
#define 	WS2812B_IO					GPIO_PB4

void ws2812b_init(void);
void ws2812b_test(void);
void ws2812b_set(unsigned int color);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

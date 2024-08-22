#ifndef	__WS2812B_H
#define	__WS2812B_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define 	WS2812B_NUM2					1
#define 	WS2812B_IO2					GPIO_PB4

void ws2812b_init2(void);
void ws2812b_test2(void);
void ws2812b_set2(unsigned int color);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

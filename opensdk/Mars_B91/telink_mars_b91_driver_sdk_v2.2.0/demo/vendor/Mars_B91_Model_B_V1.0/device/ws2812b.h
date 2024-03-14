#ifndef	__WS2812B_H
#define	__WS2812B_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define 	WS2812B_MAX					100
#define 	WS2812B_NUM					1
#define 	WS2812B_IO					GPIO_PE4

void ws2812b_init(void);
void ws2812b_test(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

#ifndef	__SHT30_H
#define	__SHT30_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define 	SHT3X_ADDR					0x88
uint8_t ReadSHT3x(float *Temp,float *Hum);
void sht3x_test(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif


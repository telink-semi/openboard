#ifndef	__PHOTORESISTOR_H
#define	__PHOTORESISTOR_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

void photoresistor_init(void);
unsigned short adc_get_voltage_dma(void);
unsigned int get_res_value(void);
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

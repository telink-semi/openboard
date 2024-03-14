#ifndef	__LIS2DH_H
#define	__LIS2DH_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define 	LIS2DH_ADDR					0x30

typedef struct {
	short x;
	short y;
	short z;
}axis_info_t;
extern axis_info_t g_axis_info;
uint8_t Lis2dh_Init(void);
void get_acc_value(axis_info_t *sample);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

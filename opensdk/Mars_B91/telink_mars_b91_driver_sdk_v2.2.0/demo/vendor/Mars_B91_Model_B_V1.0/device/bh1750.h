#ifndef	__BH1750_H
#define	__BH1750_H
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#define BH1750_ADDR				    0x46

#define POWER_OFF					0x00
#define POWER_ON					0x01
#define MODULE_RESET				0x07
#define	CONTINUE_H_MODE				0x10
#define CONTINUE_H_MODE2			0x11
#define Measure_Mode   				CONTINUE_H_MODE

#if (Measure_Mode == CONTINUE_H_MODE2)
	#define Resolurtion		0.5
#elif (Measure_Mode == CONTINUE_H_MODE)
	#define Resolurtion		1
#endif

uint8_t bh1750_Init(void);
void get_LingtIntensity(void);

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif

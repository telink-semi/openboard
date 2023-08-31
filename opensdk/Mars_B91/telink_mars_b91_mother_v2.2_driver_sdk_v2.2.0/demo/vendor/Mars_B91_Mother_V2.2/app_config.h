#ifndef	__APP_CONFIG_H
#define	__APP_CONFIG_H
#include "driver.h"
#include <stdio.h>
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define     I2C_CLK_SPEED		1000000
/*******************pin defined******************/
#define LED            			GPIO_PB6
#define BUTTON_LFT				GPIO_PD1
#define BUTTON_MID           	GPIO_PD2
#define BUTTON_RHT         		GPIO_PD3
#define WS2812B_DIN            	PWM_PWM0_PB4
#define UART0_TX_PIN   			UART0_TX_PB2
#define UART0_RX_PIN			UART0_RX_PB3
#define ADC_SAMPLE_PIN			ADC_GPIO_PB5
#define I2C_GPIO_SCL_PIN   		I2C_GPIO_SCL_E0
#define I2C_GPIO_SDA_PIN   		I2C_GPIO_SDA_C2
/*******************error defined******************/
typedef enum{
	EPERM    	 = -1,
	ENOENT   	 = -2,
 	EINTR    	 = -3,
    ENAMETOOLONG = -4,
	ENOMEM       = -5,
	EAGAIN		 = -6,
	EPIPE    	 = -7,
	EFBIG  		 = -8,
	EBUSY    	 = -9,
 	ESPIPE   	 = -10,
 	E2BIG 		 = -11,
    EOPDIS 	     = -12,
	ECRC 	     = -13,
	ERANGE 	     = -14,
	ETYPE 	     = -15,
	ESUPPORT     = -16,
	EEVKCMD      = -17,
	ESWIRE 		 = -18,
	ETIME 		 = -19,
	EACK 		 = -20,
	ERESET		 = -21,
	EACTIVE		 = -22,
	ELOCKFLASH   = -23,
}ERROR_Typdef;
/*******************example_select defined******************/
#define EXAMPLE_ALL				0
#define EXAMPLE_KEY				1
#define EXAMPLE_ADC				2
#define EXAMPLE_UART			3
#define EXAMPLE_OLED			4
#define EXAMPLE_SHT30			5
#define EXAMPLE_LIS2DHTR		6
#define EXAMPLE_WS2812B			7
#define EXAMPLE_SELECT			EXAMPLE_ALL
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif


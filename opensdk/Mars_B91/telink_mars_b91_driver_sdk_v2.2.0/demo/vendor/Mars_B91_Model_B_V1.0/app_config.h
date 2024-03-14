#ifndef	__APP_CONFIG_H
#define	__APP_CONFIG_H
#include "driver.h"
#include <stdio.h>
#include "device/serial_port.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif
#define	I2C_CLK_SPEED			   100000
#define	SPI_CLK_SPEED			 25000000
#define SD_SPI_MODE	 			SPI_MODE3
#define SD_FPP_CLK	    		  4000000
#define SD_FOD_CLK	    		   240000
#define DMIC_GROUP 				DMIC_GROUPC_C1_DAT_C2_C3_CLK
/*******************pin defined******************/
#define LED            			GPIO_PB6
#define USB_DM_PIN            	GPIO_PA5
#define USB_DP_PIN            	GPIO_PA6
#define SWS_PIN            		GPIO_PA7
#define BUTTON_LFT				GPIO_PB1
#define BUTTON_MID           	GPIO_PE3
#define BUTTON_RHT         		GPIO_PE1
#define PWM_LED_PIN            	PWM_PWM1_PB5
#define WS2812B_DIN            	PWM_PWM4_PE4
#define UART0_TX_PIN   			UART0_TX_PB2
#define UART0_RX_PIN			UART0_RX_PB3
#define UART0_CTS_PIN   		UART0_CTS_PD0
#define UART0_RTS_PIN			UART0_RTS_PB4
#define ADC_SAMPLE_PIN			ADC_GPIO_PB0
#define LED_R_PIN   			GPIO_PE6
#define LED_G_PIN   			GPIO_PE7
#define LED_B_PIN   			GPIO_PE5
#define DMIC_DAT_PIN   			GPIO_PC1
#define DMIC_CLK1_PIN   		GPIO_PC2
#define DMIC_CLK2_PIN   		GPIO_PC3
#define I2C_GPIO_SCL_PIN   		I2C_GPIO_SCL_E0
#define I2C_GPIO_SDA_PIN   		I2C_GPIO_SDA_E2
#define PSPI_MISO_PIN   		PSPI_MISO_IO1_PD2
#define PSPI_MOSI_PIN   		PSPI_MOSI_IO0_PD3
#define PSPI_CK_PIN   		    PSPI_CLK_PD1
#define PSPI_CN_PIN   			PSPI_CSN_PC0
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
#define EXAMPLE_KEY				1
#define EXAMPLE_ADC				2
#define EXAMPLE_UART			3
#define EXAMPLE_LCD				4
#define EXAMPLE_SHT30			5
#define EXAMPLE_LIS2DHTR		6
#define EXAMPLE_WS2812B			7
#define EXAMPLE_BH1750			8
#define EXAMPLE_SDCARD			9
#define EXAMPLE_FATFS			10
#define EXAMPLE_LUA				11
#define EXAMPLE_YMODEM			12
#define EXAMPLE_SELECT			EXAMPLE_YMODEM
/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
#endif


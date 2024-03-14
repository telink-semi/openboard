/********************************************************************************************************
 * @file    app.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "app_config.h"

#define  ADC_DMA_CHN		DMA7
#define  ADC_SAMPLE_PIN		ADC_GPIO_PD1
#define  ADC_SAMPLE_NUM		8
#define  ADC_SAMPLE_FREQ	ADC_SAMPLE_FREQ_96K
#define  ADC_SAMPLE_NDMA_DELAY_TIME	((1000/(6*(2<<(ADC_SAMPLE_FREQ)))) + 1)//delay 2 sample cycle

volatile unsigned short adc_temp_val;
volatile unsigned short adc_vol_mv_val;
volatile unsigned short adc_code_val;
volatile unsigned short adc_sample_buffer[ADC_SAMPLE_NUM] __attribute__((aligned(4))) = {0};


unsigned short adc_sort_and_get_average_code(void);
unsigned short adc_get_voltage_dma(void);
unsigned short adc_get_temperature_dma(void);
unsigned short adc_get_voltage(void);
unsigned short adc_get_temperature(void);

void user_init()
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
#if (ADC_MODE == ADC_DMA_MODE)
#if(MCU_CORE_B91)
	adc_set_dma_config(ADC_DMA_CHN);
#elif(MCU_CORE_B92)
	//audio and adc share the audio fifo, and the same fifo cannot be configured for audio and adc at the same time.
	adc_set_dma_config(ADC_DMA_CHN, FIFO1);
#endif
#endif

#if (ADC_SAMPLE_MODE == ADC_GPIO_SAMPLE)
	/**
	*   Change the default Pre_scale setting as ADC_PRESCALE_1F4.
	*	Changed by chaofan.20201230.
	**/
	adc_gpio_sample_init(ADC_SAMPLE_PIN, ADC_VREF_1P2V, ADC_PRESCALE_1F4, ADC_SAMPLE_FREQ);
#elif(ADC_SAMPLE_MODE == ADC_VBAT_SAMPLE)
	adc_battery_voltage_sample_init();
#elif(ADC_SAMPLE_MODE == ADC_TEMP_SENSOR_SAMPLE)
	adc_temperature_sample_init();
#endif
	adc_power_on();
}

void main_loop (void)
{
#if (ADC_MODE == ADC_DMA_MODE)
#if (ADC_SAMPLE_MODE != ADC_TEMP_SENSOR_SAMPLE)
	adc_vol_mv_val = adc_get_voltage_dma();
#else
	adc_temp_val = adc_get_temperature_dma();
#endif
#elif(ADC_MODE == ADC_NDMA_MODE)
#if (ADC_SAMPLE_MODE != ADC_TEMP_SENSOR_SAMPLE)
	adc_vol_mv_val = adc_get_voltage();
#else
	adc_temp_val = adc_get_temperature();
#endif
#endif
	delay_ms(1000);
	gpio_toggle(LED1);
}
/**
 * @brief This function serves to sort adc sample code and get average value.
 * @return 		adc_code_average 	- the average value of adc sample code.
 */
unsigned short adc_sort_and_get_average_code(void)
{

	unsigned short adc_code_average = 0;
	int i, j;
	unsigned short temp;
	/**** insert Sort and get average value ******/
	for(i = 1 ;i < ADC_SAMPLE_NUM; i++)
	{
		if(adc_sample_buffer[i] < adc_sample_buffer[i-1])
		{
			temp = adc_sample_buffer[i];
			adc_sample_buffer[i] = adc_sample_buffer[i-1];
	/**
		 * add judgment condition "j>=0" in for loop,
		 * otherwise may have array out of bounds.
		 * changed by chaofan.20201230.
	 */
			for(j=i-1; j>=0 && adc_sample_buffer[j] > temp;j--)
			{
				adc_sample_buffer[j+1] = adc_sample_buffer[j];
			}
			adc_sample_buffer[j+1] = temp;
		}
	}

	//get average value from raw data(abandon 1/4 small and 1/4 big data)
	for (i = ADC_SAMPLE_NUM>>2; i < (ADC_SAMPLE_NUM - (ADC_SAMPLE_NUM>>2)); i++)
	{
		adc_code_average += adc_sample_buffer[i]/(ADC_SAMPLE_NUM>>1);
	}
	return adc_code_average;
}
/**
 * @brief This function serves to get adc sample code by dma and convert to voltage value.
 * @return 		adc_vol_mv_average 	- the average value of adc voltage value.
 */
unsigned short adc_get_voltage_dma(void)
{
	unsigned short adc_vol_mv_average = 0;
	unsigned short adc_code_average = 0;

	adc_get_code_dma((unsigned short *)adc_sample_buffer, ADC_SAMPLE_NUM);
	adc_code_average = adc_sort_and_get_average_code();
	adc_vol_mv_average = adc_calculate_voltage(adc_code_average);
	return adc_vol_mv_average;
}

/**
 * @brief This function serves to get adc sample code by manual and convert to voltage value.
 * @return 		adc_vol_mv_average 	- the average value of adc voltage value.
 */
unsigned short adc_get_voltage(void)
{
	unsigned short adc_vol_mv_average = 0;
	unsigned short adc_code_average = 0;
	for (int i = 0; i < ADC_SAMPLE_NUM; i++)
	{
	/**
	 * move the "2 sample cycle" wait operation before adc_get_code(),
	 * otherwise may have data lose due to no waiting when adc_power_on.
	 * changed by chaofan.20201230.
	 */
		delay_us(ADC_SAMPLE_NDMA_DELAY_TIME);//wait at least 2 sample cycle(f = 96K, T = 10.4us)
		adc_sample_buffer[i] = adc_get_code();
	}
	adc_code_average = adc_sort_and_get_average_code();
	adc_vol_mv_average = adc_calculate_voltage(adc_code_average);
	return adc_vol_mv_average;
}
/**
 * @brief This function serves to get temperature sensor adc sample code by dma and temperature value.
 * @return 		adc_temp_average 	- the average value of temperature value.
 */
unsigned short adc_get_temperature_dma(void)
{
	unsigned short adc_temp_average = 0;
	unsigned short adc_code_average = 0;

	adc_get_code_dma((unsigned short *)adc_sample_buffer, ADC_SAMPLE_NUM);
	adc_code_average = adc_sort_and_get_average_code();
	adc_temp_average = adc_calculate_temperature(adc_code_average);
	return adc_temp_average;
}

/**
 * @brief This function serves to get adc sample code by manual and convert to temperature value.
 * @return 		adc_temp_average 	- the average value of temperature value.
 */
unsigned short adc_get_temperature(void)
{
	unsigned short adc_temp_average = 0;
	unsigned short adc_code_average = 0;

	for (int i = 0; i < ADC_SAMPLE_NUM; i++)
	{
	/**
		 * move the "2 sample cycle" wait operation before adc_get_code(),
		 * otherwise may have data lose due to no waiting when adc_power_on.
		 * changed by chaofan.20201230.
	 */		
		delay_us(ADC_SAMPLE_NDMA_DELAY_TIME);//wait at least 2 sample cycle(f = 96K, T = 10.4us)
		adc_sample_buffer[i] = adc_get_code();
	}
	adc_code_average = adc_sort_and_get_average_code();
	adc_temp_average = adc_calculate_temperature(adc_code_average);
	return adc_temp_average;
}





#include "../app_config.h"
#include "photoresistor.h"
#include "serial_port.h"
#include "oled.h"

#define  ADC_DMA_CHN		DMA7
#define  ADC_SAMPLE_NUM		8
#define  ADC_SAMPLE_FREQ	ADC_SAMPLE_FREQ_96K
#define  ADC_SAMPLE_NDMA_DELAY_TIME	((1000/(6*(2<<(ADC_SAMPLE_FREQ)))) + 1)//delay 2 sample cycle


volatile unsigned short adc_vol_mv_val;
volatile unsigned short adc_code_val;
volatile unsigned short adc_sample_buffer[ADC_SAMPLE_NUM] __attribute__((aligned(4))) = {0};

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

unsigned short adc_get_voltage_dma(void)
{
	unsigned short adc_vol_mv_average = 0;
	unsigned short adc_code_average = 0;
	adc_get_code_dma((unsigned short *)adc_sample_buffer, ADC_SAMPLE_NUM);
	adc_code_average = adc_sort_and_get_average_code();
	adc_vol_mv_average = adc_calculate_voltage(adc_code_average);
	return adc_vol_mv_average;
}

void photoresistor_init(void)
{
	adc_set_dma_config(ADC_DMA_CHN);
	adc_gpio_sample_init(ADC_SAMPLE_PIN, ADC_VREF_1P2V, ADC_PRESCALE_1F4, ADC_SAMPLE_FREQ);
	adc_power_on();
}

unsigned int get_res_value(void){
	unsigned short adc_temp_val = adc_get_voltage_dma();
	unsigned int Vol;
	unsigned int R13;
	Vol = (adc_temp_val*3300)/4096;
	R13 = 47000*Vol/(3300-Vol);
	char oled_show_buff[32];
	sprintf((char*)oled_show_buff,"V:%04d",Vol);
	OLED_ShowStr(72,4,(unsigned char *)oled_show_buff);
	sprintf((char*)oled_show_buff,"R:%04d",R13);
	OLED_ShowStr(72,6,(unsigned char *)oled_show_buff);
	return R13;
}


/********************************************************************************************************
 * @file    app_codec_0581.c
 *
 * @brief   This is the source file for B91m
 *
 * @author  Driver Group
 * @date    2023
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
#if (AUDIO_MODE == I2S_TO_CODEC_0581)

#include "driver.h"
#include "app_sin_data.h"

#define BUFFER_TO_DAC_OUTPUT                        (0) /* data route: mcu buffer -> i2s -> asrci -> dac output */
#define TWO_CHANNEL_ADC_INPUT_TO_BUFFER             (1) /* data route: two channels adc input -> asrco -> i2s -> mcu buffer */
#define ADC_INPUT_TO_BUFFER_TO_DAC_OUTPUT           (2) /* data route: one channel adc input -> asrco -> i2s -> mcu buffer -> i2s -> asrci -> dac output */
#define DIRECT_WRITE_CODEC_0581_REG                 (3) /* data route: one channel adc input -> asrco -> i2s -> mcu buffer -> i2s -> asrci -> dac output */
#define ADC_TO_BUFFER_TO_EQ_DAC_OUTPUT              (4) /* data route: one channel adc input -> asrco -> i2s -> mcu buffer -> i2s -> asrci -> eq -> dac output */
#define FDSP_ANC_SAMPLE                             (5) /* data route: input : adc0 -> asrco0 -> i2s left -> mcu
                                                                               adc1 -> asrco1 -> i2s right -> mcu
                                                                       output: adc0 after filtering   \
                                                                               adc1 after filtering    ¡ª¡ª mix -> dac output
                                                                               asrci after filtering  /
                                                        */

/* codec work mode select */
#define CODEC_0581_MODE                             BUFFER_TO_DAC_OUTPUT
#define I2S_SAMPLE_RATE                             (48000)

/* audio buffer define */
#define AUDIO_BUFF_SIZE                             (4096)
/* if data_width is 20 or 24, type need define as int */
short AUDIO_BUFF[AUDIO_BUFF_SIZE >> 1] __attribute__((aligned(4)));

/**
 *                                          i2s_clk_config[2]   i2s_clk_config[3]-->lrclk_adc(sampling rate)
 *                                                  ||                  ||
 *  pll(192M default)------->div---->i2s_clk--->2 * div(div=0,bypass)--->blck----->div
 *                           ||                                         ||
 *           i2s_clk_config[0]/i2s_clk_config[1]                 i2s_clk_config[4]-->lrclk_dac(sampling rate)
 * For example:sampling rate=16K, i2s_clk_config[5]={ 8,125,6,64,64}, sampling rate=192M*(8/125)/(2*6)/64=16K
 *
 */
unsigned short audio_i2s_16k_config[5] = {8,  125,  6,  64,  64}; /* sampling rate = 192M * (8 / 125) / (2 * 6) / 64   = 16K */
unsigned short audio_i2s_44k1_config[5] = {76, 235, 11, 64,  64}; /* sampling rate = 192M * (76 / 235) / (2 * 11) / 64 = 44.1K */
unsigned short audio_i2s_48k_config[5] = {2,  125,  0,  64,  64}; /* sampling rate = 192M * (2 / 125) / 64             = 48K */

void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);
    gpio_function_en(LED2);
    gpio_output_en(LED2);
    gpio_input_dis(LED2);

#if (CODEC_0581_MODE == BUFFER_TO_DAC_OUTPUT)
    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
        .sample_rate            = audio_i2s_48k_config, /* default i2s sample 48k and send sin_48k_mono_d1 buffer */
    };
    /* i2s output config */
    codec_0581_i2s_output_t i2s_output = {
        .tx_dma_num             = DMA1,
        .output_data_buf        = sin_48k_mono_d1,
        .output_buf_size        = sizeof(sin_48k_mono_d1),
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, 0, &i2s_output);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** output path: buffer -> sap/i2s -> asrci -> dac ****/
    /* asrci */
    codec_0581_output_asrci_config_t asrci_config = {
        .asrci_out_fs       = CODEC_ASRC_FS_48K,
    };
    /* dac */
    codec_0581_output_dac_config_t dac_config = {
        .dac_rate           = CODEC_DAC_SAMPLE_RATE_48KHz,
        .dac_input          = CODEC_DAC_ROUTE_ASRCI0,
    };
    /* output config */
    codec_0581_output_t codec_0581_output_config = {
        .asrci_config       = &asrci_config,
        .fint_config        = 0, /* this example fint not used */
        .dac_config         = &dac_config,
    };

    /**** codec_0581 output path init ****/
    codec_0581_output_init(&codec_0581_output_config);

    /* enable tx dma */
    audio_tx_dma_en(i2s_output.tx_dma_num);
#elif (CODEC_0581_MODE == TWO_CHANNEL_ADC_INPUT_TO_BUFFER)
    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
#if (I2S_SAMPLE_RATE == 16000)
        .sample_rate            = audio_i2s_16k_config,
#elif (I2S_SAMPLE_RATE == 44100)
        .sample_rate            = audio_i2s_44k1_config,
#elif (I2S_SAMPLE_RATE == 48000)
        .sample_rate            = audio_i2s_48k_config,
#endif
    };
    /* i2s input config */
    codec_0581_i2s_input_t i2s_input = {
        .i2s_ch_sel             = I2S_STEREO,
        .rx_dma_num             = DMA0,
        .input_data_buf         = AUDIO_BUFF,
        .input_buf_size         = AUDIO_BUFF_SIZE,
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, &i2s_input, 0);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** input path 0: adc channel 0 -> asrco -> sap/i2s left ****/
    /* adc0 */
    codec_0581_input_adc_config_t codec_0581_adc_config0 = {
        .adc_chnl           = CODEC_ADC_CHNL_0,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco0 */
    codec_0581_input_asrco_config_t codec_0581_asrco_config0 = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_0,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_ADC0,
    };
    /* sap left channel*/
    codec_0581_input_sap_config_t codec_0581_sap_config0= {
        .slot_id            = CODEC_SAP_SLOT0_LEFT,
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO0,
    };
    /* input config0 */
    codec_0581_input_t codec_0581_input_config0 = {
        .adc_config         = &codec_0581_adc_config0,
        .asrco_config       = &codec_0581_asrco_config0,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config0,
    };

    /**** input path 1: adc channel 1 -> asrco -> sap/i2s right ****/
    /* adc1 */
    codec_0581_input_adc_config_t codec_0581_adc_config1 = {
        .adc_chnl           = CODEC_ADC_CHNL_1,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco1 */
    codec_0581_input_asrco_config_t codec_0581_asrco_config1 = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_1,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_ADC1,
    };
    /* sap right channel */
    codec_0581_input_sap_config_t codec_0581_sap_config1= {
        .slot_id            = CODEC_SAP_SLOT1_RIGHT,
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO1,
    };
    /* input config1 */
    codec_0581_input_t codec_0581_input_config1 = {
        .adc_config         = &codec_0581_adc_config1,
        .asrco_config       = &codec_0581_asrco_config1,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config1,
    };

    /**** codec_0581 input path init ****/
    codec_0581_input_init(&codec_0581_input_config0);
    codec_0581_input_init(&codec_0581_input_config1);

    /* enable rx dma */
    audio_rx_dma_en(i2s_input.rx_dma_num);
#elif (CODEC_0581_MODE == ADC_INPUT_TO_BUFFER_TO_DAC_OUTPUT)
    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
#if (I2S_SAMPLE_RATE == 16000)
        .sample_rate            = audio_i2s_16k_config,
#elif (I2S_SAMPLE_RATE == 44100)
        .sample_rate            = audio_i2s_44k1_config,
#elif (I2S_SAMPLE_RATE == 48000)
        .sample_rate            = audio_i2s_48k_config,
#endif
    };
    /* i2s input config */
    codec_0581_i2s_input_t i2s_input = {
        .i2s_ch_sel             = I2S_MONO,
        .rx_dma_num             = DMA0,
        .input_data_buf         = AUDIO_BUFF,
        .input_buf_size         = AUDIO_BUFF_SIZE,
    };
    /* i2s output config */
    codec_0581_i2s_output_t i2s_output = {
        .tx_dma_num             = DMA1,
        .output_data_buf        = AUDIO_BUFF,
        .output_buf_size        = AUDIO_BUFF_SIZE,
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, &i2s_input, &i2s_output);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** input path: adc -> asrco -> sap/i2s ****/
    /* adc */
    codec_0581_input_adc_config_t codec_0581_adc_config = {
        .adc_chnl           = CODEC_ADC_CHNL_0,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco */
    codec_0581_input_asrco_config_t codec_0581_asrco_config = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_0,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_ADC0,
    };
    /* sap */
    codec_0581_input_sap_config_t codec_0581_sap_config = {
        .slot_id            = CODEC_SAP_SLOT1_RIGHT, /* The i2s channel is mono, it must be CODEC_SAP_SLOT1_RIGHT. */
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO0,
    };
    /* input config */
    codec_0581_input_t codec_0581_input_config = {
        .adc_config         = &codec_0581_adc_config,
        .asrco_config       = &codec_0581_asrco_config,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config,
    };

    /**** output path: sap/i2s -> asrci -> dac ****/
    /* asrci */
    codec_0581_output_asrci_config_t codec_0581_asrci_config = {
        .asrci_out_fs       = CODEC_ASRC_FS_48K,
    };
    /* dac */
    codec_0581_output_dac_config_t codec_0581_dac_config = {
        .dac_rate           = CODEC_DAC_SAMPLE_RATE_48KHz,
        .dac_input          = CODEC_DAC_ROUTE_ASRCI0,
    };
    /* output config */
    codec_0581_output_t codec_0581_output_config = {
        .asrci_config       = &codec_0581_asrci_config,
        .fint_config        = 0, /* this example fint not used */
        .dac_config         = &codec_0581_dac_config,
    };

    /**** codec_0581 input path init ****/
    codec_0581_input_init(&codec_0581_input_config);
    /**** codec_0581 output path init ****/
    codec_0581_output_init(&codec_0581_output_config);

    /* enable rx dma */
    audio_rx_dma_en(i2s_input.rx_dma_num);
    /* enable tx dma */
    audio_tx_dma_en(i2s_output.tx_dma_num);
#elif (CODEC_0581_MODE == DIRECT_WRITE_CODEC_0581_REG)
    /* generate by gui */
    extern unsigned int reg_pre[144][2];
    extern unsigned int reg_post[5][2];

    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
#if (I2S_SAMPLE_RATE == 16000)
        .sample_rate            = audio_i2s_16k_config,
#elif (I2S_SAMPLE_RATE == 44100)
        .sample_rate            = audio_i2s_44k1_config,
#elif (I2S_SAMPLE_RATE == 48000)
        .sample_rate            = audio_i2s_48k_config,
#endif
    };
    /* i2s input config */
    codec_0581_i2s_input_t i2s_input = {
        .i2s_ch_sel             = I2S_MONO,
        .rx_dma_num             = DMA0,
        .input_data_buf         = AUDIO_BUFF,
        .input_buf_size         = AUDIO_BUFF_SIZE,
    };
    /* i2s output config */
    codec_0581_i2s_output_t i2s_output = {
        .tx_dma_num             = DMA1,
        .output_data_buf        = AUDIO_BUFF,
        .output_buf_size        = AUDIO_BUFF_SIZE,
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, &i2s_input, &i2s_output);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** gui config route: adc0 -> asrco -> sap -> asrci -> dac ****/
    for (unsigned int i = 0; i < sizeof(reg_pre) / sizeof(reg_pre[0]); i++)
    {
        codec_0581_reg_write(reg_pre[i][0], reg_pre[i][1]);
    }

    for (unsigned int i = 0; i < sizeof(reg_post) / sizeof(reg_post[0]); i++)
    {
        codec_0581_reg_write(reg_post[i][0], reg_post[i][1]);
    }

    /* enable rx dma */
    audio_rx_dma_en(i2s_input.rx_dma_num);
    /* enable tx dma */
    audio_tx_dma_en(i2s_output.tx_dma_num);
#elif (CODEC_0581_MODE == ADC_TO_BUFFER_TO_EQ_DAC_OUTPUT)
    /* generate by gui */
    extern unsigned int eq_program[22]; /* attention : eq_program[length] and eq_param_bank[length], length is variable */
    extern unsigned int eq_param_bank0[10];

    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
#if (I2S_SAMPLE_RATE == 16000)
        .sample_rate            = audio_i2s_16k_config,
#elif (I2S_SAMPLE_RATE == 44100)
        .sample_rate            = audio_i2s_44k1_config,
#elif (I2S_SAMPLE_RATE == 48000)
        .sample_rate            = audio_i2s_48k_config,
#endif
    };
    /* i2s input config */
    codec_0581_i2s_input_t i2s_input = {
        .i2s_ch_sel             = I2S_MONO,
        .rx_dma_num             = DMA0,
        .input_data_buf         = AUDIO_BUFF,
        .input_buf_size         = AUDIO_BUFF_SIZE,
    };
    /* i2s output config */
    codec_0581_i2s_output_t i2s_output = {
        .tx_dma_num             = DMA1,
        .output_data_buf        = AUDIO_BUFF,
        .output_buf_size        = AUDIO_BUFF_SIZE,
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, &i2s_input, &i2s_output);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** input path: adc -> asrco -> sap/i2s ****/
    /* adc */
    codec_0581_input_adc_config_t codec_0581_adc_config = {
        .adc_chnl           = CODEC_ADC_CHNL_0,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco */
    codec_0581_input_asrco_config_t codec_0581_asrco_config = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_0,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_ADC0,
    };
    /* sap */
    codec_0581_input_sap_config_t codec_0581_sap_config = {
        .slot_id            = CODEC_SAP_SLOT1_RIGHT, /* The i2s channel is mono, it must be CODEC_SAP_SLOT1_RIGHT. */
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO0,
    };
    /* input config */
    codec_0581_input_t codec_0581_input_config = {
        .adc_config         = &codec_0581_adc_config,
        .asrco_config       = &codec_0581_asrco_config,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config,
    };

    /**** output path: sap/i2s -> asrci -> eq -> dac ****/
    /* asrci */
    codec_0581_output_asrci_config_t codec_0581_asrci_config = {
        .asrci_out_fs       = CODEC_ASRC_FS_48K,
    };
    /* dac */
    codec_0581_output_dac_config_t codec_0581_dac_config = {
        .dac_rate           = CODEC_DAC_SAMPLE_RATE_48KHz,
        .dac_input          = CODEC_DAC_ROUTE_EQ,
    };
    /* output config */
    codec_0581_output_t codec_0581_output_config = {
        .asrci_config       = &codec_0581_asrci_config,
        .fint_config        = 0, /* this example fint not used */
        .dac_config         = &codec_0581_dac_config,
    };

    /* eq config: bank 0 select, gui generate eq_program and eq_param_bank data */
    codec_0581_eq_config_t codec_0581_eq_config = {
        .bank_id            = CODEC_EQ_BANK_0,
        .eq_route_from      = CODEC_EQ_ROUTE_ASRCI_0,
        .program_addr       = eq_program,
        .program_len        = sizeof(eq_program) / sizeof(eq_program[0]),
        .param_addr         = eq_param_bank0,
        .param_len          = sizeof(eq_param_bank0) / sizeof(eq_param_bank0[0]),
    };

    /**** codec_0581 input path init ****/
    codec_0581_input_init(&codec_0581_input_config);
    /**** codec_0581 eq init ****/
    codec_0581_eq_init(&codec_0581_eq_config);
    /**** codec_0581 output path init ****/
    codec_0581_output_init(&codec_0581_output_config);

    /* enable rx dma */
    audio_rx_dma_en(i2s_input.rx_dma_num);
    /* enable tx dma */
    audio_tx_dma_en(i2s_output.tx_dma_num);
#elif (CODEC_0581_MODE == FDSP_ANC_SAMPLE)
    /* generate by gui */
    extern unsigned int fdsp_program[36]; /* attention : fdsp_program[length] and fdsp_param_bank_a[5][length], length is variable */
    extern unsigned int fdsp_param_bank_a[5][36];

    /* i2s config */
    codec_0581_i2s_init_t i2s_init = {
        .data_width             = I2S_BIT_16_DATA,
        .sample_rate            = audio_i2s_48k_config, /* default i2s sample 48k and send sin_48k_mono_d1 buffer */
    };
    /* i2s input config */
    codec_0581_i2s_input_t i2s_input = {
        .i2s_ch_sel             = I2S_STEREO,
        .rx_dma_num             = DMA0,
        .input_data_buf         = AUDIO_BUFF,
        .input_buf_size         = AUDIO_BUFF_SIZE,
    };
    /* i2s output config */
    codec_0581_i2s_output_t i2s_output = {
        .tx_dma_num             = DMA1,
        .output_data_buf        = sin_48k_mono_d1,
        .output_buf_size        = sizeof(sin_48k_mono_d1),
    };

    /* init i2s */
    codec_0581_i2s_init(&i2s_init, &i2s_input, &i2s_output);

    /* init codec_0581 */
    if (CODEC_0581_OK != codec_0581_init())
    {
        gpio_toggle(LED2);
        return;
    }

    /**** input path 0: adc channel 0 -> fdsp1 -> asrco0 -> sap/i2s left ****/
    /* adc0 */
    codec_0581_input_adc_config_t codec_0581_adc_config0 = {
        .adc_chnl           = CODEC_ADC_CHNL_0,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco0 */
    codec_0581_input_asrco_config_t codec_0581_asrco_config0 = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_0,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_FDSP1,
    };
    /* sap left channel*/
    codec_0581_input_sap_config_t codec_0581_sap_config0= {
        .slot_id            = CODEC_SAP_SLOT0_LEFT,
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO0,
    };
    /* input config0 */
    codec_0581_input_t codec_0581_input_config0 = {
        .adc_config         = &codec_0581_adc_config0,
        .asrco_config       = &codec_0581_asrco_config0,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config0,
    };

    /**** input path 1: adc channel 1 -> fdsp2 -> asrco1 -> sap/i2s right ****/
    /* adc1 */
    codec_0581_input_adc_config_t codec_0581_adc_config1 = {
        .adc_chnl           = CODEC_ADC_CHNL_1,
        .adc_rate           = CODEC_ADC_SAMPLE_RATE_48KHz,
    };
    /* asrco1 */
    codec_0581_input_asrco_config_t codec_0581_asrco_config1 = {
        .asrco_in_fs        = CODEC_ASRC_FS_48K,
        .asrco_chnl         = CODEC_ASRCO_CHNL_1,
        .asrco_route_from   = CODEC_ASRCO_ROUTE_FDSP2,
    };
    /* sap right channel */
    codec_0581_input_sap_config_t codec_0581_sap_config1= {
        .slot_id            = CODEC_SAP_SLOT1_RIGHT,
        .sap_route_from     = CODEC_SAP_OUT_ROUTE_FROM_ASRCO1,
    };
    /* input config1 */
    codec_0581_input_t codec_0581_input_config1 = {
        .adc_config         = &codec_0581_adc_config1,
        .asrco_config       = &codec_0581_asrco_config1,
        .fdec_config        = 0, /* this example fdec not used */
        .sap_config         = &codec_0581_sap_config1,
    };

    /**** output path: sap/i2s left -> asrci -> fdsp0 -> dac ****/
    /* asrci */
    codec_0581_output_asrci_config_t asrci_config = {
        .asrci_out_fs       = CODEC_ASRC_FS_48K,
    };
    /* dac */
    codec_0581_output_dac_config_t dac_config = {
        .dac_rate           = CODEC_DAC_SAMPLE_RATE_48KHz,
        .dac_input          = CODEC_DAC_ROUTE_FDSP0,
    };
    /* output config */
    codec_0581_output_t codec_0581_output_config = {
        .asrci_config       = &asrci_config,
        .fint_config        = 0, /* this example fint not used */
        .dac_config         = &dac_config,
    };

    /* fdsp config: bank a select, gui generate program and param data */
    codec_0581_fdsp_config_t fdsp_config = {
        .bank_id            = CODEC_FDSP_PARAM_BANK_A,
        .program_addr       = &fdsp_program[0],
        .program_len        = sizeof(fdsp_program) / sizeof(fdsp_program[0]),
        .param_addr         = &fdsp_param_bank_a[0][0],
    };

    /**** codec_0581 input0 path init ****/
    codec_0581_input_init(&codec_0581_input_config0);
    /**** codec_0581 input1 path init ****/
    codec_0581_input_init(&codec_0581_input_config1);
    /**** codec_0581 fdsp init ****/
    codec_0581_fdsp_init(&fdsp_config);
    /**** codec_0581 output path init ****/
    codec_0581_output_init(&codec_0581_output_config);

    /* enable rx dma */
    audio_rx_dma_en(i2s_input.rx_dma_num);
    /* enable tx dma */
    audio_tx_dma_en(i2s_output.tx_dma_num);
#endif
}

void main_loop(void)
{
    gpio_toggle(LED1);
    delay_ms(500);
}

#endif

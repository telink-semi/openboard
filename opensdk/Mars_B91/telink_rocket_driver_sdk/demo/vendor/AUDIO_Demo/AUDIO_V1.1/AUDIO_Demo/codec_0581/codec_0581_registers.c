/********************************************************************************************************
 * @file    codec_0581_registers.c
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
#include <stdint.h>

/**
  * Register Configuration
  * Array item as {register address, register value}
  */

unsigned int reg_pre[144][2] =
{
    /* pre-memory registers */
    {0xc051, 0x00000044}, /* DAC_CTRL2 */
    {0xc052, 0x000000FD}, /* DAC_VOL0 */
    {0xc004, 0x00000000}, /* ADC_DAC_HP_PWR */
    {0xc00c, 0x00000011}, /* CHIP_PWR */
    {0xc00c, 0x00000015}, /* CHIP_PWR */
    {0xc005, 0x00000081}, /* PLL_PGA_PWR */
    {0xc00e, 0x000000D8}, /* CLK_CTRL1 */
    {0xc00f, 0x00000003}, /* CLK_CTRL2 */
    {0xc010, 0x00000006}, /* CLK_CTRL3 */
    {0xc011, 0x00000000}, /* CLK_CTRL4 */
    {0xc012, 0x00000012}, /* CLK_CTRL5 */
    {0xc013, 0x00000000}, /* CLK_CTRL6 */
    {0xc014, 0x0000007D}, /* CLK_CTRL7 */
    {0xc015, 0x00000000}, /* CLK_CTRL8 */
    {0xc017, 0x00000001}, /* CLK_CTRL10 */
    {0xc018, 0x00000000}, /* CLK_CTRL11 */
    {0xc016, 0x00000001}, /* CLK_CTRL9 */
    {0xc016, 0x00000001}, /* CLK_CTRL9 */
    {0xc007, 0x00000001}, /* DSP_PWR */
    {0xc006, 0x00000003}, /* SAI_CLK_PWR */
    {0xc008, 0x00000011}, /* ASRC_PWR */
    {0xc009, 0x00000000}, /* FINT_PWR */
    {0xc00a, 0x00000000}, /* FDEC_PWR */
    {0xc00b, 0x00000010}, /* KEEPS */
    {0xc020, 0x00000022}, /* ADC_CTRL1 */
    {0xc021, 0x00000000}, /* ADC_CTRL2 */
    {0xc022, 0x00000000}, /* ADC_CTRL3 */
    {0xc023, 0x00000000}, /* ADC_CTRL4 */
    {0xc024, 0x00000040}, /* ADC_CTRL5 */
    {0xc025, 0x00000026}, /* ADC_CTRL6 */
    {0xc026, 0x00000040}, /* ADC_CTRL7 */
    {0xc027, 0x00000000}, /* ADC_MUTES */
    {0xc028, 0x00000040}, /* ADC0_VOL */
    {0xc029, 0x00000040}, /* ADC1_VOL */
    {0xc02a, 0x00000040}, /* ADC2_VOL */
    {0xc02b, 0x00000002}, /* ADC_DITHER_LEV */
    {0xc030, 0x00000000}, /* PGA0_CTRL1 */
    {0xc031, 0x00000000}, /* PGA0_CTRL2 */
    {0xc032, 0x00000000}, /* PGA1_CTRL1 */
    {0xc033, 0x00000000}, /* PGA1_CTRL2 */
    {0xc034, 0x00000000}, /* PGA2_CTRL1 */
    {0xc035, 0x00000000}, /* PGA2_CTRL2 */
    {0xc036, 0x00000000}, /* PGA_CTRL1 */
    {0xc039, 0x00000000}, /* PGA_CTRL2 */
    {0xc040, 0x00000000}, /* EQ_CFG */
    {0xc044, 0x00000000}, /* EQ_ROUTE */
    {0xc050, 0x00000002}, /* DAC_CTRL1 */
    {0xc054, 0x00000003}, /* DAC_ROUTE0 */
    {0xc060, 0x00000000}, /* HP_CTRL */
    {0xc061, 0x00000000}, /* HP_LVMODE_CTRL1 */
    {0xc062, 0x00000030}, /* HP_LVMODE_CTRL2 */
    {0xc063, 0x00000000}, /* HP_LVMODE_CTRL3 */
    {0xc066, 0x00000000}, /* HPLDO_CTRL */
    {0xc06f, 0x00000000}, /* PB_CTRL */
    {0xc070, 0x00000025}, /* FDEC_CTRL1 */
    {0xc071, 0x00000025}, /* FDEC_CTRL2 */
    {0xc074, 0x00000000}, /* FDEC_ROUTE0 */
    {0xc075, 0x00000000}, /* FDEC_ROUTE1 */
    {0xc076, 0x00000000}, /* FDEC_ROUTE2 */
    {0xc077, 0x00000000}, /* FDEC_ROUTE3 */
    {0xc080, 0x00000052}, /* FINT_CTRL1 */
    {0xc084, 0x00000000}, /* FINT_ROUTE0 */
    {0xc085, 0x00000000}, /* FINT_ROUTE1 */
    {0xc090, 0x00000002}, /* ASRCI_CTRL */
    {0xc091, 0x00000000}, /* ASRCI_ROUTE01 */
    {0xc093, 0x00000002}, /* ASRCO_CTRL */
    {0xc094, 0x00000000}, /* ASRCO_ROUTE0 */
    {0xc095, 0x00000000}, /* ASRCO_ROUTE1 */
    {0xc096, 0x00000000}, /* ASRCO_ROUTE2 */
    {0xc0a0, 0x00000000}, /* FDSP_RUN */
    {0xc0a1, 0x00000070}, /* FDSP_CTRL1 */
    {0xc0a2, 0x0000003F}, /* FDSP_CTRL2 */
    {0xc0a3, 0x00000000}, /* FDSP_CTRL3 */
    {0xc0a4, 0x00000000}, /* FDSP_CTRL4 */
    {0xc0a5, 0x0000007F}, /* FDSP_CTRL5 */
    {0xc0a6, 0x00000000}, /* FDSP_CTRL6 */
    {0xc0a7, 0x00000000}, /* FDSP_CTRL7 */
    {0xc0a8, 0x00000000}, /* FDSP_CTRL8 */
    {0xc0a9, 0x00000000}, /* FDSP_SL_ADDR */
    {0xc0aa, 0x00000000}, /* FDSP_SL_P0_0 */
    {0xc0ab, 0x00000000}, /* FDSP_SL_P0_1 */
    {0xc0ac, 0x00000000}, /* FDSP_SL_P0_2 */
    {0xc0ad, 0x00000000}, /* FDSP_SL_P0_3 */
    {0xc0ae, 0x00000000}, /* FDSP_SL_P1_0 */
    {0xc0af, 0x00000000}, /* FDSP_SL_P1_1 */
    {0xc0b0, 0x00000000}, /* FDSP_SL_P1_2 */
    {0xc0b1, 0x00000000}, /* FDSP_SL_P1_3 */
    {0xc0b2, 0x00000000}, /* FDSP_SL_P2_0 */
    {0xc0b3, 0x00000000}, /* FDSP_SL_P2_1 */
    {0xc0b4, 0x00000000}, /* FDSP_SL_P2_2 */
    {0xc0b5, 0x00000000}, /* FDSP_SL_P2_3 */
    {0xc0b6, 0x00000000}, /* FDSP_SL_P3_0 */
    {0xc0b7, 0x00000000}, /* FDSP_SL_P3_1 */
    {0xc0b8, 0x00000000}, /* FDSP_SL_P3_2 */
    {0xc0b9, 0x00000000}, /* FDSP_SL_P3_3 */
    {0xc0ba, 0x00000000}, /* FDSP_SL_P4_0 */
    {0xc0bb, 0x00000000}, /* FDSP_SL_P4_1 */
    {0xc0bc, 0x00000000}, /* FDSP_SL_P4_2 */
    {0xc0bd, 0x00000000}, /* FDSP_SL_P4_3 */
    {0xc0bf, 0x00000000}, /* FDSP_ONZ_MASK0 */
    {0xc0c0, 0x00000000}, /* FDSP_ONZ_MASK1 */
    {0xc0c1, 0x00000000}, /* FDSP_ONZ_MASK2 */
    {0xc0c2, 0x00000000}, /* RCOSC_ACTION */
    {0xc0c3, 0x00000000}, /* RCOSC_CALCFG0 */
    {0xc0c4, 0x00000000}, /* RCOSC_CALCFG1 */
    {0xc0c8, 0x00000000}, /* RCOSC_FXTRIM */
    {0xc0c9, 0x00000000}, /* RCOSC_TCTRIM */
    {0xc0d2, 0x00000000}, /* MCLKOUT_IRQ_CTRL */
    {0xc0d6, 0x00000000}, /* MP_CTRL7 */
    {0xc0e0, 0x00000005}, /* FSYNC0_CTRL */
    {0xc0e1, 0x00000005}, /* BCLK0_CTRL */
    {0xc0e2, 0x00000005}, /* SDATAO0_CTRL */
    {0xc0e3, 0x00000005}, /* SDATAI0_CTRL */
    {0xc0e4, 0x00000005}, /* IRQ_CTRL */
    {0xc0e5, 0x00000005}, /* MCLKO_CTRL */
    {0xc0ec, 0x00000033}, /* I2C_SPI_CTRL */
    {0xc100, 0x00000000}, /* IRQ_CTRL1 */
    {0xc101, 0x00000071}, /* IRQ1_MASK1 */
    {0xc102, 0x000000FF}, /* IRQ1_MASK2 */
    {0xc103, 0x00000077}, /* IRQ1_MASK3 */
    {0xc104, 0x00000071}, /* IRQ2_MASK1 */
    {0xc105, 0x000000FF}, /* IRQ2_MASK2 */
    {0xc106, 0x00000077}, /* IRQ2_MASK3 */
    {0xc150, 0x00000000}, /* SPT0_CTRL1 */
    {0xc151, 0x00000000}, /* SPT0_CTRL2 */
    {0xc152, 0x00000000}, /* SPT0_CTRL3 */
    {0xc153, 0x0000003F}, /* SPT0_ROUTE0 */
    {0xc154, 0x00000003}, /* SPT0_ROUTE1 */
    {0xc155, 0x0000003F}, /* SPT0_ROUTE2 */
    {0xc156, 0x0000003F}, /* SPT0_ROUTE3 */
    {0xc157, 0x0000003F}, /* SPT0_ROUTE4 */
    {0xc158, 0x0000003F}, /* SPT0_ROUTE5 */
    {0xc159, 0x0000003F}, /* SPT0_ROUTE6 */
    {0xc15a, 0x0000003F}, /* SPT0_ROUTE7 */
    {0xc15b, 0x0000003F}, /* SPT0_ROUTE8 */
    {0xc15c, 0x0000003F}, /* SPT0_ROUTE9 */
    {0xc15d, 0x0000003F}, /* SPT0_ROUTE10 */
    {0xc15e, 0x0000003F}, /* SPT0_ROUTE11 */
    {0xc15f, 0x0000003F}, /* SPT0_ROUTE12 */
    {0xc160, 0x0000003F}, /* SPT0_ROUTE13 */
    {0xc161, 0x0000003F}, /* SPT0_ROUTE14 */
    {0xc162, 0x0000003F}, /* SPT0_ROUTE15 */
    {0xcc04, 0x00000000}, /* DAC_NOISE_CTRL2 */
    {0xcc13, 0x00000000}, /* DAC_NOISE_CTRL1 */
};

unsigned int reg_post[5][2] =
{
    /* post-memory registers */
    {0xc007, 0x00000000}, /* DSP_PWR */
    {0xc00c, 0x00000017}, /* CHIP_PWR */
    {0xc004, 0x00000011}, /* ADC_DAC_HP_PWR */
    {0xc051, 0x00000004}, /* DAC_CTRL2 */
    {0xc052, 0x00000040}, /* DAC_VOL0 */
};

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
#if(TRAP_DEMO==INTERRUPT_NESTED_DEMO)
/**************************notices******************************************
 * ********timer clock use APB clock ******************/
#define TIMER_MODE_GPIO_TRIGGER_TICK    0xf
volatile unsigned int t0;
unsigned char  ble_tx_packet[48] __attribute__ ((aligned (4))) ={3,0,0,0,0,10,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xc,0xf};
#define TX_PKT_PAYLOAD		15
#define RF_FREQ				17
#if(MCU_CORE_B91)
#define RF_POWER			RF_POWER_P4p35dBm
#elif(MCU_CORE_B92)
#define RF_POWER			RF_POWER_P8p82dBm
#elif(MCU_CORE_B93)
#define RF_POWER            RF_POWER_P8p05dBm
#elif(MCU_CORE_B95)
#define RF_POWER            RF_POWER_P8p82dBm
#elif(MCU_CORE_B96)
#define RF_POWER            RF_POWER_P8p82dBm
#endif
#define ACCESS_CODE        0x29417671//0xd6be898e// 0x898e898e//
void user_init(void)
{
	gpio_function_en(LED1);
	gpio_output_en(LED1);
	gpio_input_dis(LED1);
	gpio_function_en(LED2);
	gpio_output_en(LED2);
	gpio_input_dis(LED2);
	gpio_function_en(LED3);
	gpio_output_en(LED3);
	gpio_input_dis(LED3);
	gpio_function_en(LED4);
	gpio_output_en(LED4);
	gpio_input_dis(LED4);
	gpio_set_low_level(LED1);
	gpio_set_low_level(LED2);
	gpio_set_low_level(LED3);
	gpio_set_low_level(LED4);
	plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);
	core_interrupt_enable();
	plic_set_priority(IRQ_SYSTIMER,IRQ_PRI_LEV3);
	plic_set_priority(IRQ_TIMER0, IRQ_PRI_LEV2);
	plic_set_priority(IRQ_ZB_RT, IRQ_PRI_LEV1);


	/******** rf init********/
	plic_interrupt_enable(IRQ_ZB_RT);

	rf_set_power_level(RF_POWER);
	rf_set_ble_chn(RF_FREQ);
	rf_access_code_comm(ACCESS_CODE);
	rf_set_tx_dma(2,128);


	unsigned char rf_data_len = TX_PKT_PAYLOAD+2;
	ble_tx_packet[4]=0;
	ble_tx_packet[5]=TX_PKT_PAYLOAD;
	unsigned int rf_tx_dma_len = rf_tx_packet_dma_len(rf_data_len);
	ble_tx_packet[3] = (rf_tx_dma_len >> 24)&0xff;
	ble_tx_packet[2] = (rf_tx_dma_len >> 16)&0xff;
	ble_tx_packet[1] = (rf_tx_dma_len >> 8)&0xff;
	ble_tx_packet[0] = rf_tx_dma_len&0xff;

	rf_set_txmode();
	delay_us(85);//Wait for calibration to stabilize


	  /******** stimer init********/
	plic_interrupt_enable(IRQ_SYSTIMER);
#if (MCU_CORE_B93)
    stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS); //1ms
#else
	stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS); //1ms
#endif

  /******** timer0 init********/
	plic_interrupt_enable(IRQ_TIMER0);
	timer_set_init_tick(TIMER0,0);
	timer_set_cap_tick(TIMER0,sys_clk.pclk*1000);//1ms
	timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
	rf_set_irq_mask(FLD_RF_IRQ_TX);
	timer_start(TIMER0);
#if (MCU_CORE_B93)
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_D25F);		//irq enable
#else
	stimer_set_irq_mask(FLD_SYSTEM_IRQ);		//irq enable
#endif
}

void main_loop(void)
{
	rf_tx_pkt ((void *)&ble_tx_packet);
	delay_us(1000);//1ms
	gpio_toggle(LED1);

}


_attribute_ram_code_sec_  void stimer_irq_handler(void)
{
#if (MCU_CORE_B93)
    if(stimer_get_irq_status(FLD_SYSTEM_IRQ_D25F))
#else
	if(stimer_get_irq_status(FLD_SYSTEM_IRQ))
#endif
	{
		gpio_set_high_level(LED2);
#if (MCU_CORE_B93)
        stimer_clr_irq_status(FLD_SYSTEM_IRQ_D25F);  //clr irq
        stimer_set_irq_capture_d25f(stimer_get_tick() + 16*1000);
#else
		stimer_clr_irq_status(FLD_SYSTEM_IRQ);  //clr irq
		stimer_set_irq_capture(stimer_get_tick() + 16*1000);
#endif
		delay_us(250);
		gpio_set_low_level(LED2);
	}
}
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)

_attribute_ram_code_sec_ void timer0_irq_handler(void)
{

	if(timer_get_irq_status(TMR_STA_TMR0))
	{
		gpio_set_high_level(LED3);
		timer_clr_irq_status(TMR_STA_TMR0);
		delay_us(600);
		gpio_set_low_level(LED3);
	}
}
PLIC_ISR_REGISTER(timer0_irq_handler, IRQ_TIMER0)

_attribute_ram_code_sec_  void rf_irq_handler(void)
{
	if(rf_get_irq_status(FLD_RF_IRQ_TX)){
		gpio_set_high_level(LED4);
		rf_clr_irq_status(FLD_RF_IRQ_TX);
		delay_us(800);
		gpio_set_low_level(LED4);
	}
	else
	{
		rf_clr_irq_status(FLD_RF_IRQ_ALL);
	}
}
PLIC_ISR_REGISTER(rf_irq_handler, IRQ_ZB_RT)
#elif(TRAP_DEMO ==HSP_DEMO)

/*
 * This demo program shows hardware stack protection and recording mechanism.
 *
 * Scenario:
 *
 * We use HANOI tower game to illustrate the Hardware Stack Protection features.
 * The main function will enable the HW stack operating scheme of recording
 * mechanism first to get the top of stack for HANOI towers moves using
 * recursion. And then, switch to enable the HW stack operating scheme of
 * stack protection mechanism to do the HANOI towers moves again by reducing
 * the stack size bound to generate the stack overflow exception trap.
 *
 */
#define TRAP_M_STACKOVF     	32
#define TRAP_M_STACKUDF     	33

/* How many disks for HANOI tower */
#define HANOI_DISKS			4
/* String buffer size */
#define STRBUF_SIZE			256
void towers(int num, char frompeg, char topeg, char auxpeg, int *move)// A, C,B
{
	/* consume the stack */
    char buf[STRBUF_SIZE];

	if (num == 1) {
		sprintf(buf, "\n Move disk 1 from %c to %c", frompeg, topeg);
		printf("%s", buf);
		(*move)++;
		return;
	}
	towers(num - 1, frompeg, auxpeg, topeg, move);
	sprintf(buf, "\n Move disk %d from %c to %c", num, frompeg, topeg);
	printf("%s", buf);

	(*move)++;
	towers(num - 1, auxpeg, topeg, frompeg, move);
}

void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);

	int disks, moves;
	unsigned long tos;
     long _stack;

	printf("\n hsp-demo-program\n");
	/*
	 * Check whether the CPU configured with HW Stack protection/recording or not.
	 * The MMSC_CFG bit 5 indicates this.
	 */
	if (!(read_csr(NDS_MMSC_CFG) & BIT(5))) {
		printf("CPU does NOT support HW Stack protection/recording.\n");
		goto error;
	}

	printf("\nThe initial stack pointer : 0x%x\n\n", (unsigned int)(long)&_stack);

	/*
	 * Following demos the HW Stack recording.
	 */
	printf("[HW Stack Recording]\n");

	/* Disable HW Stack protection first. */
	core_set_mhsp_ctr(0);

	printf("HANOI Tower with %d disks : \n", HANOI_DISKS);

	core_set_msp_bound(-1);/* Set SP bound to maximum */
	core_set_msp_base(0xa0000);//top of stack
	/* Enable MHSP_CTL : Top stack recording, machine mode enabled */
	core_set_mhsp_ctr( MHSP_CTL_OVF_EN|MHSP_CTL_SCHM_SEL|MHSP_CTL_M_EN|MHSP_CTL_UDF_EN);
	/* Hanoi Tower */
	disks = HANOI_DISKS;
	moves = 0;
	towers(disks, 'A', 'C', 'B', &moves);

	printf("\nHANOI(%d) = %d moves\n", disks, moves);
	/* get latest updated from the SP_BOUND register */
	tos = core_get_msp_bound();
	printf("Top of Stack : 0x%x \n\n", (unsigned int)tos);
	/*
	 * Following demos the HW Stack overflow detection.
	 */
	printf("[HW Stack Overflow Detection]\n");

	/* Disable HW Stack protection first. */
	core_set_mhsp_ctr(0);

	/*
	 * Set SP bound to stack overflow value.
	 * (Large than top of stack and 512 byte alignment)
	 */
	tos = ((tos + 512) & ~511);
	core_set_msp_bound(tos);
	printf("Set stack top bonud : 0x%x\n", (unsigned int)tos);

	printf("Retest...\nHANOI Tower with %d disks : \n", HANOI_DISKS);
	/* Enable MHSP_CTL : Overflow detection, machine mode enabled */
	core_set_mhsp_ctr( MHSP_CTL_OVF_EN | (~MHSP_CTL_SCHM_SEL)|MHSP_CTL_M_EN|MHSP_CTL_UDF_EN);
	/*
	 * Retest the HANOI tower moves again
	 * Since the SP bound is set to the value large than top of stack needed
	 * in HANOI tower moves, the stack overflow exception will be generated.
	 */
	disks = HANOI_DISKS;
	moves = 0;
	towers(disks, 'A', 'C', 'B', &moves);

	/* Never to here */
	printf("ERROR : HW Stack protection failed\n");

   error:
   printf("CPU does NOT support HSP\n");
}
_attribute_ram_code_sec_ void except_handler(void)
{
	unsigned long mcause = core_get_mcause();
	unsigned long mepc   = core_get_mepc();
	unsigned long bound  = core_get_msp_bound();
	unsigned long base   = core_get_msp_base();
	/* Show exception error type. */
	/* MCAUSE.Exccode[9:0] is exception/interrupt code for PLIC */
	switch (mcause & 0x3FF) {
		case TRAP_M_STACKOVF:
			/*sp may be greater than bound, because mcu will make a judgment,
			 the next time the value of sp is reduced to meet the Stack Overflow condition,
			 it will goto overflow exception, and the corresponding sp has not been updated. confirm by minghai*/
			printf("Stack Overflow (sp = 0x%x)\n", (unsigned int)core_get_current_sp());
			printf("The value of bound : 0x%x \n\n", (unsigned int)bound);
			break;
		case TRAP_M_STACKUDF:
			printf("Stack underflow (sp = 0x%x)\n", (unsigned int)core_get_current_sp());
			printf("The value of base : 0x%x \n\n", (unsigned int)base);
				break;
		default:
			/* Unhandled Trap */
			printf("Unhandled Trap : mcause = 0x%x, mepc = 0x%x\n", (unsigned int)mcause, (unsigned int)mepc);
			break;
	}
	while(1);
}
void main_loop(void)
{

	delay_us(1000);//1ms
	gpio_toggle(LED1);

}


#elif(TRAP_DEMO ==WFI_DEMO)

/**
 * The function of the demo: entry WFI mode and then awoken by stimer and timer0 interrupt.
 * there are two awoken modes:
 *  - When the core is awoken by the taken interrupt and global interrupts enable, it will resume and start to execute from the corresponding interrupt service routine.
 *  - When the core is awoken by the pending interrupt and global interrupts disable, it will resume and start to execute from the instruction after the WFI instruction.
 */
# define WFI_MODE_GLOBAL_INTR_EN    0
# define WFI_MODE_GLOBAL_INTR_DIS   1
# define WFI_AWOKEN_MODE   WFI_MODE_GLOBAL_INTR_EN
volatile unsigned int current_pc=0;

void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);
    gpio_function_en(LED2);
    gpio_output_en(LED2);
    gpio_input_dis(LED2);
    gpio_function_en(LED3);
    gpio_output_en(LED3);
    gpio_input_dis(LED3);

    /**
     * Enable MEI and disable MSI and MTI, so that MEI can wake up mcu after core enters WFI, if want to wake up with MSI or MTI, enable the bit of response.
     */
    core_mie_enable(FLD_MIE_MEIE);
    core_mie_disable(FLD_MIE_MSIE |FLD_MIE_MTIE);
#if (WFI_AWOKEN_MODE== WFI_MODE_GLOBAL_INTR_DIS)
    /* disable global interrupt. */
    core_interrupt_disable();
#else
    /* enable global interrupt. */
    core_interrupt_enable();
#endif

    /* init stimer */
#if (MCU_CORE_B93)
    stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_D25F);
#else
     stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
     stimer_set_irq_mask(FLD_SYSTEM_IRQ);
#endif
     plic_interrupt_enable(IRQ_SYSTIMER);

    /* init timer0 */
    timer_set_init_tick(TIMER0, 0);
    timer_set_cap_tick(TIMER0, 1500 * sys_clk.pclk * 1000);
    timer_set_mode(TIMER0, TIMER_MODE_SYSCLK);
    timer_start(TIMER0);
    plic_interrupt_enable(IRQ_TIMER0);

    current_pc = core_get_current_pc(); /* only for watch current_pc */
    printf("\r\n before enter WFI the current_pc=0x%4x\r\n", current_pc);
}

#if (WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_EN)
_attribute_ram_code_sec_ void timer0_irq_handler(void)
{
    /* resume and start to execute from the corresponding interrupt service routine.*/
    current_pc = core_get_current_pc(); /* only for watch current_pc */
    printf("\r\ncurrent_pc=0x%4x\r\n", current_pc);
    printf("\r\nleave wfi mode from timer0 interrupt service routine \r\n");

    if (timer_get_irq_status(TMR_STA_TMR0))
    {
        timer_clr_irq_status(TMR_STA_TMR0);
        gpio_toggle(LED3);
    }
}
PLIC_ISR_REGISTER(timer0_irq_handler, IRQ_TIMER0)

_attribute_ram_code_sec_ void stimer_irq_handler(void)
{
    /* resume and start to execute from the corresponding interrupt service routine.*/
    current_pc = core_get_current_pc(); /* only for watch current_pc */
    printf("\r\ncurrent_pc=0x%4x\r\n", current_pc);
    printf("\r\nleave wfi mode from stimer interrupt service routine \r\n");

#if (MCU_CORE_B93)
    if (stimer_get_irq_status(FLD_SYSTEM_IRQ_D25F))
    {
        stimer_clr_irq_status(FLD_SYSTEM_IRQ_D25F); /* clr irq */
        stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
        gpio_toggle(LED2);
    }
#else
    if (stimer_get_irq_status(FLD_SYSTEM_IRQ))
    {
        stimer_clr_irq_status(FLD_SYSTEM_IRQ); /* clr irq */
        stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S);
        gpio_toggle(LED2);
    }
#endif
}
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)
#endif /* end of WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_EN */

void main_loop(void)
{
#if (WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_DIS)
    /**
     * Before entering wfi mode, clear all current requests from the PLIC.
     */
    if (plic_clr_all_request() == 0)
    {
        /* clear plic request failed, check your irq status and non-wakeup source interrupts have been disabled */
        printf("clear plic request failed !!!\r\n");
        return;
    }
#endif /* end of WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_DIS */

    core_entry_wfi_mode(); /* WFI instruction enables the processor to enter the wait-for-interrupt (WFI) mode */

#if (WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_DIS)
    /**
     * Since global interrupts are disabled, make sure that all interrupts are claimed and completed after exiting from WFI mode, \n
     * otherwise the interrupt flag will remain set((When global interrupt enable, hardware handles the claim, complete is handled in the plic_isr function)).
     */
    unsigned int cur_claim = plic_interrupt_claim(); /* getting the wakeup source */
    printf(" current claim %d\r\n", cur_claim);
    switch (cur_claim)
    {
    case IRQ_SYSTIMER:
#if (MCU_CORE_B93)
        if (stimer_get_irq_status(FLD_SYSTEM_IRQ_D25F))
        {
            stimer_clr_irq_status(FLD_SYSTEM_IRQ_D25F); /* clr irq */
            /**
             * Re-set the stimer capture so that time0 interrupts come sooner. \n
             * The purpose of this is to allow the stimer and timer0 to wake up the WFI alternately, in practice it is not necessary to set the capture value alternately.
             */
            stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S + SYSTEM_TIMER_TICK_1MS * 500);
            gpio_toggle(LED2);
            plic_interrupt_complete(cur_claim); /* notify PLIC that the stimer interrupt processing is complete */
        }
#else
        if (stimer_get_irq_status(FLD_SYSTEM_IRQ))
        {
            stimer_clr_irq_status(FLD_SYSTEM_IRQ); /* clr irq */
            /**
             * Re-set the stimer capture so that time0 interrupts come sooner. \n
             * The purpose of this is to allow the stimer and timer0 to wake up the WFI alternately, in practice it is not necessary to set the capture value alternately.
             */
            stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S + SYSTEM_TIMER_TICK_1MS * 500);
            gpio_toggle(LED2);
            plic_interrupt_complete(cur_claim); /* notify PLIC that the stimer interrupt processing is complete */
        }
#endif
        break;
    case IRQ_TIMER0:
        if (timer_get_irq_status(TMR_STA_TMR0))
        {
            timer_clr_irq_status(TMR_STA_TMR0); /* clr irq */
            /**
             * Re-set the stimer capture so that stimer interrupts come sooner. \n
             * The purpose of this is to allow the stimer and timer0 to wake up the WFI alternately, in practice it is not necessary to set the capture value alternately.
             */
#if (MCU_CORE_B93)
            stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1S - SYSTEM_TIMER_TICK_1MS * 100);
#else
            stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1S - SYSTEM_TIMER_TICK_1MS * 100);
#endif
            gpio_toggle(LED3);
            plic_interrupt_complete(cur_claim); /* notify PLIC that the time0 interrupt processing is complete */
        }
        break;
    default:
        break;
    }

    /* resume and start to execute from the instruction after the WFI instruction. */
    current_pc = core_get_current_pc(); /* only for watch current_pc */
    printf("\r\ncurrent_pc=0x%4x\r\n", current_pc);
    printf("\r\n leave wfi mode from after the WFI instruction\r\n");
#endif /* end of WFI_AWOKEN_MODE == WFI_MODE_GLOBAL_INTR_DIS */

    gpio_toggle(LED1);
}
#elif (TRAP_DEMO == INTERRUPT_MTIME_DEMO)
void user_init(void)
{
    gpio_function_en(LED3);
    gpio_output_en(LED3);
    gpio_input_dis(LED3);
    gpio_function_en(LED4);
    gpio_output_en(LED4);
    gpio_input_dis(LED4);

    gpio_set_low_level(LED3);
    gpio_set_low_level(LED4);

    /* 1: mtime clk init */
    mtime_clk_init(CLK_32K_RC);

    /* 2: enable global interrupt */
    core_interrupt_enable();

    /* 3: enable machine time interrupt */
    core_mie_enable(FLD_MIE_MTIE);

    /* 4: set mtime interval */
    mtime_set_interval_ms(500);
}

void main_loop(void)
{
    delay_ms(500);
    gpio_toggle(LED3); /* indicate software is run */
}

/**
 * @brief     mtime irq handler.
 * @return    none
 */
_attribute_ram_code_sec_ void mtime_irq_handler(void)
{
    /* update the timer to specified period */
    mtime_set_interval_ms(500);
    gpio_toggle(LED4);
}
#elif (TRAP_DEMO == INTERRUPT_SWI_DEMO)
void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);
    gpio_function_en(LED2);
    gpio_output_en(LED2);
    gpio_input_dis(LED2);

    gpio_set_low_level(LED1);
    gpio_set_low_level(LED2);

    /* 1: enable global interrupt and software interrupt */
    core_interrupt_enable();

    /* 2: enable software interrupt */
    core_mie_enable(FLD_MIE_MSIE);

    /* 3: plic_sw interrupt enable */
    plic_sw_interrupt_enable();
}

void main_loop(void)
{
    delay_ms(500); /* indicate software is run */
    gpio_toggle(LED1);
    plic_sw_set_pending(); /* trigger swi */
}

/**
 * @brief     swi irq handler.
 * @return    none
 */
_attribute_ram_code_sec_ void mswi_irq_handler(void)
{
    gpio_toggle(LED2);
}
#elif (TRAP_DEMO == EXT_MTIME_SWI_NESTED_DEMO)

/**
 * @brief mie.mtie and mie.msie are not cleared in the core_save_nested_context() and these three interrupts(mei, msi and mti) do not occur simultaneously, they can be nested within each other.
 * 
 */
void user_init(void)
{
    gpio_function_en(LED1);
    gpio_output_en(LED1);
    gpio_input_dis(LED1);
    gpio_function_en(LED2);
    gpio_output_en(LED2);
    gpio_input_dis(LED2);
    gpio_function_en(LED3);
    gpio_output_en(LED3);
    gpio_input_dis(LED3);
    gpio_function_en(LED4);
    gpio_output_en(LED4);
    gpio_input_dis(LED4);

    gpio_set_low_level(LED1);
    gpio_set_low_level(LED2);
    gpio_set_low_level(LED3);
    gpio_set_low_level(LED4);

    /* 1: preempt enable */
    plic_preempt_feature_en(CORE_PREEMPT_PRI_MODE0);

    /* 2: enable global interrupt */
    core_interrupt_enable();

    /* 3: enable mtime interrupt and software interrupt */
    core_mie_enable(FLD_MIE_MTIE | FLD_MIE_MSIE);

    /* 4: plic_sw interrupt enable */
    plic_sw_interrupt_enable();

    /* 5: mtime clk init and enable */
    mtime_clk_init(CLK_32K_RC);
    mtime_set_interval_ms(1);

    /* 6: stimer init */
    plic_interrupt_enable(IRQ_SYSTIMER);
#if (MCU_CORE_B93)
    stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS); /* 1ms */
    stimer_set_irq_mask(FLD_SYSTEM_IRQ_D25F);                               /* irq enable */
#else
    stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS); /* 1ms */
    stimer_set_irq_mask(FLD_SYSTEM_IRQ);                               /* irq enable */
#endif
}

void main_loop(void)
{
    delay_ms(500); /* indicate software is run */
    gpio_toggle(LED1);
}

_attribute_ram_code_sec_ void mtime_irq_handler(void)
{
    gpio_set_high_level(LED2);
    /* update the timer to specified period */
    mtime_set_interval_ms(1);
    plic_sw_set_pending(); /* trigger swi */
    delay_us(100);
    gpio_set_low_level(LED2);
}

_attribute_ram_code_sec_ void mswi_irq_handler(void)
{
    gpio_set_high_level(LED3);
    delay_us(100);
    gpio_set_low_level(LED3);
}

_attribute_ram_code_sec_ void stimer_irq_handler(void)
{
#if (MCU_CORE_B93)
    if (stimer_get_irq_status(FLD_SYSTEM_IRQ_D25F))
#else
    if (stimer_get_irq_status(FLD_SYSTEM_IRQ))
#endif
    {
        gpio_set_high_level(LED4);
#if (MCU_CORE_B93)
        stimer_clr_irq_status(FLD_SYSTEM_IRQ_D25F); /* clr irq */
        stimer_set_irq_capture_d25f(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS);
#else
        stimer_clr_irq_status(FLD_SYSTEM_IRQ); /* clr irq */
        stimer_set_irq_capture(stimer_get_tick() + SYSTEM_TIMER_TICK_1MS);
#endif
        delay_us(500);
        gpio_set_low_level(LED4);
    }
}
PLIC_ISR_REGISTER(stimer_irq_handler, IRQ_SYSTIMER)

#endif

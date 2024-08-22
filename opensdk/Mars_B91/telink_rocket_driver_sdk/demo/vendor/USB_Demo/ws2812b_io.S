.section .ram_code, "ax"
.global ws2812b_bit02
.global ws2812b_bit12
// CCLK_96M_HCLK_48M_PCLK_24M
// CCLK:10.417ns   PCLK:41.667ns
.global GPIOB_READ;
 ws2812b_bit02 :
	la  t2,GPIOB_READ2
	lb t1,0(t2)
	ori t1, t1, 0x10
    lui t0,0x80140
	sb	t1,0x30b(t0) // gpio APB_CLOCK
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
    nop // 30*10.417 = 312.51

    la  t2,GPIOB_READ
	lb t1,0(t2)
	andi t1, t1, 0xef
    lui t0,0x80140
	sb	t1,0x30b(t0)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop// 72*10.417 = 750.024
	ret

 ws2812b_bit12 :
	la  t2,GPIOB_READ2
	lb t1,0(t2)
	ori t1, t1, 0x10
    lui t0,0x80140
	sb	t1,0x30b(t0)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop// 72*10.417 = 750.024
    la  t2,GPIOB_READ
	lb t1,0(t2)
	andi t1, t1, 0xef
    lui t0,0x80140
	sb	t1,0x30b(t0)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop// 72*10.417 = 750.024
	ret

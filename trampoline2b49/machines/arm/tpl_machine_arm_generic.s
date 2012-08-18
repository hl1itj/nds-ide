	.text
	.code 32
	.align 4
	.arm

	.global tpl_enable_interrupts_asm
	.global tpl_disable_interrupts_asm

tpl_enable_interrupts_asm:
	mrs r0, spsr
	bic r0, r0, #0b11000000
	msr spsr, r0
	bx  lr

tpl_disable_interrupts_asm:
	mrs r0, cpsr
	orr r0, r0, #0b11000000
	msr cpsr, r0
	mrs r0, spsr /* interrupts remain locked in user space */
	orr r0, r0, #0b11000000 
	msr spsr, r0
	bx lr
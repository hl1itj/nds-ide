	.text
	.arm
	.align 4
	.global tpl_enable_interrupts
	.global tpl_disable_interrupts
	.type tpl_enable_interrupts STT_FUNC
	.type tpl_disable_interrupts STT_FUNC

tpl_enable_interrupts:
	mrs r0, cpsr
	bic r0, r0, #0b11000000
	msr cpsr, r0
	bx  lr

tpl_disable_interrupts:
	mrs r0, cpsr
	orr r0, r0, #0b11000000
	msr cpsr, r0
	bx lr
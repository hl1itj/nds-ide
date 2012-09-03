/**
 * @file tpl_irq.S
 *
 * @section descr File description
 *
 * IRQ handling.
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Copyright ESEO for function and data structures documentation and ARM port
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_asm_definitions.h"

#define OS_START_SEC_CODE
#include "tpl_as_memmap.h"

	.text
	.arm
	.align 4
	.global tpl_switch_context
	.type tpl_switch_context STT_FUNC
tpl_switch_context:
	/* if old_context is null, skip a save sequence */
	cmp r0, #0
	beq _restore_switch_context
	
	/* start saving */
	/* r0 = old_context->context */
	ldr r0, [r0]
	/* save registers into old context's stack */
	stmdb sp!, {r2}
	/* r2 = old_context->r3 addr */
	add r2, r0, #(4 * 3)
	/* save old context's r3-r12 registers */
	stmia r2!, {r3-r12}
	/* load saved r2 registers */
	ldmia sp!, {r2}
	/* save old context's r0-r2 registers */
	stmia r0, {r0-r2}
	/* r0 = old_context->sp addr */	
	add r2, r0, #(4 * 13)
	stmia r2!, {sp, lr}
	/* r3 = set old context's pc for exit this function */
	adr r3, _exit_switch_context + 4
	mrs r4, cpsr
	/* save old contect's pc & cpsr */
	stmia r2!, {r3, r4}

_restore_switch_context:	
	/* start restoring */
	/* r1 = running_context->context */
	ldr r1, [r1]
	/* r0 = running_context->r4 addr */
	add r0, r1, #(4 * 4)
	ldmia r0!, {r4-r12, sp, lr}
	/* lr = running_context's pc      *
	 * r4 = running_context->psr addr */
	ldmia r0!, {r3}
	sub r3, r3, #4
	/* push pc value into stack */
	stmdb sp!, {r3}
	/* save registers to protect the values */
	stmdb sp!, {r4}
	/* r5 = running_context->psr */
	ldr r4, [r0]
	/* recovered cpsr */
	msr cpsr, r4
	/* recover running_context's r0-r3 */
	ldmia r1, {r0-r3}

	/* enable interrupt */
	mov r4, #0x4000001
	str r4, [r4, #0x207]
	
	/* recover saved running_context's r4-r5 from sp */
	ldmia sp!, {r4, pc}
	
_exit_switch_context:	
	bx lr

	.align 4
	.extern tpl_tick_timer0_overflow_check_reschedule
	.global tpl_tick_timer0_overflow
tpl_tick_timer0_overflow:	
	/*   irq_sp     sys_sp
	 * .--------. .--------.  <- top
	 * |spsr_irq| |  cpsr  |
         * +--------+ +--------+
	 * |   IME  | | lr_irq |
         * +--------+ +--------+
	 * |REG_BASE| |        |
         * +--------+ +--------+
	 * | lr_irq | |        |
         * +--------+ +--------+
	 * |   r0   | |        |
         * +--------+ +--------+
	 * |   r1   | |        |
         * +--------+ +--------+
	 * |   r2   | |        |
         * +--------+ +--------+
	 * |   r3   | |        |
         * +--------+ +--------+
	 * |  r12   | |        |
         * +--------+ +--------+
	 * | pc_sys | |        |
         * +--------+ +--------+
	 */
	/* save sequence */
	stmdb sp!, {lr}
	bl tpl_tick_timer0_overflow_check_reschedule
	ldmia sp!, {lr}
	cmp r0, #NEED_RESCHEDULING
	bxne lr

	ldr r0, =tpl_kern
	ldrb r1, [r0, #TPL_KERN_OFFSET_NEED_SWITCH]
	cmp r2, #NO_NEED_SWITCH
	bxeq lr

	// roll back to when int occured
	pop {r2, lr}
	msr cpsr, r2

	/* irq mode - arm */
	ldr r1, [r0, #TPL_KERN_OFFSET_S_OLD]
	ldr r3, [r1]

	cmp r3, #0
	beq _restore_switch_context_from_it
	
	/* r1 = context->r[4] */
	add r1, r3, #(4 * 4)
	/* save r4-r11, sp, lr registers 
	 * to be saved registers : r0-r3, r12, pc, psr */
	stmia r1!, {r4-r11}
	add r1, r1, #4
	stmia r1, {sp, lr}^
	
	/* load spsr, ime, reg_base, lr_irq */
	ldmia sp!, {r4-r7}

	/* save psr register 
	 * to be saved registers : r0-r3, r12, pc */
	str r4, [r1, #8]
	
	/* pop r0-r3, r12, lr(pc_sys) */
	ldmia sp!, {r4-r7, r12, lr}
	/* save r0-r3 register 
	 * to be saved registers : r12, pc */
	stmia r3!, {r4-r7}
	/* r1 = context->r[12] */
	add r1, r3, #(4 * 12)
	/* save r12 register 
	 * to be saved registers : pc */
	stmia r1, {r12}
	/* save pc register */
	str lr, [r1, #(4 * 3)]
	
_restore_switch_context_from_it:	
	/* restore sequence */
	ldr r1, [r0, #TPL_KERN_OFFSET_S_RUNNING]
	ldr r0, [r1]
	
	/* load pc */
	ldr r1, [r0, #(4 * 15)]
	sub lr, r1, #4

	/* load psr */
	ldr r3, [r0, #(4 * 16)]
	msr spsr, r3

	/* disable processor interrupt */
	mrs r3, cpsr
	orr r3, r3, #0xc0
	msr cpsr, r3
	
	/* enable nds interrupt */
	mov r3, #0x4000001
	str r3, [r3, #0x207]

	ldmia r0, {r0-r14}^
	
	movs pc, lr
	
#define OS_STOP_SEC_CODE
#include "tpl_as_memmap.h"

#define OS_START_LTORG
#include "tpl_as_memmap.h"
#define OS_STOP_LTORG
#include "tpl_as_memmap.h"

/* End of file tpl_irq.S */
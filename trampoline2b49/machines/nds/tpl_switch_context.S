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
	
	/* start saving
	 * Free to use r0 register
	 */
	/* r0 = old_context->context */
	ldr r0, [r0]
	/* pass storing if old_context->context is null */
	cmp r0, #0
	beq _restore_switch_context
	
	/* save registers into old context's stack */
	push {r2}
	/* r2 = old_context->r3 addr */
	add r2, r0, #(4 * 3)
	/* save old context's r3-r12 registers */

	stmia r2, {r3-r12}
	/* load saved r2 registers */
	pop {r2}
	/* save old context's r2 registers */
	str r2, [r0, #(4 * 2)]
	/* r2 = old_context->r13 */
	add r2, r0, #(4 * 13)
	/* r3 = set old context's pc for exit this function */
	adr r3, _exit_switch_context + 4
	mrs r4, cpsr
	/* save old contect's sp, lr, pc & cpsr */
	stmia r2!, {sp, lr}
	stmia r2!, {r3-r4}

_restore_switch_context:
	ldr r0, =tpl_kern
	/* change mode to irq */
	mrseq r4, cpsr
	bic r4, r4, #0xdf
	orr r4, r4, #0x92
	msr cpsr, r4

	b _restore_switch_context_from_it
	
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
	 * |pc_sys+4| |        |
         * +--------+ +--------+
	 */
	/* reset need switch var */
	ldr r0, =tpl_kern
	mov r1, #NO_NEED_SWITCH
	strb r1, [r0, #TPL_KERN_OFFSET_NEED_SWITCH]
	
	/* save sequence */
	push {r0, lr}
	bl tpl_tick_timer0_overflow_check_reschedule
	bl tpl_schedule_from_running
	pop {r0, lr}

	ldrb r1, [r0, #TPL_KERN_OFFSET_NEED_SWITCH]
	cmp r1, #NO_NEED_SWITCH
	bxeq lr
	
	/* roll back to when int occurs */
	pop {r2, lr}
	msr cpsr, r2

	/* irq mode start */
	ldr r1, [r0, #TPL_KERN_OFFSET_S_OLD]
	ldr r3, [r1]

	/* if context == NULL, skip saving */
	cmp r3, #0
	addeq sp, sp, #(4 * 10)
	beq _restore_switch_context_from_it

	/* need saving?? */
	ldrb r1, [r0, #TPL_KERN_OFFSET_NEED_SWITCH]
	tst r1, #NEED_SAVE
	addeq sp, sp, #(4 * 10)
	beq _restore_switch_context_from_it
	
	/* r1 = context->r[4] */
	add r1, r3, #(4 * 4)
	/* save r4-r11, sp, lr registers 
	 * to be saved registers : r0-r3, r12, pc, psr */
	stmia r1!, {r4-r11}
	add r1, r1, #4
	stmia r1, {sp, lr}^
	
	/* load spsr, ime, reg_base, lr_irq */
	pop {r4-r7}

	/* save psr register 
	 * to be saved registers : r0-r3, r12, pc
	 * r1 = r13
	 */
	str r4, [r1, #(4 * 3)]
	
	/* pop r0-r3, r12, lr(pc_sys) */
	pop {r4-r7, r12, lr}
	/* save r0-r3 register 
	 * to be saved registers : r12, pc */
	stmia r3!, {r4-r7}
	/* r3 = context->r[4]
	 * r1 = context->r[12]
	 */
	add r1, r3, #(4 * 8)
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
	/* if pc reg value indicates in thumbmode, set thumb mode to psr */
	tst lr, #1
	orrne r3, r3, #0x20
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

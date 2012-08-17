/*
 * assemfunc.s
 *
 *  Created on: 2010. 5. 15.
 *  Readed on : 2010. 5. 18.
 *      Author: gamja9e
 */

	.section ".text"
    .align  4
    .arm

   .global vPortDisableInterruptsFromThumb
   .global vPortEnableInterruptsFromThumb
   .global portRESTORE_CONTEXT_FIRST
   .global portRESTORE_CONTEXT
   .global vPreemptiveTick


vPortDisableInterruptsFromThumb:
	STMDB	SP!, {R0}
	MRS	R0, CPSR
	ORR	R0, R0, #0xC0
	MSR	CPSR, R0
	LDMIA	SP!, {R0}
	BX		R14

vPortEnableInterruptsFromThumb:
	STMDB	SP!, {R0}
	MRS	R0, CPSR
	BIC	R0, R0, #0xC0
	MSR	CPSR, R0
	LDMIA	SP!, {R0}
	BX		R14

	.extern pxCurrentTCB
	.extern ulCriticalNesting
portRESTORE_CONTEXT_FIRST:
    // 인터럽트 금지
	MOV R3, #0x4000000
    STR R3, [ R3, #0x208 ]
    // ISR 모드로 변경
	MRS R3, CPSR
    BIC R3, R3, #0xDF
    ORR R3, R3, #0x92
    MSR CPSR, R3
portRESTORE_CONTEXT:
	LDR		R0, =pxCurrentTCB
	LDR		R0, [R0]
	LDR		r0, [R0]
	// ulCriticalNesting 읽어서 저장
	LDR		R1, =ulCriticalNesting
	LDMFD	r0!, {R2}
	STR		R2, [R1]
	LDMFD	r0!, {R2}
	MSR	SPSR, R2
	// PC 값을 읽어서 4를 뺀후 값 Lr에 저장
	LDR		r1, [R0, #60]
	sub		lr, r1, #4
	// 인터럽트 가능
	MOV R3, #0x4000001
    STR R3, [ R3, #0x207 ]
    // lr, pc를 제외한 레지스터 기록
	LDMFD	r0, {R0-R14}^
	// pc값 적용
	movs	pc, lr

	.extern vTaskIncrementTick
	.extern vTaskSwitchContext
vPreemptiveTick:
    MOV R3, #0x4000000
    STR R3, [ R3, #0x208 ]
    // 태스크의 SPSR과 LR을 스택에서 뺌
    pop {r2, lr}
    // 컨텍스트를 저장할 위치의 최 상단 이동
    SUB R0, SP, #72
    // System 모드 r4-r14 레지스터를 저장한다.
    ADD R3, R0, #24
    STMIA R3, { R4-R14 }^
    // 인터럽트 당시의 모드(ISR)로 전환
    MSR CPSR, R2
	// {spsr, IME, REG_BASE lr_irq} 빼내서 SPSR 저장
    LDMFD SP!, { R5-R6, R7, R8 }
    STR R5, [ R0, #4 ]
    // {r0-r3, r12, lr} 빼내서 복구
    LDMFD SP!, { R5-R8, R12, LR }
    ADD	  r3, r0, #8
    STMIA R3, { R5-R8 }
    STR	  r12, [ r0, #56 ]
    // LR은  태스크의 PC에 저장
    STR   LR, [ R0, #68 ]
	// ulCriticalNesting 저장
    LDR R5, =ulCriticalNesting
    LDR R5, [R5]
    STR R5, [R0]
	// 컨텍스트 저장위치 최 상위 위치 주소를  pxCurrentTCB에 저장
    LDR R5, =pxCurrentTCB
    LDR R5, [R5]
    STR R0, [R5]

	BL vTaskIncrementTick
	BL vTaskSwitchContext

	// 컨텍스트 복원 시작
	B portRESTORE_CONTEXT

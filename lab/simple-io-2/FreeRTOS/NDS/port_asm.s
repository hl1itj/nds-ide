//
// port_asm.s
//
//  Created on: 2010. 5. 15.
//  Readed on : 2010. 5. 18.
//  Author    : Minsuk Lee (rewrite from Dukjoon Jeon's version)

    .section ".text"
    .align  4
    .arm

    .global vPortDisableInterruptsFromThumb
    .global vPortEnableInterruptsFromThumb
    .global portRESTORE_CONTEXT_FIRST
    .global portRESTORE_CONTEXT
    .global vPreemptiveTick
    .global vPortYIELD

    .type vPortDisableInterruptsFromThumb STT_FUNC
    .type vPortEnableInterruptsFromThumb STT_FUNC
    .type portRESTORE_CONTEXT_FIRST STT_FUNC
    .type portRESTORE_CONTEXT STT_FUNC
    .type vPreemptiveTick STT_FUNC
    .type vPortYIELD STT_FUNC

vPortDisableInterruptsFromThumb:
    STMDB   SP!, {R0}
    MRS     R0, CPSR
    ORR     R0, R0, #0xC0
    MSR     CPSR, R0
    LDMIA   SP!, {R0}
    BX      R14

vPortEnableInterruptsFromThumb:
    STMDB   SP!, {R0}
    MRS     R0, CPSR
    BIC     R0, R0, #0xC0
    MSR     CPSR, R0
    LDMIA   SP!, {R0}
    BX      R14

    .extern pxCurrentTCB
    .extern ulCriticalNesting

portRESTORE_CONTEXT_FIRST:
    MOV     R3, #0x4000000
    STR     R3, [ R3, #0x208 ]      // disable interrupt *(0x4000208) = 0x4000000, LSB = 0

    MRS     R3, CPSR                // imitate irq mode
    BIC     R3, R3, #0xDF           // IRQ, FIRQ Enable, Mode = 0
    ORR     R3, R3, #0x92           // IRQ Disable, Mode = irq mode
    MSR     CPSR, R3

portRESTORE_CONTEXT:
    LDR     R0, =pxCurrentTCB       // R0 = pxCurrentTCB 포인터의 주소
    LDR     R0, [R0]                // R0 = Current TCB 구조체의 주소
    LDR     R0, [R0]                // R0 = Current TCB 구조체 첫 field pxTopOfStack

    LDR     R1, =ulCriticalNesting  // R1은 ulCritialNesting 변수의 주소
    LDMFD   R0!, {R2}               // pop top of stack (stack에 저장된 ulCriticalNesting) ==> R2
    STR     R2, [R1]                // stack의 ulCritialNesting을 global 변수로 복원

    LDMFD   R0!, {R2}               // pop SPSR ==> R2, R0는 stack의 R0 위치를 가리킴, 'FD' = 이후 증가 IA
    MSR     SPSR, R2                // SPSR = R2  <== interrupt 걸리기전 SPSR 값으로 복귀 (user thread)

    LDR     R1, [R0, #60]           // stack의 R0-60위치(즉, R15=PC 위치)에서 값 ==> R1
    SUB     LR, R1, #4              // R1(Stack에 저장된 PC 값)에서 4를 뺀값을 LR_irq에 저장
                                    // (Interrupt에 의한 것이므로 -4)

    MOV     R3, #0x4000001          // LSB=1 means NDS Interrupt Enable
    STR     R3, [R3, #0x207]        // *(0x4000208, NDS-Interrupt Mask Enable) = 0x4000001

    LDMFD   R0, {R0-R14}^           // stack의 R0 위치에서부터, R0-R14를 pop (system mode SP, LR)

    MOVS    PC, LR                  // Return to the Context (with LR_irq), SPSR -> CPSR

    .extern vTaskIncrementTick
    .extern vTaskSwitchContext

vPreemptiveTick:
    MOV     R3, #0x4000000          // LSB = 0 ==> NDS Interrupt Disable
    STR     R3, [R3, #0x208]        // *(0x4000208, NDS-Interrupt Mask Enable) = 0x4000000

    POP     {R2, LR}                // 태스크의 SPSR과 LR을 스택에서 R2, LR에 다시 Load
                                    // Nothing on Stack now

    SUB     R0, SP, #72             // R0 = 컨텍스트를 저장할 위치의 최하단 (ulCriticalNesting position)
    ADD     R3, R0, #24             // R3 = 6칸 위 (즉, R4 위치)
    STMIA   R3, {R4-R14}^           // System 모드 R4-R14 레지스터를 저장한다. - 즉, LR, SP는 user thread 용 !

    MSR     CPSR, R2                // 인터럽트 당시의 모드(ISR)로 전환 - 즉 SP_irq ! 를 사용
    LDMFD   SP!, {R5-R6, R7, R8}    // NDS-BIOS가 저장한 {spsr, IME, REG_BASE, lr_irq}를 R5-R8로 빼냄
    STR     R5, [R0, #4]            // NDS-BIOS 상태의 SPSR 값을 Context 저장 stack의 SPSR 위치에 저장

    LDMFD   SP!, {R5-R8, R12, LR}   // {r0-r3, r12, lr} 빼내서 복구 : SP_irq는 interrupt전 원래 값으로 원위치됨

    ADD     R3, R0, #8              // R3 = context stack의 R0 위치
    STMIA   R3, { R5-R8 }           // 위에서 빼낸 R0-R3를 context stack의 해당 위치에 저장
    STR     R12, [R0, #56]          // 위에서 빼낸 R12를 context stack의 해당 위치에 저장
    STR     LR, [R0, #68]           // 위에서 빼낸 LR을 context stack의 해당 위치에 저장

    LDR     R5, =ulCriticalNesting
    LDR     R5, [R5]                // R5 = global 변수 ulCriticalNesting
    STR     R5, [R0]                // *R0 (ulCriticalCritical 위치)에 R5 저장

    LDR     R5, =pxCurrentTCB
    LDR     R5, [R5]                // R5 = &pxCurrentTCB == &(pxCurrentPCB->pxTopOfStack)
    STR     R0, [R5]                // pxCurrentTCB.pxTopOfStack = R0(ulCriticalCritical 위치) 저장

    BL      vTaskIncrementTick      // Do the timer Stuff
    BL      vTaskSwitchContext      // Select the highest priotity task to run
    B       portRESTORE_CONTEXT     // Restore Context and return to the next Task

// ---------------------------------------------------------------------------------------

vPortYIELD:
    // Disable Interrupt before Stack operation.
    MOV     R0, #0x4000000
    STR     R0, [R0, #0x208]    // disable interrupt *(0x4000208) = 0x4000000, LSB = 0

    // we dont need to save R0, because it's used as scratch pad
    // STR     R0, [SP, #-64]   // Save R0

    SUB     R0, SP, #60         // R0 in R1 position
    STMIA   R0, { R1-R14 }      // Save R1~R12, SP, LR (system mode SP, LR)

    ADR     R1, exit + 4
    STR     R1, [SP, #-4]       // exit 위치를 PC 위치에 저장

    MRS     R0, CPSR
    STR     R0, [SP, #-68]      // Save CPSR of current thread

    SUB     R0, SP, #72
    LDR     R1, =ulCriticalNesting
    LDR     R1, [R1]
    STR     R1, [R0]            // Save ulCriticalNesting

    LDR     R1, =pxCurrentTCB
    LDR     R1, [R1]
    STR     R0, [R1]            // Save Top of Stack

    MRS     R0, CPSR            // imitate irq mode
    BIC     R0, R0, #0xDF
    ORR     R0, R0, #0x92       // irq disable, mode irq
    MSR     CPSR, R0

    BL      vTaskSwitchContext  // Select Next Task
    B       portRESTORE_CONTEXT // Jump to it
exit:
    BX      LR

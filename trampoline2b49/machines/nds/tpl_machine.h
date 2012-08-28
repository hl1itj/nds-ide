/**
 * @file tpl_machine.h
 *
 * @section descr File description
 *
 * Trampoline core exported definitions
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
#ifndef TPL_MACHINE_H
#define TPL_MACHINE_H

#include "tpl_os_std_types.h"
#include "tpl_os_internal_types.h"
#include "tpl_os_custom_types.h"

/**
 * ARM internal registers symbolic names
 */
typedef enum
{
  armreg_r0 = 0,
    armreg_a1 = 0,
  armreg_r1 = 1,
    armreg_a2 = 1,
  armreg_r2 = 2,
    armreg_a3 = 2,
  armreg_r3 = 3,
    armreg_a4 = 3,
  armreg_r4 = 4,
    armreg_v1 = 4,
  armreg_r5 = 5,
    armreg_v2 = 5,
  armreg_r6 = 6,
    armreg_v3 = 6,
  armreg_r7 = 7,
    armreg_v4 = 7,
  armreg_r8 = 8,
    armreg_v5 = 8,
  armreg_r9 = 9,
    armreg_v6 = 9,
    armreg_rfp = 9,
  armreg_r10 = 10,
    armreg_v7 = 10,
    armreg_sl = 10,
  armreg_r11 = 11,
    armreg_v8 = 11,
    armreg_fp = 11,
  armreg_r12 = 12,
    armreg_ip = 12,
  armreg_r13 = 13,
    armreg_sp = 13,
  armreg_r14 = 14,
    armreg_lr = 14,
  armreg_r15 = 15,
    armreg_pc = 15
} tpl_arm_register_names;

/**
 * ARM core registers
 */
struct ARM_CONTEXT {
  u32 r[16];
  u32 psr;
};

/**
 * ARM generic platform context
 */
typedef struct ARM_CONTEXT arm_context;

/**
 * default size of an element in a stack
 */
typedef u32 tpl_stack_word;

/**
 * type of a stack size
 */
typedef u32 tpl_stack_size;

/**
 * Stack definition
 */
struct TPL_STACK {
    tpl_stack_word  *stack_zone;
    tpl_stack_size  stack_size;
};

/**
 * Stack definition
 */
typedef struct TPL_STACK tpl_stack;

extern VAR (arm_context, OS_VAR) idle_task_context;

/** 
 * Defines the context block of the task "idle"
 */
#define IDLE_CONTEXT &idle_task_context

#define SIZE_OF_IDLE_STACK 200

extern VAR(tpl_stack_word, OS_VAR) idle_stack[SIZE_OF_IDLE_STACK/sizeof(tpl_stack_word)];

/**
 * Defines the stack (void) of the task "idle"
 */
#define IDLE_STACK {idle_stack,SIZE_OF_IDLE_STACK}


/**
 * Defines the entry point of the idle task
 */
#define IDLE_ENTRY tpl_sleep

/**
 * Gives the ARM processor mode the normal user tasks
 * will run into. The value can be :
 * - 0x10 : unprivileged
 * - 0x1F : privileged
 */
#define USER_TASKS_ARM_MODE 0x1F

typedef struct ARM_CONTEXT *tpl_context;

extern struct ARM_CONTEXT idle_task_context;

#endif /* TPL_MACHINE_H */

/* End of file tpl_machine.h */

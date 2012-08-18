/**
 * @file nxt_tpl_stubs.c
 *
 * @section descr File description
 *
 * SIMTEC platform specific routines and variables
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
#include "tpl_machine.h"
#include "tpl_machine_interface.h"
#include "tpl_os_definitions.h"
#include "tpl_os_kernel.h"

extern void tpl_init_machine_generic (void);

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

void tpl_init_machine(void)
{
    tpl_init_machine_generic();
}

void tpl_shutdown(void)
{
    while(1) {
    }
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/* End of file nxt_tpl_stubs.c */

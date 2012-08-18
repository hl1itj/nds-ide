/**
 * @file nds_tpl_sleep.S
 *
 * @section descr File description
 *
 * Implementation of tpl_sleep function, called by idle task
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

#define OS_START_SEC_CODE
#include "tpl_as_memmap.h"

/* We want tpl_sleep not to use stack. Writing in assembler make us able to
 * prevent this. */
.global tpl_sleep
tpl_sleep:
  /* falls into infinite loop */
    b .

#define OS_STOP_SEC_CODE
#include "tpl_as_memmap.h"

#define OS_START_LTORG
#include "tpl_as_memmap.h"
#define OS_STOP_LTORG
#include "tpl_as_memmap.h"

/* End of file nxt_tpl_sleep.S */

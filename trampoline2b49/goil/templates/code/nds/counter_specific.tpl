#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(tpl_status, OS_CODE) $TICK_FUNC$$IT_SOURCE$_check_reschedule()
{
  tpl_status  need_rescheduling = NO_SPECIAL_CODE;
$COUNTER_LIST$

  return need_rescheduling;

/*
  if (need_rescheduling == NEED_RESCHEDULING)
  {
    tpl_schedule_from_running();
#ifndef WITH_SYSTEM_CALL
    if (tpl_kern.need_switch != NO_NEED_SWITCH) {
      tpl_switch_context_from_it(
        &(tpl_kern.s_old->context),
        &(tpl_kern.s_running->context)
      );
    }
#endif
  }
*/
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

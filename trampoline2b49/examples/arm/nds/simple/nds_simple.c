/**
 * @file nxt_simple.c
 *
 * @section desc File description
 *
 * simple application on the Lego Mindstorm NXT2.0
 *
 * @section copyright Copyright
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_os.h"

#define APP_Task_task1_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(int, OS_APPL_CODE) main(void)
{   
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

#define APP_Task_task1_STOP_SEC_CODE
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(int, OS_VAR) switch_count = 0;
VAR(TaskType, OS_VAR) new_task;
#define OS_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

#define APP_Task_task1_START_SEC_CONST_32BIT
#include "tpl_memmap.h"
CONST(int, AUTOMATIC) toto = 0;
#define APP_Task_task1_STOP_SEC_CONST_32BIT
#include "tpl_memmap.h"

#define APP_Task_task1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(int, AUTOMATIC) titi = 0;
#define APP_Task_task1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

#define APP_Task_task1_START_SEC_VAR_32BIT
#include "tpl_memmap.h"
VAR(u32, AUTOMATIC) motor2speed = 0;
#define APP_Task_task1_STOP_SEC_VAR_32BIT
#include "tpl_memmap.h"

DeclareTask(task1);
DeclareTask(task5);

#define APP_Task_task1_START_SEC_CODE
#include "tpl_memmap.h"

TASK(task1)
{
    while(1){
    };
    
    ChainTask(task1);
}

#define APP_Task_task1_STOP_SEC_CODE
#include "tpl_memmap.h"


#define APP_Task_task5_START_SEC_CODE
#include "tpl_memmap.h"

TASK(task5)
{   
    TerminateTask();
}

#define APP_Task_task5_STOP_SEC_CODE
#include "tpl_memmap.h"

/* End of file nxt_simple.c */

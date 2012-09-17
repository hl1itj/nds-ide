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

#include <nds.h>

volatile int frame = 0;

void Vblank()
{
	frame++;
}

FUNC(int, OS_APPL_CODE) main(void)
{   
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

#define APP_Task_task1_STOP_SEC_CODE
#include "tpl_memmap.h"

DeclareTask(task1);

#define APP_Task_task1_START_SEC_CODE
#include "tpl_memmap.h"

TASK(task1)
{
	touchPosition touchXY;

	irqSet(IRQ_VBLANK, Vblank);

	consoleDemoInit();

	iprintf("      Hello DS dev'rs\n");
	iprintf("     \x1b[32mwww.devkitpro.org\n");
	iprintf("   \x1b[32;1mwww.drunkencoders.com\x1b[39m");
 
	while(1) {
	
		swiWaitForVBlank();
		touchRead(&touchXY);

		// print at using ansi escape sequence \x1b[line;columnH 
		iprintf("\x1b[10;0HFrame = %d",frame);
		iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);		
	
	}

	TerminateTask();
}

#define APP_Task_task1_STOP_SEC_CODE
#include "tpl_memmap.h"

/* End of file nxt_simple.c */

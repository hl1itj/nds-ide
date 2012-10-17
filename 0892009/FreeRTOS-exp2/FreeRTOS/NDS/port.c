/*
    FreeRTOS V6.0.4 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS eBook                                  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/


/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM7 port.
 *
 * Components that can be compiled to either ARM or THUMB mode are
 * contained in this file.  The ISR routines, which can only be compiled
 * to ARM mode are contained in portISR.c.
 *----------------------------------------------------------*/

#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <nds.h>

#define portINITIAL_SPSR                ( ( portSTACK_TYPE ) 0x3f ) /* System mode, THUMB mode, interrupts enabled. */
#define portTHUMB_MODE_BIT              ( ( portSTACK_TYPE ) 0x20 )
#define portINSTRUCTION_SIZE            ( ( portSTACK_TYPE ) 4 )
#define portNO_CRITICAL_SECTION_NESTING ( ( portSTACK_TYPE ) 0 )

/*-----------------------------------------------------------*/

static void prvSetupTimerInterrupt( void );
static void prvSetupInterrupt( void );
extern void vPortISRStartFirstTask( void );

/*-----------------------------------------------------------*/

portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
    portSTACK_TYPE *pxOriginalTOS;
    pxOriginalTOS = pxTopOfStack;

    *pxTopOfStack = ( portSTACK_TYPE ) pxCode + portINSTRUCTION_SIZE;
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode + portINSTRUCTION_SIZE;           /* R14 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) pxOriginalTOS; /* Stack used when task starts goes in R13. */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x12121212;  /* R12 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x11111111;  /* R11 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x10101010;  /* R10 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x09090909;  /* R9 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x08080808;  /* R8 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x07070707;  /* R7 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x06060606;  /* R6 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x05050505;  /* R5 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x04040404;  /* R4 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x03030303;  /* R3 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x02020202;  /* R2 */
    pxTopOfStack--;
    *pxTopOfStack = ( portSTACK_TYPE ) 0x01010101;  /* R1 */
    pxTopOfStack--;

    *pxTopOfStack = ( portSTACK_TYPE ) pvParameters; /* R0 */
    pxTopOfStack--;

    *pxTopOfStack = ( portSTACK_TYPE ) portINITIAL_SPSR;

    // 무조건 적인 thumb 모드로 인하여 필요없음
#if 0
    if( ( ( unsigned long ) pxCode & 0x01UL ) != 0x00 )
    {
        *pxTopOfStack |= portTHUMB_MODE_BIT;
    }
#endif
    pxTopOfStack--;
    *pxTopOfStack = portNO_CRITICAL_SECTION_NESTING;

    return pxTopOfStack;
}

portBASE_TYPE xPortStartScheduler( void )
{
    prvSetupTimerInterrupt();
    prvSetupInterrupt();
    vPortISRStartFirstTask();
    return 0;
}

void vPortEndScheduler( void ) {}

extern void vPreemptiveTick( void );
extern void vNonPreemptiveTick( void ) __attribute__ ((interrupt ("IRQ")));

static void prvSetupTimerInterrupt( void )
{
    vPortDisableInterruptsFromThumb();

    TIMER0_DATA = TIMER_FREQ_256(1000);
    TIMER0_CR = TIMER_ENABLE | TIMER_IRQ_REQ | TIMER_DIV_256;
    irqSet(IRQ_TIMER0, vPreemptiveTick);
    irqEnable(IRQ_TIMER0);
}

extern void vInterruptHandler( void ) __attribute__((naked));
static void prvSetupInterrupt( void )
{
    // Do Nothing, NDS runs IRQ=on already
}

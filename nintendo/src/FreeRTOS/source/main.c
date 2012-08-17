/*
 * main.c
 *
 *  Created on: 2011. 9. 24.
 *      Author: Minsuk Lee
 */

#include <stdio.h>          // C-Standard Header
#include <time.h>
#include <stdlib.h>

#include <FreeRTOS.h>       // Free RTOS Headers
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <nds.h>            // NDS / Sevencore Board Headers
#include <sevencore_io.h>

#include "card_spi.h"		// Debugging Headers
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Test_Task_1, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Test_Task_1,
			(const signed char * const)"Test_Task_1",
			2048,
			(void *)NULL,
			tskIDLE_PRIORITY + 1,
			NULL);

	vTaskStartScheduler();		// Never returns
	while(1)
		;
	return 0;
}

void
InitDebug(void)
{
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

static
portTASK_FUNCTION(Test_Task_1, pvParameters )
{
	unsigned int cnt = 0;

	while(1)
	{
		cnt++;
		iprintf("Test_Task_1 is running, count : %d\n", cnt);
		vTaskDelay(1000);
	}
}



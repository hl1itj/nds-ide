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

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Sample_Task_1, pvParameters);
static portTASK_FUNCTION(Sample_Task_2, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Sample_Task_1,
					     (const signed char * const)"Sample_Task_1",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);
	xTaskCreate(Sample_Task_2,
					     (const signed char * const)"Sample_Task_2",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
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
portTASK_FUNCTION(Sample_Task_1, pvParameters)
{
	u8 barled = 0;
	int i;

	while (1) {
		printf("1");
		barled = ~barled;
		writeb_virtual_io(BARLED1, barled);

		for (i = 0; i < 100000; i++)
			barled = ~barled;

		vTaskDelay(MSEC2TICK(500));
	}
}

static
portTASK_FUNCTION(Sample_Task_2, pvParameters)
{
	u8 barled = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	int i;

	while (1) {
		printf("2");
		barled = ~barled;
		writeb_virtual_io(BARLED2, barled);

		for (i = 0; i < 100000; i++)
			barled = ~barled;

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}
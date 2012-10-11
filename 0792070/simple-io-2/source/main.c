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

#define END 1
#define NOTEND 0

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int main(void) {
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Homework_1, (const signed char * const)"Homework_1", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 5, NULL);
	xTaskCreate(Homework_2, (const signed char * const)"Homework_2", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 2, NULL);
	vTaskStartScheduler();		// Never returns
	while (1)
		;
	return 0;
}

void InitDebug(void) {
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

static
portTASK_FUNCTION(Homework_1, pvParameters) {
	u16 barnum = 0x01;
	u16 sw;

	u8 keypressed = FALSE;

	writeb_virtual_io(BARLED1, barnum);

	while (1) {
		sw = NDS_SWITCH();

		if (!keypressed && (sw & KEY_L)) {
			if (barnum == 0x80)
				continue;
			barnum = barnum << 1;
			printf("L");
			keypressed = TRUE;
		} else if (!keypressed && (sw & KEY_R)) {
			if (barnum == 0x01)
				continue;
			barnum = barnum >> 1;
			printf("R");
			keypressed = TRUE;
		}

		writeb_virtual_io(BARLED1, barnum);

		if (keypressed && (!(sw & KEY_L) && !(sw & KEY_R)))
			keypressed = FALSE;

		vTaskDelay(50);
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters) {
	u16 barled = 0x01;
	u8 barledState = NOTEND;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {
		writeb_virtual_io(BARLED2, barled);

		if (barledState == NOTEND) {
			if (barled <= 0x80)
				barled = barled << 1;
			if (barled == 0x80) {
				barledState = END;
			}
		} else if (barledState == END) {
			barled = 0x01;
			barledState = NOTEND;
		}
		printf(".");

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );
	}
}


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

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int main(void) {
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Homework_1, (const signed char * const)"Sample_Task_1", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 1, NULL);
	xTaskCreate(Homework_2, (const signed char * const)"Sample_Task_2", 2048,
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
	u16 sw;
	u8 left_key_pressed = FALSE;
	u8 right_key_pressed = FALSE;
	u8 key_number = 1;
	u32 BARLED = 0x408;

	writeb_virtual_io(BARLED, key_number);

	while (1) {
		sw = NDS_SWITCH();
		if ((sw & KEY_L) && (left_key_pressed == FALSE)) {
			left_key_pressed = TRUE;

			if (key_number < 128) {
				key_number = key_number * 2;
				writeb_virtual_io(BARLED, key_number);
			}
		}

		if ((left_key_pressed == TRUE) && (!(sw & KEY_L)))
			left_key_pressed = FALSE;

		if ((sw & KEY_R) && (right_key_pressed == FALSE)) {
			right_key_pressed = TRUE;

			if (key_number > 1) {
				key_number = key_number / 2;
				writeb_virtual_io(BARLED, key_number);
			}
		}

		if ((right_key_pressed == TRUE) && (!(sw & KEY_R)))
			right_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START) {
			writeb_virtual_io(BARLED, 0);
			break;
		}
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed

}

static
portTASK_FUNCTION(Homework_2, pvParameters) {
	u8 key_pressed = FALSE;
	int key_number = 1;
	portTickType xLastWakeTime = xTaskGetTickCount();

	//writeb_virtual_io(BARLED2, key_number);

	while (1) {
		if (key_pressed == FALSE) {
			key_pressed = TRUE;
			if (key_number < 256) {
				writeb_virtual_io(BARLED2, key_number);
				key_number = key_number * 2;

				if (key_number == 256)
					key_number = 1;
			}
		}
		if (key_pressed == TRUE)
			key_pressed = FALSE;



		if (NDS_SWITCH() & KEY_START) {
			writeb_virtual_io(BARLED2, 0);
			break;
		}

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

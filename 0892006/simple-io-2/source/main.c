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

	xTaskCreate(Homework_1, (const signed char * const)"Homework_1", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 5, NULL);
	xTaskCreate(Homework_2, (const signed char * const)"Homework_2", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 1, NULL);
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

	u8 led_state = 0x80;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();
		//연속입력을 체크.
		if (key_pressed == TRUE && (!(sw & KEY_R)) && (!(sw & KEY_L))) {
			key_pressed = FALSE;
		}
		//R키 입력 체크.
		if (NDS_SWITCH() & KEY_R && key_pressed == FALSE && led_state >= 0x02) {
			led_state = led_state >> 1;
			writeb_virtual_io(BARLED1, led_state);
			key_pressed = TRUE;

		}
		//L키 입력 체크.
		if (NDS_SWITCH() & KEY_L && key_pressed == FALSE && led_state <= 0x40) {
			led_state = led_state << 1;
			writeb_virtual_io(BARLED1, led_state);
			key_pressed = TRUE;
		}

		 vTaskDelay(MSEC2TICK(50));
	}

}

static
portTASK_FUNCTION(Homework_2, pvParameters) {

	u8 led_state = 0x01;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {
		writeb_virtual_io(BARLED2, led_state);
		if (led_state == 0x80)
			led_state = 0x01;
		else
			led_state = led_state << 1;

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );
	}
}

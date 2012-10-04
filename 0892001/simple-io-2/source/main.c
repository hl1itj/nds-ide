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
			(void *)NULL, tskIDLE_PRIORITY + 2, NULL);
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
	u8 led_state = 0x08;
	int key_pressed = 0;
	writeb_virtual_io(BARLED1, led_state);

	while (1) {
		writeb_virtual_io(BARLED1, led_state);
		sw = NDS_SWITCH();

		if ((key_pressed == 0) && (sw & KEY_L) && (led_state <= 0x40)) {
			key_pressed = 1;
			led_state *= 2;
		}

		if ((key_pressed == 0) && (sw & KEY_R) && (led_state >= 0x02)) {
			key_pressed = 1;
			led_state /= 2;
		}

		if ((key_pressed == 1) && (!(sw & KEY_L)) && (!(sw & KEY_R)))
			key_pressed = 0;
		vTaskDelay(MSEC2TICK(50));
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters) {
	u16 sw;
	portTickType xLastWakeTime =xTaskGetTickCount();
	u8 led_state2 = 0x01;

	while (1) {
		writeb_virtual_io(BARLED2, led_state2);
		if(led_state2==0x80){
			led_state2=0x01;
		}
		else{
			led_state2*=2;
		}
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}

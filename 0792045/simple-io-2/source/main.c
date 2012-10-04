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

static portTASK_FUNCTION(HomeWork_1, pvParameters);
static portTASK_FUNCTION(HomeWork_2, pvParameters);

void InitDebug(void);

int main(void) {
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(HomeWork_1, (const signed char * const)"HomeWork_1", 2048,
			(void *)NULL, tskIDLE_PRIORITY + 2, NULL);
	xTaskCreate(HomeWork_2, (const signed char * const)"HomeWork_2", 2048,
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
portTASK_FUNCTION(HomeWork_1, pvParameters) {
	u16 sw;
	u16 led_state = 0x01;

	u8 R_key_pressed = FALSE;
	u8 L_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0x00);
	while (1) {
		sw = NDS_SWITCH();
		if (((R_key_pressed == FALSE) && (sw & KEY_R) && led_state != 0x01)) {
			led_state = led_state >> 1;
			R_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		} else if ((R_key_pressed == TRUE) && !(sw & KEY_R))
			R_key_pressed = FALSE;

		if (((L_key_pressed == FALSE) && (sw & KEY_L) && led_state != 0x80)) {
			led_state = led_state << 1;
			L_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		} else if ((L_key_pressed == TRUE) && !(sw & KEY_L))
			L_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
}

static
portTASK_FUNCTION(HomeWork_2, pvParameters) {
	u8 barled = 0x01;
	u8 state = TRUE; //TRUE 일 경우 왼쪽으로 이동 FALSE 일 경우 오른쪽으로 이동

	portTickType xLastWakeTime = xTaskGetTickCount();
	while (1) {
		if(state == TRUE)
			barled = barled<<1;
		else
			barled = barled>>1;

		if(barled == 0x80)
			state = FALSE;
		else if(barled == 0x01)
			state = TRUE;
		writeb_virtual_io(BARLED2, barled);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime,MSEC2TICK(500));
	}
}

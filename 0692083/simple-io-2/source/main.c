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

#define LED_MIN 0x01
#define LED_MAX 0x80
#define LED_OFF 0x00
#define SHIFT 2

struct Homework_1_State {

	u16 sw;
	u8 led_position;
	u8 led_state;
	u8 left_key_pressed;
	u8 right_key_pressed;
};

struct Homework_2_State {

	u16 sw;
	u8 led_position;
	u8 led_state;
	portTickType xLastWakeTime;

};

///////// HOMEWORK 1 Function //////////////////

void SetOnLeftKeyPressed(struct Homework_1_State *s) {

	s->left_key_pressed = TRUE;
	s->led_state = !s->led_state;
	s->led_position = s->led_position * SHIFT;
	writeb_virtual_io(BARLED1, s->led_position);

}

void SetOnRightKeyPressed(struct Homework_1_State *s) {

	s->right_key_pressed = TRUE;
	s->led_state = !s->led_state;
	s->led_position = s->led_position / SHIFT;
	writeb_virtual_io(BARLED1, s->led_position);
}

///////// HOMEWORK 2 Function //////////////////

void SetOnAutoLeftMove(struct Homework_2_State *s) {

	s->led_state = !s->led_state;
	s->led_position = s->led_position * SHIFT;
	writeb_virtual_io(BARLED2, s->led_position);

}

///////////////////////////////////////////////////////////

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

	struct Homework_1_State s1;

	s1.led_position = LED_MIN;
	s1.led_state = FALSE;
	s1.left_key_pressed = FALSE;
	s1.right_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, s1.led_position);

	while (1) {

		s1.sw = NDS_SWITCH();

		if ((s1.left_key_pressed == FALSE) && (s1.sw & KEY_L)) {

			if (s1.led_position < LED_MAX) { // led_state < 2E7 and KEY_LEFT

				SetOnLeftKeyPressed(&s1);

			}

		}

		if ((s1.left_key_pressed == TRUE) && (!(s1.sw & KEY_L))) {

			s1.left_key_pressed = FALSE;

		}

		if ((s1.right_key_pressed == FALSE) && (s1.sw & KEY_R)) {

			if (s1.led_position > LED_MIN) { // led_state < 2E0 and KEY_RIGHT

				SetOnRightKeyPressed(&s1);
			}

		}

		if ((s1.right_key_pressed == TRUE) && (!(s1.sw & KEY_R))) {

			s1.right_key_pressed = FALSE;
		}

		vTaskDelay(MSEC2TICK(50));

	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters) {

	struct Homework_2_State s2;

	s2.led_position = LED_MIN;
	s2.xLastWakeTime = xTaskGetTickCount();

	writeb_virtual_io(BARLED2, s2.led_position);

	while (1) {

		if (s2.led_position < LED_MAX)
			SetOnAutoLeftMove(&s2);

		else {
			s2.led_position = LED_MIN;
			writeb_virtual_io(BARLED2, s2.led_position);
		}

		vTaskDelayUntil(&s2.xLastWakeTime, MSEC2TICK(500));
	}
}

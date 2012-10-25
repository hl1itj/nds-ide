/*

 * Exp_Homework.c

 *

 *  Created on: 2011. 9. 22.

 *      Author: Minsuk Lee

 */

#include "FreeRTOS.h"

#include "task.h"

#include <nds.h>

#include <stdio.h>

#include <time.h>

#include <stdlib.h>

#include "sevencore_io.h"

portTickType short_timer;

u16 barled1;
u16 barled2;

int chk;

#define NUM_STATE 6

#define NUM_INPUT 3

#define SHORT 4

#define LONG 5

// Actions

struct state_machine_x {

	int check_timer;

	int next_state[NUM_INPUT];

	void (*action[NUM_INPUT])(void *p);

};

enum {

	SW_ON, SW_OFF, TO

};

static

void f_led_B(void *p) {

	u8 tmpled;

	if (chk == SHORT) {
		if (barled1 == 0x00)
			barled1 = 0x80;
		else if (barled1 == 0xff && barled2 == 0) {
			barled2 = 0x80;
		} else if (barled1 == 0xff && barled2 != 0) {
			tmpled = barled2 >> 1;
			barled2 = tmpled | barled2;

		}

		else {
			tmpled = barled1 >> 1;
			barled1 = tmpled | barled1;
		}
		writeb_virtual_io(BARLED1, barled1);

		writeb_virtual_io(BARLED2, barled2);
		chk = 0;
	}

	else if (chk == LONG) {

		barled1 = 0xFF;
		barled2 = 0x00;
		writeb_virtual_io(BARLED1, barled1);
		writeb_virtual_io(BARLED2, barled2);
		chk = 0;
	}

}

static

void f_led_S(void *p) {
	u8 tmpled;
	if (chk == SHORT) {
		if (barled1 == 0xff && barled2 != 0) {
			tmpled = barled2 << 1;
			barled2 = tmpled;
		} else if (barled1 != 0 && barled2 == 0) {
			tmpled = barled1 << 1;
			barled1 = tmpled;

		}

		writeb_virtual_io(BARLED1, barled1);
		writeb_virtual_io(BARLED2, barled2);
		chk = 0;
	}

	else if (chk == LONG) {
		barled1 = 0xFF;
		barled2 = 0xFF;

		writeb_virtual_io(BARLED1, barled1);

		writeb_virtual_io(BARLED2, barled2);
		chk = 0;
	}

}

static

void f_led_L(void *p) {

	if (chk == SHORT) {

		barled1 = 0xFC;
		barled2 = 0x00;
		writeb_virtual_io(BARLED1, barled1);

		writeb_virtual_io(BARLED2, barled2);
		chk = 0;

	}

	else if (chk == LONG) {

		writeb_virtual_io(BARLED1, 0);

		writeb_virtual_io(BARLED2, 0);

		chk = 0;

		barled1 = 0x00;
		barled2 = 0x00;

	}

}

static

void f_ts(void *p) {

	short_timer = xTaskGetTickCount();

}

static

void f_chk_double_S(void *p) {
	f_ts(NULL);

	chk = SHORT;

}

static

void f_chk_double_L(void *p) {

	f_ts(NULL);

	chk = LONG;

}

struct state_machine_x SM[NUM_STATE] = {

// SW_ON              SW_OFF           TO

		{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, /* State 0 */

		{ 1, { 1, 3, 2 }, { NULL, f_chk_double_S, NULL } }, /* State 1 */

		{ 0, { 2, 3, 2 }, { NULL, f_chk_double_L, NULL } }, /* State 2 */

		{ 1, { 4, 3, 0 }, { f_ts, NULL, f_led_B } }, /* State 3 */

		{ 1, { 4, 0, 5 }, { NULL, f_led_S, NULL } }, /* State 4 */

		{ 0, { 5, 0, 5 }, { NULL, f_led_L, NULL } } /* State 5 */

};

void Exp_3_Homework(void) {

	int state;

	int input;

	barled1 = 0x00;
	barled2 = 0x00;

	printf("EXP_3_HomeWork\n");

	state = 0;       // Initial State 0 : All LED Off

	writeb_virtual_io(BARLED1, barled1);

	writeb_virtual_io(BARLED2, barled2);

	while (1) {

		/* Step 0: Generate Input Event */

		if (SM[state].check_timer) {

			if ((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200) ) {

				input = TO;

				goto do_action;

				// Input happens

			}

		}

		if (NDS_SWITCH() & KEY_A)

			input = SW_ON;

		else

			input = SW_OFF;

		/* Step 1: Do Action */

		do_action: if (SM[state].action[input])

			SM[state].action[input](NULL);

		/* Step 2: Set Next State */

		state = SM[state].next_state[input];

		if (NDS_SWITCH() & KEY_START)

			break;

		vTaskDelay(MSEC2TICK(50) );

	}

	while (NDS_SWITCH() & KEY_START)

		vTaskDelay(MSEC2TICK(10) );  // Wait while START KEY is being pressed

}

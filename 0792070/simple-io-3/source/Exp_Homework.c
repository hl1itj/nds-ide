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
portTickType start_time_c;
u16 barled;
u16 position = BARLED1;
u8 doublecheck;

#define NUM_STATE	9
#define NUM_INPUT	4

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO, NDC
};

void f_led1(void *p) { // On BARLED1 all
	printf("<L>\n");
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0);
	barled = 0xFF;
	position = BARLED1;
}

static
void f_ledall(void *p) { // On BARLED1, 2 all
	printf("<LS>\n");
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0xFF);
	barled = 0xFF;
	position = BARLED2;
}

static
void f_led1_6(void *p) { // On BARLED1 6ea from left
	printf("<SL>\n");
	writeb_virtual_io(BARLED1, 0xFC);
	writeb_virtual_io(BARLED2, 0);
	barled = 0xFC;
	position = BARLED1;
}

static void f_clear(void *p) {
	printf("<LL>\n");
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
	barled = 0x00;
	position = BARLED1;
}

static void f_plusplus(void *p) {

	if (barled == 0xFF) {
		if (position == BARLED1) {
			barled = 0x80;
			position = BARLED2;
		} else if (position == BARLED2)
			;
	} else {
		barled = (barled >> 1) + 0x80;
		printf("<S>\n");
	}
	writeb_virtual_io(position, barled);
}

static void f_minus(void *p) {

	if (barled == 0x00) {
		if (position == BARLED2) {
			barled = 0xFE;
			position = BARLED1;
		} else if (position == BARLED1)
			;
	} else {
		barled = (barled - 0x80) << 1;
		printf("<SS>\n");
	}
	writeb_virtual_io(position, (barled));
}

static
void f_ts(void *p) {
	start_time_c = xTaskGetTickCount();
}

static
void f_dts(void *p) {
	short_timer = xTaskGetTickCount();
	doublecheck = TRUE;
}

struct state_machine_x SM[NUM_STATE] = {
{ 0, { 1, 0, 0, 0 }, { f_ts, NULL,	NULL, NULL } }, /* State 0 */
{ 1, { 1, 2, 5, 1 }, { NULL, f_dts, NULL, NULL } }, /* State 1 */
{ 1, { 3, 2, 2, 0 }, { f_ts, NULL, NULL, f_plusplus } }, /* State 2 */
{ 1, { 3, 0, 4, 3 }, { NULL, f_minus, NULL, NULL } }, /* State 3 */
{ 0, { 4, 0, 4, 4 }, { NULL, f_led1_6, NULL, NULL } }, /* State 4 */
{ 0, { 5, 6, 5, 5 }, { NULL, f_dts, NULL, NULL } }, /* State 5 */
{ 1, { 7, 6, 6, 0 }, { f_ts, NULL, NULL, f_led1 } }, /* State 6 */
{ 1, { 7, 0, 8, 7 }, { NULL, f_ledall, NULL, NULL } }, /* State 7 */
{ 0, { 8, 0, 8, 8 }, { NULL, f_clear, NULL, NULL } } /* State 8 */
};

void Exp_3_Homework(void) {
	int state;
	int input;

	printf("EXP_Homework\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if (((xTaskGetTickCount() - start_time_c) >= MSEC2TICK(200) )&&(doublecheck == FALSE)){
			input = TO;
			goto do_action;
		}
		if(((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200)) &&(doublecheck == TRUE)) {
			input = NDC;
			doublecheck = FALSE;
			goto do_action;
		}
	}

	if (NDS_SWITCH() & KEY_A) {
		input = SW_ON;
		doublecheck = FALSE;
	}
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
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

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

portTickType start_time_H = 0;

u16 barled = 0x00;
u8 status = TRUE;

#define NUM_STATE 9
#define NUM_INPUT 4

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO, NO_DOUBLE
};

static
void h_led1(void *p) {
	if (status && barled == 0xFF) {
		barled = 0x80;
		status = FALSE;
	} else if (!status && barled == 0xFF)
		;
	else {
		barled = (barled >> 1) + 0x80;
	}
	if (status)
		writeb_virtual_io(BARLED1, (barled));
	else
		writeb_virtual_io(BARLED2, (barled));
}
static
void h_led2(void *p) {
	status = TRUE;
	barled = 0xFF;
	writeb_virtual_io(BARLED1, barled);
	writeb_virtual_io(BARLED2, 0x00);
}
static
void h_led3(void *p) {
	if (!status && barled == 0x00) {
		barled = 0xFE;
		status = TRUE;
	} else if (status && barled == 0x00)
		;
	else {
		barled = (barled - 0x80) << 1;
	}
	if (status)
		writeb_virtual_io(BARLED1, (barled));
	else
		writeb_virtual_io(BARLED2, (barled));
}
static
void h_led4(void *p) {
	status = TRUE;
	barled = 0xFC;
	writeb_virtual_io(BARLED1, barled);
	writeb_virtual_io(BARLED2, 0);
}
static
void h_led5(void *p) {
	status = FALSE;
	barled = 0xFF;
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, barled);
}
static
void h_led6(void *p) {
	status = TRUE;
	barled = 0x00;
	writeb_virtual_io(BARLED1, barled);
	writeb_virtual_io(BARLED2, 0);
}

static
void h_ts(void *p) {
	start_time_H = xTaskGetTickCount();
}


struct state_machine_x SM[NUM_STATE] = {
{ 0, { 1, 0, 0 }, { h_ts, NULL,NULL } }, /* State 0 */
{ 1, { 1, 2, 5 }, { NULL, h_ts, NULL} }, /* State 1 */
{ 1, { 3, 2, 0 }, { h_ts, NULL, h_led1 } }, /* State 2 */
{ 1, { 3, 0, 4 }, { NULL, h_led3, NULL } }, /* State 3 */
{ 0, { 4, 0, 4 }, { NULL, h_led4, NULL } }, /* State 4 */
{ 0, { 5, 6, 5 }, { NULL, h_ts, NULL} }, /* State 5 */
{ 1, { 7, 6, 0 }, { h_ts, NULL, h_led2 } }, /* State 6 */
{ 1, { 7, 0, 8 }, { NULL, h_led5, NULL } }, /* State 7 */
{ 0, { 8, 0, 8 }, { NULL, h_led6, NULL } } /* State 8 */
};

void Exp_3_Homework(void) {

	int state;
	int input;
	status = TRUE;
	barled = 0x00;

	printf("Exp_3_Homework\n");

	state = 0;							// Initial State 0 : All LED Off

	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if (((xTaskGetTickCount() - start_time_H) >= MSEC2TICK(200) )){
			input = TO;
			goto do_action;
			// Input happens
		}
	}
	if (NDS_SWITCH() & KEY_A) {
		input = SW_ON;
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

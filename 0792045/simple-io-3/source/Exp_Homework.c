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

portTickType short_timer = 0;
portTickType start_time_H = 0;
u16 barled = 0x01;
u8 d_check = FALSE;

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
	if (barled <= 0xFF)
		barled = barled << 1;
	writeb_virtual_io(BARLED2, (barled - 0x01));
}
static
void h_led2(void *p) {
	writeb_virtual_io(BARLED1, 0xFF);
}
static
void h_led3(void *p) {
	if (barled > 0x01)
		barled = barled >> 1;
	writeb_virtual_io(BARLED2, (barled - 0x01));
}
static
void h_led4(void *p) {
	writeb_virtual_io(BARLED1, 0xFC);
}
static
void h_led5(void *p) {
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0xFF);
}
static
void h_led6(void *p) {
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
}

static
void h_ts(void *p) {
	start_time_H = xTaskGetTickCount();
}
static
void h_dts(void *p) {
	short_timer = xTaskGetTickCount();
	d_check = TRUE;
}

struct state_machine_x SM[NUM_STATE] = { { 0, { 1, 0, 0, 0 }, { h_ts, NULL,
		NULL, NULL } }, /* State 0 */
{ 1, { 1, 2, 5, 1 }, { NULL, h_dts, NULL, NULL } }, /* State 1 */
{ 1, { 3, 2, 2, 0 }, { h_ts, NULL, NULL, h_led1 } }, /* State 2 */
{ 1, { 3, 0, 4, 3 }, { NULL, h_led3, NULL, NULL } }, /* State 3 */
{ 0, { 4, 0, 4, 4 }, { NULL, h_led4, NULL, NULL } }, /* State 4 */
{ 0, { 5, 6, 5, 5 }, { NULL, h_dts, NULL, NULL } }, /* State 5 */
{ 1, { 7, 6, 6, 0 }, { h_ts, NULL, NULL, h_led2 } }, /* State 6 */
{ 1, { 7, 0, 8, 7 }, { NULL, h_led5, NULL, NULL } }, /* State 7 */
{ 0, { 8, 0, 8, 8 }, { NULL, h_led6, NULL, NULL } } /* State 8 */
};

void Exp_3_Homework(void) {

	int state;
	int input;

	printf("Exp_3_Homework\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if (((xTaskGetTickCount() - start_time_H) >= MSEC2TICK(200) )&&
			(d_check == FALSE)){
			input = TO;
			goto do_action;
			// Input happens
		}
		if(((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200)) &&
				(d_check == TRUE)) {
			input = NO_DOUBLE;
			d_check = FALSE;
			goto do_action;
			// Not Double Click happens
		}
	}
	if (NDS_SWITCH() & KEY_A) {
		input = SW_ON;
		d_check = FALSE;
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

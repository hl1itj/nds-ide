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
u16 barled;
u16 position;
u8 doublecheck;

#define NUM_STATE	1
#define NUM_INPUT	1

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO, DC
};

void f_led1(void *p) { // On BARLED1 all
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0);
	barled = 0xFF;
	position = BARLED1;
}

static
void f_ledall(void *p) { // On BARLED1, 2 all
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0xFF);
	barled = 0xFF;
	position = BARLED2;
}

static
void f_led1_6(void *p) { // On BARLED1 6ea from left
	writeb_virtual_io(BARLED1, 0xFC);
	writeb_virtual_io(BARLED2, 0);
	barled = 0xFC;
	position = BARLED1;
}

static void f_clear(void *p) {
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
	barled = 0x00;
	position = BARLED1;
}

static void f_plusplus(void *p) {
	if (barled <= 0xFF) {
		if (barled == 0xFF && position == BARLED1) {
			barled = 0x80;
			position = BARLED2;
		}
		barled += (barled >> 1);
	}
	writeb_virtual_io(position, barled);
}

static void f_minus(void *p) {
	if (barled >= 0x00) {

	}
}

static
void f_ts(void *p) {
	start_time_c = xTaskGetTickCount();
	short_timer = xTaskGetTickCount();
	doublecheck = TRUE;
}

struct state_machine_x SM[NUM_STATE] = {
//Time check // SW_ON, SW_OFF, TO, DC
		{ 1, { 1, 0, 0, 0 }, { f_ts, NULL, NULL, NULL } }, /* state wait */
		{ 0, { 1, 0, 2, 3 }, { NULL, f_ts, NULL, f_ts } }, /* state 0 */
		{ 1, { 1, 0, 2, 3 }, { NULL, f_plusplus, NULL, f_ts } }, /* state 1 */
		{ 1, { 2, 0, 2, 5 }, { NULL, f_led1, NULL, f_ts } }, /* state 2 */
		{ 0, { 4, 0, 5, 3 }, { NULL, f_minus, NULL, f_ts } }, /* state 3 */
		{ 0, { 4, 0, 5, 3 }, { NULL, f_led1_6, NULL, f_ts } }, /* state 4 */
		{ 0, { 5, 0, 5, 5 }, { NULL, f_ledall, NULL, f_ts } } /* state 5 */
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
		if (Exam_SM_C[state].check_timer) {
			if (((xTaskGetTickCount() - start_time_c) >= MSEC2TICK(200) )&& !doublecheck){
				input = TO;
				goto do_action;
				// Input happens
			}

			if ((xTaskGetTickCount() - short_timer) <= MSEC2TICK(200) ) {

			}
		}
		if (NDS_SWITCH() & KEY_A)
			input = SW_ON;
		else
			input = SW_OFF;

		/* Step 1: Do Action */
		do_action: if (Exam_SM_C[state].action[input])
			Exam_SM_C[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = Exam_SM_B[state][input].next_state;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

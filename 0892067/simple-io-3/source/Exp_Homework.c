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

#define NUM_STATE	9
#define NUM_INPUT	3

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void);
};
enum { SW_ON, SW_OFF, TO };

static
void
f_led1(void)
{
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0);
}

static
void
f_led2(void)
{
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0xFF);
}

static
void
f_ts(void)
{
	start_time_c = xTaskGetTickCount();
}

struct state_machine_x SM[NUM_STATE] = {
	// Fill here
	{ 0, { 1, 0, 0 }, { f_ts, NULL,   NULL } },    /* State 0 */
    { 1, { 1, 0, 2 }, { NULL, f_led1, NULL } },    /* State 1 */
    { 0, { 2, 0, 2 }, { NULL, f_led2, NULL } }     /* State 2 */
};

void
Exp_3_Homework(void)
{
    // variables

	while (1) {
		/* Step 0: Generate Input Event */
		if (Exam_SM_C[state].check_timer) {
			if ((xTaskGetTickCount() - start_time_c) >= MSEC2TICK(300)) {
				input = TO;
				goto do_action;		// Input happens
			}
		}
		if (NDS_SWITCH() & KEY_A)
			input = SW_ON;
		else
			input = SW_OFF;

		/* Step 1: Do Action */
do_action:
		if (Exam_SM_C[state].action[input])
			Exam_SM_C[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = Exam_SM_B[state][input].next_state;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

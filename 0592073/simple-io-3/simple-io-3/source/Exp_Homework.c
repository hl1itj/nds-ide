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
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };

static u8 offset = 0x00;
portTickType start_time = 0;

static void f_ts(void *p)
{
	start_time = xTaskGetTickCount();
}

static void f_led1 (void *p)
{
	printf("1\n");
	/*
	offset = offset * 2;
	writeb_virtual_io(BARLED2, offset);
	*/
}

static void f_led2 (void *p)
{
	printf("2\n");
}

static void f_led3 (void *p)
{
	printf("3\n");
	/*
	offset = offset / 2;
	writeb_virtual_io(BARLED2, offset);
	*/
}

static void f_led4 (void *p)
{
	printf("4\n");
	/*
	writeb_virtual_io(BARLED1, 254);
	*/
}

static void f_led5 (void *p)
{
	printf("5\n");
	/*
	writeb_virtual_io(BARLED1, 255);
	writeb_virtual_io(BARLED2, 255);
*/
}

static void f_led6 (void *p)
{
	printf("6\n");
	/*
	offset = 0x00;
	writeb_virtual_io(BARLED1, 0x00);
	writeb_virtual_io(BARLED2, 0x00);
	*/
}

struct state_machine_x Exam_SM_X[NUM_STATE] = {
	// Fill here
		{0, {1, 0, 0 }, {f_ts, NULL, NULL } },
		{1, {1, 3, 2 }, {NULL, f_ts, NULL } },
		{0, {2, 6, 0 }, {NULL, f_ts, NULL } },
		{1, {4, 0, 0 }, {f_ts, f_led1, NULL} },
		{1, {4, 0, 5 }, {NULL, f_led3, NULL} },
		{0, {5, 0, 5 }, {NULL, f_led4, NULL} },
		{1, {7, 6, 0 }, {f_ts, f_led2, NULL} },
		{1, {7, 0, 8 }, {NULL, f_led5, NULL} },
		{0, {8, 0, 8 }, {NULL, f_led6, NULL} }
};

void
Exp_3_Homework(void)
{
    // variables
	int state;
	int input;

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (Exam_SM_X[state].check_timer) {
			if ((xTaskGetTickCount() - start_time) >= MSEC2TICK(200)) {
				input = TO;
				goto do_action;		// Input happens
			}
		}

		if (NDS_SWITCH() & KEY_A)
			input = SW_ON;
		else
			input = SW_OFF;

do_action:
	/* Step 1: Do Action */
	if (Exam_SM_X[state].action[input])
		Exam_SM_X[state].action[input](NULL);

	/* Step 2: Set Next State */
		state = Exam_SM_X[state].next_state[input];

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

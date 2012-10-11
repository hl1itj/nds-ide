/*
 * Exp_Sample.c
 *
 *  Created on: 2010. 10. 11.
 *      Author: Minsuk Lee
 */

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

// First Method : Use Case
void
Exp_3_Sample_A(void)
{
	int state;
	u16 SWITCH_ON;
	portTickType start_time = 0;

	printf("EXP_3_Sample_A\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		SWITCH_ON = (NDS_SWITCH() & KEY_A);
		switch (state) {
		  case 0 :
			if (SWITCH_ON) {
				state = 1;							// Next State
				start_time = xTaskGetTickCount();	// Action
			}
			break;
		  case 1 :
			if (!SWITCH_ON) {
				state = 0;							// Next State
				writeb_virtual_io(BARLED1, 0xFF);	// Action for Short Click
				writeb_virtual_io(BARLED2, 0);
			} else	if ((xTaskGetTickCount() - start_time) >= MSEC2TICK(300)) {
					state = 2;		// Timeout !
			}
			break;
		  case 2 :
			if (!SWITCH_ON) {
				state = 0;							// Next State
				writeb_virtual_io(BARLED1, 0);		// Action
				writeb_virtual_io(BARLED2, 0xFF);
			}
			break;
		  default :	// Illegal State !!!
			state = 0;							// Going to Initial State
			writeb_virtual_io(BARLED1, 0);
			writeb_virtual_io(BARLED2, 0);
			break;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

// Second Method : Use State Table

#define NUM_STATE    3
#define NUM_INPUT    3

struct state_machine_b {
	int next_state;
	int action;
};

enum { SW_ON, SW_OFF, TO };
enum { NOP, LED1, LED2, TS };

struct state_machine_b Exam_SM_B[NUM_STATE][NUM_INPUT] = {
	  // SW_ON       SW_OFF        TO
    { { 1, TS  }, { 0, NOP  }, { 0, NOP } },    /* State 0 */
    { { 1, NOP }, { 0, LED1 }, { 2, NOP } },    /* State 1 */
    { { 2, NOP }, { 0, LED2 }, { 2, NOP } }     /* State 2 */
};

void
Exp_3_Sample_B(void)
{
	portTickType start_time = 0;
    int state;
    int input;

	printf("EXP_3_Sample_B\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if ((state == 1) && ((xTaskGetTickCount() - start_time) >= MSEC2TICK(300)))
			input = TO;
		else if (NDS_SWITCH() & KEY_A)
			input = SW_ON;
		else
			input = SW_OFF;

		/* Step 1: Do Action */
		switch (Exam_SM_B[state][input].action) {
		  case TS :
			start_time = xTaskGetTickCount();
			break;
		  case LED1 :
			writeb_virtual_io(BARLED1, 0xFF);
			writeb_virtual_io(BARLED2, 0);
			break;
		  case LED2 :
			writeb_virtual_io(BARLED1, 0);
			writeb_virtual_io(BARLED2, 0xFF);
			break;
		  case NOP :	// No operation
		  default :		// Error !, No nothing
			break;
		}
		/* Step 2: Set Next State */
		state = Exam_SM_B[state][input].next_state;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

portTickType start_time_c = 0;

struct state_machine_c {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};

static
void
f_led1(void *p)
{
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0);
}

static
void
f_led2(void *p)
{
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0xFF);
}

static
void
f_ts(void *p)
{
	start_time_c = xTaskGetTickCount();
}

struct state_machine_c Exam_SM_C[NUM_STATE] = {
	   // SW_ON              SW_OFF           TO
    { 0, { 1, 0, 0 }, { f_ts, NULL,   NULL } },    /* State 0 */
    { 1, { 1, 0, 2 }, { NULL, f_led1, NULL } },    /* State 1 */
    { 0, { 2, 0, 2 }, { NULL, f_led2, NULL } }     /* State 2 */
};

void
Exp_3_Sample_C(void)
{
    int state;
    int input;

	printf("EXP_3_Sample_C\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

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

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
portTickType interval_timer;
u16 barled;
int pre_click = 0;

#define NUM_STATE	6
#define NUM_INPUT	3
#define MIN_LED 0x01
#define MAX_LED 0x80
#define LED1(x) writeb_virtual_io(BARLED1, x)
#define LED2(x) writeb_virtual_io(BARLED2, x)

// Actions
static
void
f_dcs(void *p)
{
	printf("%s\n", (pre_click == 1) ? "SS" : "LS");
}

static
void
f_dcl(void *p)
{
	printf("%s\n", (pre_click == 1) ? "SL" : "LL");
}

static
void
f_sc(void *p)
{
	printf("%s\n", (pre_click == 1) ? "S" : "L");

	if (pre_clicked == 1) {
		barled
	}
}

static
void
f_ts(void *p)
{
	short_timer = xTaskGetTickCount();
}

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };

struct state_machine_x SM[NUM_STATE] = {
		   // SW_ON              SW_OFF           TO
	    { 0, { 1, 0, 0 }, { f_ts, NULL, NULL } },    /* State 0 */
	    { 1, { 1, 3, 2 }, { NULL, NULL, NULL } },    /* State 1 */
	    { 0, { 2, 3, 2 }, { NULL, NULL, NULL } },     /* State 2 */
	    { 0, { 4, 0, 0 }, { f_ts, f_sc, NULL } },     /* State 3 */
	    { 1, { 4, 0, 5 }, { NULL, f_dcs, NULL } },    /* State 4 */
	    { 0, { 5, 0, 5 }, { NULL, f_dcl, NULL } },    /* State 5 */
};

void
Exp_3_Homework(void)
{
    // variables
	int state;
	int input;
	barled = MIN_LED;

	printf("Exp_3_Homework\n");

	while (1) {
		if (SM[state].check_timer) {
			if ((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200)) {
				input = TO;
				goto do_action;		// Input happens
			}
		}

		if(state == 3) {
			if (NDS_SWITCH() & KEY_A) {
				if ((xTaskGetTickCount() - interval_timer) <= MSEC2TICK(200))
					input = SW_ON;
				else
					input = SW_OFF;
			}
			else
				input = SW_OFF;
		}
		else {
			if (NDS_SWITCH() & KEY_A)
				input = SW_ON;
			else
				input = SW_OFF;
		}

		/* Step 1: Do Action */
do_action:
		if (SM[state].action[input])
				SM[state].action[input](NULL);

		/* Step 2: Set Next State */
		if(state==1 || state==2)
			pre_click = state;

		state = SM[state].next_state[input];
		if(state == 3)
			interval_timer = xTaskGetTickCount();

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

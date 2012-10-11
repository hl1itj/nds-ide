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

portTickType start_time_x = 0;
portTickType wait_time_x = 0;
u16 barled;

#define NUM_STATE	1
#define NUM_INPUT	1

// Actions

u8 barled2_st = 0x00;
u8 barled2_ct = 0;

static
void
x_led1(void *p)
{
	if (barled2_ct != 8) {
		barled2_st += 0x80 >> barled2_ct;
		barled2_ct++;
	}
	writeb_virtual_io(BARLED2, barled2_st);
}

static
void
x_led2(void *p)
{
	if (barled2_ct != 0) {
		barled2_st += 0x01 << (8-barled2_ct);
		barled2_ct--;
	}
	writeb_virtual_io(BARLED2, barled2_st);
}

static
void
x_led3(void *p)
{
	writeb_virtual_io(BARLED1, 0xFC);
}

static
void
x_led4(void *p)
{
	writeb_virtual_io(BARLED1, 0xFF);
}

static
void
x_led5(void *p)
{
	writeb_virtual_io(BARLED1, 0xFF);
	writeb_virtual_io(BARLED2, 0xFF);
	barled2_st = 0xFF;
	barled2_ct = 8;
}

static
void
x_led6(void *p)
{
	writeb_virtual_io(BARLED1, 0x00);
	writeb_virtual_io(BARLED2, 0x00);
	barled2_st = 0x00;
	barled2_ct = 0;
}

static
void
x_ts(void *p)
{
	start_time_x = xTaskGetTickCount();
}

static
void
x_tw(void *p)
{
	wait_time_x = xTaskGetTickCount();
}


struct state_machine_x {
	int check_timer[NUM_INPUT];
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ST, SW_WT };
enum { SW_ON, SW_OFF, TO };

struct state_machine_x SM[NUM_STATE] = {

	    { {0,0}, { 1, 0, 0 }, { x_ts, NULL, NULL } },    /* State 0 */
	    { {1,0}, { 1, 2, 5 }, { NULL, x_tw, NULL } },    /* State 1 */
	    { {0,1}, { 3, 0, 2 }, { NULL, x_led1, NULL } },     /* State 2 */
	    { {1,0}, { 3, 0, 4 }, { x_ts, x_led2, NULL } },     /* State 3 */
	    { {0,0}, { 4, 0, 4 }, { NULL, x_led3, NULL } },     /* State 4 */
	    { {0,0}, { 5, 6, 5 }, { NULL, x_tw, NULL } },    /* State 5 */
	    { {0,1}, { 7, 0, 6 }, { x_ts, x_led4, NULL } },     /* State 6 */
	    { {1,0}, { 7, 0, 8 }, { NULL, x_led5, NULL } },    /* State 7 */
	    { {0,1}, { 8, 0, 8 }, { NULL, x_led6, NULL } },     /* State 8 */
	// Fill here
};

void
Exp_3_Homework(void)
{
    int state;
    int input;

	printf("EXP_3_Howmwork\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
	// variables

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer[SW_ST]) {
			if ((xTaskGetTickCount() - start_time_x) >= MSEC2TICK(200)) {
				input = TO;
				goto do_action;		// Input happens
			}
		}
		if (SM[state].check_timer[SW_WT]) {
			if ((xTaskGetTickCount() - wait_time_x) >= MSEC2TICK(200)){
				input = SW_OFF;
				goto do_action;
			}
		}
		if (NDS_SWITCH() & KEY_A)
			input = SW_ON;
		else
			input = SW_OFF;


		/* Step 1: Do Action */
do_action:
		if (SM[state].action[input])
			SM[state].action[input](NULL);
		/* Step 2: Set Next State */
		state = SM[state].next_state[input];

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

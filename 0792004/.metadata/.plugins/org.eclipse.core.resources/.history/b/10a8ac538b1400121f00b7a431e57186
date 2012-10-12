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
u8 sw_bar;

#define NUM_STATE	9
#define NUM_INPUT	3

#define SW_ON_TIME MSEC2TICK(200)
#define SW_OFF_TIME MSEC2TICK(200)

#define OFF_LED 0x00
#define ON_LED 0xFF
#define LED1(x) writeb_virtual_io(BARLED1, x)
#define LED2(x) writeb_virtual_io(BARLED2, x)

// Actions
static
void
f_s(void *p)
{
	if(barled == OFF_LED)
		barled = 0x80;
	else if ((!sw_bar) && (barled == ON_LED)) {
		sw_bar = TRUE;
		barled = 0x80;
	}
	else
		barled = barled | (barled >> 1);

	(sw_bar) ? LED2(barled) : LED1(barled);
	printf("<S>\n");
}

static
void
f_l(void *p)
{
	sw_bar = FALSE;
	LED1(barled = ON_LED);
	LED2(OFF_LED);
	printf("<L>\n");
}

static
void
f_ss(void *p)
{
	if(((sw_bar) && (barled == OFF_LED)) ||
		((!sw_bar) && (barled == ON_LED))) {
		sw_bar = FALSE;
		barled = ON_LED & (ON_LED << 1);
	}
	else
		barled = barled & (barled << 1);

	(sw_bar) ? LED2(barled) : LED1(barled);
	printf("<SS>\n");
}

static
void
f_sl(void *p)
{
	barled = 0xFC;
	sw_bar = FALSE;
	LED2(0);
	LED1(barled);
	printf("<SL>\n");
}

static
void
f_ls(void *p)
{
	sw_bar = TRUE;
	barled = ON_LED;
	LED1(ON_LED);
	LED2(ON_LED);
	printf("<LS>\n");
}

static
void
f_ll(void *p)
{
	sw_bar = FALSE;
	barled = OFF_LED;
	LED1(OFF_LED);
	LED2(OFF_LED);
	printf("<LL>\n");
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
	    { 1, { 1, 3, 2 }, { NULL, f_ts, NULL } },    /* State 1 */
	    { 0, { 2, 6, 2 }, { NULL, f_ts, NULL } },     /* State 2 */
	    { 1, { 4, 3, 0 }, { f_ts, NULL, f_s } },     /* State 3 */
	    { 1, { 4, 0, 5 }, { NULL, f_ss, NULL } },    /* State 4 */
	    { 0, { 5, 0, 5 }, { NULL, f_sl, NULL } },    /* State 5 */
	    { 1, { 7, 6, 0 }, { f_ts, NULL, f_l } },    /* State 6 */
	    { 1, { 7, 0, 8 }, { NULL, f_ls, NULL } },    /* State 7 */
	    { 0, { 8, 0, 8 }, { NULL, f_ll, NULL } },    /* State 7 */
};

void
Exp_3_Homework(void)
{
    // variables
	int state;
	int input;

	//barled = 0;
	LED1(0);
	LED2(0);

	printf("Exp_3_Homework\n");

	while (1) {
		if (SM[state].check_timer) {
			if (input == SW_ON) {
				if ((xTaskGetTickCount() - short_timer) >= SW_ON_TIME) {
					input = TO;
					goto do_action;		// Input happens
				}
			}
			else {
				if ((xTaskGetTickCount() - short_timer) >= SW_OFF_TIME) {
					input = TO;
					goto do_action;		// Input happens
				}
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

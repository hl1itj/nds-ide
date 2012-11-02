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

u16 barled;
portTickType start_time = 0;

#define NUM_STATE	9
#define NUM_INPUT	3

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };


u8 led_state1=0;
u8 led_state2=0;

static
void
LEFT_ON(void *p)
{
	if(led_state1==0){
		led_state1=0x80;
	}
	else if(led_state1==0xff && led_state2==0){
		led_state2=0x80;
	}
	else if(led_state1==0xff && led_state2!=0){
		u8 temp;
		temp=led_state2>>1;
		led_state2=led_state2 | temp;
	}
	else{
		u8 temp;
		temp=led_state1>>1;
		led_state1=led_state1 | temp;
	}
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<S>\n");
}

static
void
LEFT_OFF(void *p)
{
	if(led_state2==0){
		led_state1=led_state1 << 1;
	}
	else{
		led_state2=led_state2 << 1;
	}
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<SS>\n");
}

static
void
LEFT_6_ON(void *p)
{
	led_state1=0xfc;
	led_state2=0;
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<SL>\n");
}

static
void
BAR1_ON(void *p)
{
	led_state1=0xff;
	led_state2=0;
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<L>\n");
}

static
void
BAR12_ON(void *p)
{
	led_state1=0xff;
	led_state2=0xff;
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<LS>\n");
}

static
void
BAR12_OFF(void *p)
{
	led_state1=0;
	led_state2=0;
	writeb_virtual_io(BARLED1, led_state1);
	writeb_virtual_io(BARLED2, led_state2);
	printf("<LL>\n");
}

static
void
f_ts(void *p)
{

	start_time = xTaskGetTickCount();
}

struct state_machine_x SM[NUM_STATE] = {
	   // SW_ON              SW_OFF           TO
    { 0, { 1, 0, 0 }, { f_ts, NULL,   NULL } },    /* State 0 */
    { 1, { 1, 2, 5 }, { NULL, f_ts, NULL } },    /* State 1 */
    { 1, { 3, 2, 0 }, { f_ts, NULL, LEFT_ON } },     /* State 2 */
    { 1, { 3, 0, 4 }, { NULL, LEFT_OFF,   NULL } },    /* State 3 */
    { 0, { 4, 0, 4 }, { NULL, LEFT_6_ON, NULL } },    /* State 4 */
    { 0, { 5, 6, 5 }, { NULL, f_ts, NULL } },     /* State 5 */
    { 1, { 7, 6, 0 }, { f_ts, NULL,   BAR1_ON } },    /* State 6 */
    { 1, { 7, 0, 8 }, { NULL, BAR12_ON, NULL } },    /* State 7 */
    { 0, { 8, 0, 8 }, { NULL, BAR12_OFF, NULL } }     /* State 8 */
};

void
Exp_3_Homework(void)
{
		int state;
		int input;
		printf("EXP_3_Homework\n");

		state = 0;							// Initial State 0 : All LED Off
		led_state1=0;
		led_state2=0;
		writeb_virtual_io(BARLED1, 0);
		writeb_virtual_io(BARLED2, 0);

		while (1) {
			/* Step 0: Generate Input Event */
			if (SM[state].check_timer) {
				if ((xTaskGetTickCount() - start_time) >= MSEC2TICK(200)) {
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

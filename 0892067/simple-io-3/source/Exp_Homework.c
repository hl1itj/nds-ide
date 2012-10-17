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

portTickType start_time_h = 0;
u16 barled;

#define NUM_STATE	9
<<<<<<< HEAD
#define NUM_INPUT	3 //Sw-on,Sw-off,TO
=======
#define NUM_INPUT	3
>>>>>>> 12ccbbcd4acde7797fa766f7a9f5916920d6949d

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void);
};
enum { SW_ON, SW_OFF, TO };

<<<<<<< HEAD
void short_action(void);//A2
void long_action(void);//A3
void double_short_action(void);//A4
void short_long_action(void);//A5
void long_short_action(void);//A6
void double_long_action(void);//A7
void time_start(void);

struct state_machine_x SM[NUM_STATE] = {
	// Fill here
       {{0},{1,0,0},{time_start,NULL,NULL}},
       {{1},{1,3,2},{NULL,time_start,NULL}},
       {{0},{2,4,2},{NULL,time_start,NULL}},
       {{1},{6,3,0},{time_start,NULL,short_action}},
       {{1},{5,4,0},{time_start,NULL,long_action}},
       {{1},{5,0,8},{NULL,long_short_action,NULL}},
       {{1},{6,0,7},{NULL,double_short_action,NULL}},
       {{0},{7,0,7},{NULL,short_long_action,NULL}},
       {{0},{8,0,8},{NULL,double_long_action,NULL}}
        
=======
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
>>>>>>> 12ccbbcd4acde7797fa766f7a9f5916920d6949d
};

void
Exp_3_Homework(void)
{
    // variables
    int state;
    int input;

	printf("EXP_3_Homework GO! GO!\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);


	while (1) {
		/* Step 0: Generate Input Event */
<<<<<<< HEAD
		if (SM[state].check_timer) {
			if ((xTaskGetTickCount() - start_time_h) >= MSEC2TICK(300)) {
=======
		if (Exam_SM_C[state].check_timer) {
			if ((xTaskGetTickCount() - start_time_c) >= MSEC2TICK(300)) {
>>>>>>> 12ccbbcd4acde7797fa766f7a9f5916920d6949d
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
<<<<<<< HEAD
		if (SM[state].action[input])
			SM[state].action[input]();

		/* Step 2: Set Next State */
		state = SM[state].next_state[input];
=======
		if (Exam_SM_C[state].action[input])
			Exam_SM_C[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = Exam_SM_B[state][input].next_state;
>>>>>>> 12ccbbcd4acde7797fa766f7a9f5916920d6949d

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

 void short_action()//A2
{
	printf("< S >\n");
}//A2
void long_action()//A3
{
	printf("< L >\n");
}
 void double_short_action()//A4
{
	printf("< SS >\n");
}
 void short_long_action()//A5
{
	printf("< SL >\n");
}
 void long_short_action()//A6
{
	printf("< LS >\n");
}
void double_long_action()//A7
{
	printf("< LL >\n");
}
void time_start()
{
	start_time_h = xTaskGetTickCount();
}

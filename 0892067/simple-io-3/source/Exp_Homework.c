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
u16 cur_barled = BARLED1;
short int led_result_state = 0x0000;
short int led_state = 0x0000;

#define NUM_STATE	9
#define NUM_INPUT	3 //Sw-on,Sw-off,TO
#define NUM_INPUT	3

// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void);
};
enum { SW_ON, SW_OFF, TO };

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
};


void Exp_3_Homework(void)
{
    // variables
    int state;
    int input;

	printf("EXP_3_Homework GO! GO!\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	start_time_h = 0;
	cur_barled = BARLED1;
	led_result_state = 0x0000;
	led_state = 0x0000;

	while (1) {
		/* Step 0: Generate Input Event */

		if (SM[state].check_timer) {
			if ((xTaskGetTickCount() - start_time_h) >= MSEC2TICK(300)) {
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
			SM[state].action[input]();

		/* Step 2: Set Next State */
		state = SM[state].next_state[input];

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

	if((led_state == 0x0000)){
		led_state = 0x0080;
		led_result_state = 0x0080;
		writeb_virtual_io(cur_barled, led_state);
		return;
	}

	if(led_state == 0x0001) {
		if(cur_barled == BARLED1) {
		led_state = 0x0080;
		led_result_state = 0x0080;
		cur_barled = BARLED2;
		writeb_virtual_io(cur_barled,led_state);
	    }
		return;

	} else {
		led_result_state = led_result_state | (led_state / 2);
		led_state = led_state / 2;
	}

	writeb_virtual_io(cur_barled, led_result_state);
}
void long_action()//A3
{
	printf("< L >\n");
	writeb_virtual_io(BARLED2, 0x0000);
	writeb_virtual_io(BARLED1, 0xFFFF);
	cur_barled = BARLED1;
	led_state = 0x0001;
	led_result_state = 0xFFFF;
}
 void double_short_action()//A4
{
	printf("< SS >\n");

	if ((led_state == 0x0080) && (cur_barled == BARLED1)){
		writeb_virtual_io(cur_barled, 0);
		led_state = 0x0000;
		led_result_state = 0x0000;
		return;
	}

	if((led_state == 0x0080) && (cur_barled == BARLED2)) {
		writeb_virtual_io(cur_barled, 0);
		led_state = 0x0001;
		led_result_state = 0xFFFF;
		cur_barled = BARLED1;
		return;

	} else{
		led_result_state = led_result_state & (~led_state);
		led_state = led_state * 2;
	}

	writeb_virtual_io(cur_barled, led_result_state);
}
 void short_long_action()//A5
{
	printf("< SL >\n");
	cur_barled = BARLED1;
	led_state = 0x0004;
	led_result_state = 0x80|0x40|0x20|0x10|0x08|0x04;
	writeb_virtual_io(BARLED1, led_result_state);
	writeb_virtual_io(BARLED2, 0);
}
 void long_short_action()//A6
{
	printf("< LS >\n");
	writeb_virtual_io(BARLED1, 0xFFFF);
	writeb_virtual_io(BARLED2, 0xFFFF);
	cur_barled = BARLED2;
	led_result_state = 0xFFFF;
	led_state = 0x0001;
}
void double_long_action()//A7
{
	printf("< LL >\n");
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
	cur_barled = BARLED1;
	led_result_state = 0x0000;
	led_state = 0x0000;
}
void time_start()
{
	start_time_h = xTaskGetTickCount();
}

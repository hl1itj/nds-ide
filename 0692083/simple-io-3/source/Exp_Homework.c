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

u8 LED1_barled;
u8 LED2_barled;
u8 led_max;

u8 LED1_state = TRUE;
u8 LED2_state = FALSE;

#define NUM_STATE	9
#define NUM_INPUT	3

#define LED_FULL 0xFF
#define LED_OFF 0x00

// Actions

struct state_machine_x {

	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO
};

static void f_ts(void *p) {

	short_timer = xTaskGetTickCount();
}

static void f_short_click(void *p) {

	printf("S \n");

	if (LED1_barled != LED_FULL) {

		LED1_barled = (LED1_barled >> 1) | led_max;

		writeb_virtual_io(BARLED1, LED1_barled);
		writeb_virtual_io(BARLED2, LED_OFF);


	} else {

		LED2_barled = (LED2_barled >> 1) | led_max;

		writeb_virtual_io(BARLED1, LED1_barled);
		writeb_virtual_io(BARLED2, LED2_barled);

	}

}

static void f_short_short_click(void *p) {

	printf("SS \n");

	if (LED2_barled != LED_OFF) {

		LED2_barled = LED2_barled << 1;

		writeb_virtual_io(BARLED1, LED1_barled);
		writeb_virtual_io(BARLED2, LED2_barled);


	} else {

		LED1_barled = LED1_barled << 1;

		writeb_virtual_io(BARLED1, LED1_barled);
		writeb_virtual_io(BARLED2, LED_OFF);

	}
}

static void f_short_long_click(void *p) {

	printf("SL \n");

	LED1_barled = 0xFC;
	LED2_barled = LED_OFF;

	LED2_state = FALSE;

	writeb_virtual_io(BARLED1, LED1_barled);
	writeb_virtual_io(BARLED2, LED2_barled);
}

static void f_long_click(void *p) {


	printf("L \n");

	LED1_barled = LED_FULL;
	LED2_barled = LED_OFF;

	writeb_virtual_io(BARLED1, LED1_barled);
	writeb_virtual_io(BARLED2, 0);

}

static void f_long_short_click(void *p) {

	printf("LS \n");

	LED1_barled = LED_FULL;
	LED2_barled = LED_FULL;

	LED2_state = TRUE;

	writeb_virtual_io(BARLED1, LED1_barled);
	writeb_virtual_io(BARLED2, LED2_barled);
}

static void f_long_long_click(void *p) {

	printf("LL \n");

	LED1_barled = LED_OFF;
	LED2_barled = LED_OFF;

	LED2_state = FALSE;

	writeb_virtual_io(BARLED1, LED1_barled);
	writeb_virtual_io(BARLED2, LED2_barled);

}

struct state_machine_x SM[NUM_STATE] = {
// Fill here

		{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, // state 0
		{ 1, { 1, 3, 2 }, { NULL, f_ts, NULL } }, // state 1
		{ 0, { 2, 4, 0 }, { NULL, f_ts, NULL } }, // state 2

		{ 1, { 5, 3, 0 }, { f_ts, NULL, f_short_click } }, //state 3
		{ 1, { 7, 4, 0 }, { f_ts, NULL, f_long_click } }, //state 4

		{ 1, { 5, 0, 6 }, { NULL, f_short_short_click, NULL } }, //state 5
		{ 0, { 6, 0, 0 }, { NULL, f_short_long_click, NULL } }, //state 6

		{ 1, { 7, 0, 8 }, { NULL, f_long_short_click, NULL } }, //state 7
		{ 0, { 8, 0, 0 }, { NULL, f_long_long_click, NULL } }, //state 8

		};

void Exp_3_Homework(void) {
// variables
	short_timer = 0;

	int state;
	int input;

	state = 0;

	LED1_barled = 0x00;
	LED2_barled = 0x00;
	led_max = 0x80;

	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	printf("Start Homework!!");

	while (1) {

		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if ((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200) ) {

				input = TO;
				goto do_action;
				// Input happens
			}
		}

		if (NDS_SWITCH() & KEY_A) {

			input = SW_ON;

		} else {

			input = SW_OFF;
		}

		/* Step 1: Do Action */
		do_action: if (SM[state].action[input])
			SM[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = SM[state].next_state[input];

		if (NDS_SWITCH() & KEY_START)
			break;

		vTaskDelay(MSEC2TICK(50) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );	// Wait while START KEY is being pressed
}

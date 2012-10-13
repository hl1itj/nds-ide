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

#define LED1 FALSE
#define LED2 TRUE

#define LED_MIN 0x01
#define LED_MAX 0x80
#define LED_ALL 0xFF
#define LED_6 0xFC
#define LED_NON 0

#define NUM_STATE	9
#define NUM_INPUT	3

portTickType short_timer;
portTickType click_timer;

u8 led_num = LED1;
u16 barled = 0;
u16 led1_sum = LED_NON;
u16 led2_sum = LED_NON;

// Actions

static
void f_ts(void *p) {
	short_timer = xTaskGetTickCount();
}

static
void f_left_on(void *p) {

	printf("((S))\n");

	if (led_num == LED1) {

		if (barled > LED_MIN) {
			barled /= 2;
			led1_sum += barled;
		}
		else if (barled == 0) {
			barled = LED_MAX;
			led1_sum = barled;
		}
		else if(barled == LED_MIN) {
			led1_sum = LED_ALL;
			led_num = LED2;
			barled = LED_MAX;
			led2_sum = barled;
		}

		writeb_virtual_io(BARLED1, led1_sum);
		writeb_virtual_io(BARLED2, led2_sum);
	}

	else {
		barled /= 2;
		led2_sum += barled;

		if (barled == LED_MIN)
			barled = LED_MIN;

		writeb_virtual_io(BARLED1, led1_sum);
		writeb_virtual_io(BARLED2, led2_sum);
	}
}

static
void f_led1(void *p) {
	printf("((L))\n");
	led_num = LED1;
	barled = LED_MIN;
	led1_sum = LED_ALL;
	writeb_virtual_io(BARLED1, led1_sum);
	writeb_virtual_io(BARLED2, 0);
}

static
void f_right_off(void *p) {
	printf("((SS))\n");
	if(led_num == LED2) {
		if(barled < LED_MAX) {
			led2_sum -= barled;
			barled *= 2;
		}
		else if(barled == LED_MAX) {
			led2_sum = LED_NON;
			led_num = LED1;
			barled = LED_MIN;
			led1_sum = LED_ALL;
		}
	}
	else {
		if(barled < LED_MAX) {
			led1_sum -= barled;
			barled *= 2;
		}
		else if(barled == LED_MAX) {
			led1_sum = LED_NON;
			barled = 0;
		}
	}

	writeb_virtual_io(BARLED1, led1_sum);
	writeb_virtual_io(BARLED2, led2_sum);
}

static
void f_left6(void *p) {
	printf("((SL))\n");
	led_num = LED1;
	led1_sum = LED_6;
	led2_sum = LED_NON;
	barled = 0x04;
	writeb_virtual_io(BARLED1, led1_sum);
	writeb_virtual_io(BARLED2, led2_sum);
}

static
void f_led1_2(void *p) {
	printf("((LS))\n");
	led_num = LED2;
	barled = LED_MIN;
	led1_sum = LED_ALL;
	led2_sum = LED_ALL;
	writeb_virtual_io(BARLED1, led1_sum);
	writeb_virtual_io(BARLED2, led2_sum);
}

static
void f_led_init(void *p) {
	printf("((LL))\n");
	led_num = LED1;
	barled = 0;
	led1_sum = LED_NON;
	led2_sum = LED_NON;
	writeb_virtual_io(BARLED1, led1_sum);
	writeb_virtual_io(BARLED2, led2_sum);
}

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO
};

struct state_machine_x SM[NUM_STATE] = {
// SW_ON              SW_OFF           TO
		{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, /* State 0 */
		{ 1, { 1, 3, 2 }, { NULL, NULL, NULL } }, /* State 1 */
		{ 0, { 2, 5, 2 }, { NULL, NULL, NULL } }, /* State 2 */
		{ 0, { 7, 0, 3 }, { f_ts, f_left_on, NULL } }, /* State 3 */
		{ 0, { 4, 0, 4 }, { NULL, f_left6, NULL } }, /* State 4 */
		{ 0, { 8, 0, 6 }, { f_ts, f_led1, NULL } }, /* State 5 */
		{ 0, { 6, 0, 6 }, { NULL, f_led_init, NULL } }, /* State 6 */
		{ 1, { 7, 0, 4 }, { NULL, f_right_off, NULL } }, /* State 7 */
		{ 1, { 8, 0, 6 }, { NULL, f_led1_2, NULL } } /* State 8 */
};

void Exp_3_Homework(void) {
	int state;
	int input;

	printf("Homework\n");

	state = 0;
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if ((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200) ) {
				input = TO;
				goto do_action;
				// Input happens
			}
		}
		if (state == 3) {
			if (((xTaskGetTickCount() - click_timer) < MSEC2TICK(200) )
			&& (NDS_SWITCH() & KEY_A)){
			input = SW_ON;
		}
		else
		input = SW_OFF;
	}
		if (state == 5) {
			if (((xTaskGetTickCount() - click_timer) < MSEC2TICK(200) )
			&& (NDS_SWITCH() & KEY_A)){
			input = SW_ON;
		}
		else
		input = SW_OFF;
	}

	else if(NDS_SWITCH() & KEY_A)
	input = SW_ON;
	else
	input = SW_OFF;

			/* Step 1: Do Action */
		do_action: if (SM[state].action[input])
			SM[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = SM[state].next_state[input];
		click_timer = xTaskGetTickCount();

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(50) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

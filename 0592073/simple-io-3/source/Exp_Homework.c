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

#define OFFSET_MOVEMENT 2
#define LED_OFF 0x00
#define LED_1ST 0x01
#define LED_8TH 0x80
#define LED_MAX 0xFF
// Actions

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO
};

static u8 LED1_offset = 0x00;
static u8 LED2_offset = 0x00;
u8 LED1_ON = FALSE;
u8 LED2_ON = FALSE;
portTickType start_time = 0;

static void f_ts(void *p) {
	start_time = xTaskGetTickCount();
}

static void f_led1(void *p) {
	printf("<S>\n");
	if (LED1_offset != LED_MAX) {
		LED1_offset = LED_8TH | (LED1_offset >> 1);

		writeb_virtual_io(BARLED1, LED1_offset);
		writeb_virtual_io(BARLED2, LED_OFF);

	} else {
		LED2_offset = LED_8TH | (LED2_offset >> 1);
		writeb_virtual_io(BARLED1, LED1_offset);
		writeb_virtual_io(BARLED2, LED2_offset);
	}

}

static void f_led2(void *p) {
	printf("<L>\n");
	LED1_offset = LED_MAX;
	LED2_offset = LED_OFF;
	writeb_virtual_io(BARLED1, LED1_offset);
	writeb_virtual_io(BARLED2, LED2_offset);
}

static void f_led3(void *p) {
	printf("<S-S>\n");
	if (LED2_offset == LED_OFF) {
		LED1_offset = LED1_offset << 1;
		writeb_virtual_io(BARLED1, LED1_offset);
		writeb_virtual_io(BARLED2, LED_OFF);
	} else {
		LED2_offset = LED2_offset << 1;
		writeb_virtual_io(BARLED1, LED1_offset);
		writeb_virtual_io(BARLED2, LED2_offset);
	}
}

static void f_led4(void *p) {
	printf("<S-L>\n");
	LED1_offset = 252;
	LED2_offset = LED_OFF;
	writeb_virtual_io(BARLED1, LED1_offset);
	writeb_virtual_io (BARLED2, LED2_offset);
}

static void f_led5(void *p) {
	printf("<L-S>\n");
	LED1_offset = LED_MAX;
	LED2_offset = LED_MAX;
	writeb_virtual_io(BARLED1, LED1_offset);
	writeb_virtual_io(BARLED2, LED2_offset);
}

static void f_led6(void *p) {
	printf("<L-L>\n");
	LED1_offset = LED_OFF;
	LED2_offset = LED_OFF;
	writeb_virtual_io(BARLED1, LED_OFF);
	writeb_virtual_io(BARLED2, LED_OFF);
}

struct state_machine_x Exam_SM_X[NUM_STATE] = {
// Fill here
		{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, /* state 1 */
		{ 1, { 1, 3, 2 }, { NULL, f_ts, NULL } }, /* state 2 */
		{ 0, { 2, 6, 0 }, { NULL, f_ts, NULL } }, /* state 3 */
		{ 1, { 4, 3, 0 }, { f_ts, NULL, f_led1 } },/* state 4 */
		{ 1, { 4, 0, 5 }, { NULL, f_led3, NULL } },/* state 5 */
		{ 0, { 5, 0, 0 }, { NULL, f_led4, NULL } },/* state 6 */
		{ 1, { 7, 6, 0 }, { f_ts, NULL, f_led2 } },/* state 7 */
		{ 1, { 7, 0, 8 }, { NULL, f_led5, NULL } },/* state 8 */
		{ 0, { 8, 0, 0 }, { NULL, f_led6, NULL } } };/* state 9 */

void Exp_3_Homework(void) {
	// variables
	int state;
	int input;
	printf("EXP_3_Homework\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (Exam_SM_X[state].check_timer) {
			if ((xTaskGetTickCount() - start_time) >= MSEC2TICK(200) ) {
				input = TO;
				goto do_action;
				// Input happens
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
		vTaskDelay(MSEC2TICK(50) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

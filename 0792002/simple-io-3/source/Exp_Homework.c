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

portTickType short_timer = 0;
portTickType start_timer = 0;
u16 barled = 0x00, position = BARLED1;
u8 dck = FALSE; //더블 클릭 검사 상태
u8 dc = FALSE; //더블 클릭 일 경우
int input;
#define NUM_STATE	7
#define NUM_INPUT	5

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (*action[NUM_INPUT])(void *p);
};
enum {
	SW_ON, SW_OFF, TO, NDB, DB
};

static void x_led1(void *p) {
	if(position==BARLED2 && barled == 0xFF);
	else
	barled = (barled >> 1) + 0x80;
	writeb_virtual_io(position, barled);
	if (position == BARLED1 && barled == 0xFF) {
		barled = 0x00;
		position = BARLED2;
	}
}

static void x_led2(void *p) {
	if(position==BARLED1 && barled == 0x00);
	else
	barled= (barled-0x80) <<1;
	writeb_virtual_io(position, barled);
	if (position == BARLED2 && barled == 0x00) {
		barled= 0xFF;
		position =BARLED1;
	}
}

static void x_led3(void *p) {
	position = BARLED1;
	barled = 0XFC;
	writeb_virtual_io(BARLED1, barled);
	writeb_virtual_io(BARLED2, 0);
}

static void x_led4(void *p) {
	position = BARLED1;
	barled = 0xFF;
	writeb_virtual_io(BARLED1, barled);
	writeb_virtual_io(BARLED2, 0);
}

static void x_led5(void *p) {
	position = BARLED2;
	barled = 0xFF;
	writeb_virtual_io(BARLED1, 0XFF);
	writeb_virtual_io(BARLED2, barled);
}

static void x_led6(void *p) {
	position = BARLED1;
	barled = 0;
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
}
static void ts(void *p) {
	start_timer = xTaskGetTickCount();
}

static void dts(void *p) {
	short_timer = xTaskGetTickCount();
	printf("dts");
	dck = TRUE;
	input = NULL;
}

struct state_machine_x SM[NUM_STATE] =
{ { 0, { 1, 0, 0, 0, 0 }, { ts, NULL,NULL, NULL, NULL } }, /*state 0*/
{ 1, { 1, 1, 4, 0, 2 }, { NULL, dts, NULL, x_led1, ts } }, /* state 1*/
{ 1, { 2, 0, 3, 2, 2 }, { NULL, x_led2, NULL, NULL, NULL } }, /* state 2*/
{ 0, { 3, 0, 3, 3, 3 }, { NULL, x_led3, NULL, NULL, NULL } }, /* state 3*/
{ 0, { 4, 4, 4, 0, 5 }, { NULL, dts, NULL, x_led4, ts } }, /* state 4*/
{ 1, { 5, 0, 6, 5, 5 }, { NULL, x_led5, NULL, NULL, NULL } }, /* state 5*/
{ 0, { 6, 0, 6, 6, 6, }, { NULL, x_led6, NULL, NULL, NULL } } /* state 6*/

};

void Exp_3_Homework(void) {
	int state;

	// variables
	printf("exp_3_homework");
	state = 0;
	position=BARLED1;
	barled=0x00;
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		if(state == 4 && dck == TRUE)
			SM[state].check_timer = 1;
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if (((xTaskGetTickCount() - start_timer) >= MSEC2TICK(200) )
			&&(dck ==FALSE)&& (state != 4)){
				input = TO;
				goto do_action;
			}

		if (((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200)) //더블 클릭 아님
				&&(dck ==TRUE)) {
				input = NDB;
				dc = dck = FALSE;
				if(state == 4) SM[state].check_timer = 0;
				goto do_action;
			}
		if (((xTaskGetTickCount() - short_timer) < MSEC2TICK(200)) //더블 클릭임
				&&(dck==TRUE) && (dc== TRUE)) {
				input = DB;
				dc = dck = FALSE;
				goto do_action;
			}
		}

	if (NDS_SWITCH() & KEY_A){
		input = SW_ON;
		if(dck == TRUE){
			dc = TRUE;
		printf(".");
		}
	}

	else{
		if (dck == FALSE){
			input = SW_OFF;
		}
	}

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
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed

}

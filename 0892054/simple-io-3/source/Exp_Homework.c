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
portTickType start_time_x = 0;
u16 barled;

int cur_led1 = 0x00;
int cur_led2 = 0x00;
int led_cnt=0;
#define NUM_STATE	9
#define NUM_INPUT	4

// Actions
void init(){
	cur_led1 = 0x00;
	cur_led2 = 0x00;
	led_cnt =0;

	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);

}
f_ts(void *p)
{
	start_time_x = xTaskGetTickCount();
}

end_ts(void *p)
{
	short_timer = xTaskGetTickCount();
}
static
void
S(void *p)
{
	if(led_cnt < 8 && led_cnt >=0){
		cur_led1 += (0x80 >> led_cnt);
		led_cnt++;
	}
	else if(led_cnt <16){
		cur_led2 += (0x80 >> led_cnt-8);
		led_cnt++;
	}

	printf("S\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}

static
void
L(void *p)
{
	cur_led1 = 0xFF;
	cur_led2 = 0x00;
	led_cnt = 8;

	printf("L\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}
static
void
S_S(void *p)
{
	if(led_cnt <= 8 && led_cnt > 0){
		cur_led1 -= (0x01 << 8-led_cnt);
		led_cnt--;

	}
	else if(led_cnt <= 16 && led_cnt > 0){

		cur_led2 -= (0x01 << 16-led_cnt);
		led_cnt--;
	}

	printf("SS\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}
static
void
S_L(void *p)
{
	cur_led1 = 0xFC;
	cur_led2 = 0x00;
	led_cnt = 6;

	printf("SL\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}
static
void
L_S(void *p)
{
	cur_led1 = 0xFF;
	cur_led2 = 0xFF;
	led_cnt = 16;

	printf("LS\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}

static
void
L_L(void *p)
{
	cur_led1 = 0x00;
	cur_led2 = 0x00;
	led_cnt = 0;

	printf("LL\n");
	writeb_virtual_io(BARLED1, cur_led1);
	writeb_virtual_io(BARLED2, cur_led2);
}

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO , STO};

struct state_machine_x SM[NUM_STATE] = {
		   // SW_ON              SW_OFF           TO
    { 0, { 1, 0, 0, 0 }, { f_ts, NULL, NULL, NULL },},    /* State 0 */
    { 1, { 1, 3, 2, 0 }, { NULL, end_ts, NULL, NULL },},
    { 0, { 2, 4, 0, 0 }, { NULL, end_ts, NULL, NULL },},
    { 2, { 5, 3, 0, 0 }, { f_ts, NULL, NULL, S },},
    { 2, { 6, 4, 0, 0 }, { f_ts, NULL, NULL, L },},
    { 1, { 5, 0, 7, 0 }, { NULL, S_S, NULL, NULL },},
    { 1, { 6, 0, 8, 0 }, { NULL, L_S, NULL, NULL },},
    { 0, { 7, 0, 0, 0 }, { NULL, S_L, NULL, NULL },},
    { 0, { 8, 0, 0, 0 }, { NULL, L_L, NULL, NULL },},
	// Fill here
};

void
Exp_3_Homework(void)
{
    int state;
    int input;

	printf("089054-HomeWork\n");

	state = 0;							// Initial State 0 : All LED Off
	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer==1) {
			if ((xTaskGetTickCount() - start_time_x) >= MSEC2TICK(300)) {
				input = TO;
				goto do_action;		// Input happens
			}
		}

		if (SM[state].check_timer ==2) {
			if ((xTaskGetTickCount() - short_timer) >= MSEC2TICK(200)) {
				input = STO;
				goto do_action;		// Input happens
			}
		}
		if (NDS_SWITCH() & KEY_A){
			input = SW_ON;
		}
		else
			input = SW_OFF;

		/* Step 1: Do Action */
do_action:
		if (SM[state].action[input]){
			SM[state].action[input](NULL);
		}
		/* Step 2: Set Next State */
		state = SM[state].next_state[input];
		if (NDS_SWITCH() & KEY_START){
			init();
			break;
		}
		vTaskDelay(MSEC2TICK(50));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}

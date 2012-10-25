/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */

#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sevencore_io.h>

#include <tpl_os.h>

#include "tpl_os_timeobj_kernel.h" 

//portTickType short_timer;
u16 barled;

#define NUM_STATE	9
#define NUM_INPUT	3

struct state_machine_x {
	int check_timer;
	int next_state[NUM_INPUT];
	void (* action[NUM_INPUT])(void *p);
};
enum { SW_ON, SW_OFF, TO };

static tpl_tick short_timer = 0;

void
led_out(void)
{
	writeb_virtual_io(BARLED1, barled >> 8);
	writeb_virtual_io(BARLED2, barled & 0xFF);
}
static
void
f_s(void *p)
{
	printf("<S>\n");
	barled >>= 1;
	barled |= 0x8000;
	led_out();
}

static
void
f_l(void *p)
{
	printf("<L>\n");
	barled = 0xFF00;
	led_out();
}

static
void
f_sl(void *p)
{
	printf("<SL>\n");
	barled = 0xFC00;
	led_out();
}

static
void
f_ls(void *p)
{
	printf("<LS>\n");
	barled = 0xFFFF;
	led_out();
}

static
void
f_ss(void *p)
{
	printf("<SS>\n");
	barled <<= 1;
	led_out();
}

static
void
f_ll(void *p)
{
	printf("<LL>\n");
	barled = 0;
	led_out();
}

/*
** TODO: have to be changed to use kind of APIs
*/
static 
tpl_tick
get_system_counter_date(void)
{
	extern VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc;
	return SystemCounter_counter_desc.current_date;
}

static
void
f_ts(void *p)
{
	short_timer = get_system_counter_date();
}

struct state_machine_x SM[NUM_STATE] = {
	{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } },			/* State 0 */
	{ 1, { 1, 2, 3 }, { NULL, f_ts, NULL } },			/* State 1 */
	{ 1, { 4, 2, 0 }, { f_ts, NULL, f_s  } },			/* State 2 */
	{ 0, { 3, 5, 3 }, { NULL, f_ts, NULL } },			/* State 3 */
	{ 1, { 4, 0, 6 }, { NULL, f_ss, NULL } },			/* State 4 */
	{ 1, { 7, 5, 0 }, { f_ts, NULL, f_l  } },			/* State 5 */
	{ 0, { 6, 0, 6 }, { NULL, f_sl, NULL } },			/* State 6 */
	{ 1, { 7, 0, 8 }, { NULL, f_ls, NULL } },			/* State 7 */
	{ 0, { 8, 0, 8 }, { NULL, f_ll, NULL } }			/* State 8 */
};

DeclareAlarm(EventLoopAlarm);
DeclareEvent(EventLoopEvent);

void
Exp_3_Homework(void)
{
	int state = 0, sstate = 0;
	int input, sw;

	printf("EXP_3_Homework\n");
	led_out();

	while (1) {
		printf("loop1\n");
		/* Step 0: Generate Input Event */
		if (SM[state].check_timer) {
			if ((get_system_counter_date() - short_timer) >= MSEC2TICK(200)) {
				printf("EVENT=TO\n");
				input = TO;
				goto do_action;		// Input Event Happens
			}
		}
		sw = NDS_SWITCH();

		if ((sstate == 0) && (sw & KEY_A)) {
			//printf("EVENT=SW_ON\n");
			input = SW_ON;
			sstate = 1;
		} else if ((sstate == 1) && !(sw & KEY_A)) {
			//printf("EVENT=SW_OFF\n");
			input = SW_OFF;
			sstate = 0;
		} else
			goto do_nothing;

		/* Step 1: Do Action */
do_action:
		if (SM[state].action[input])
			SM[state].action[input](NULL);

		/* Step 2: Set Next State */
		state = SM[state].next_state[input];

do_nothing:
		if (NDS_SWITCH() & KEY_START)
			break;
		if (SetRelAlarm(EventLoopAlarm, 50, 0) != E_OK) 
			printf("setrelalarm oops\n");
		if (WaitEvent(EventLoopEvent) != E_OK)
			printf("wait event oops\n");
		ClearEvent(EventLoopEvent);
	}
	while (NDS_SWITCH() & KEY_START) {
		SetRelAlarm(EventLoopAlarm, 10, 0);
		WaitEvent(EventLoopEvent);
		ClearEvent(EventLoopEvent);
	}

	// Wait while START KEY is being pressed
}

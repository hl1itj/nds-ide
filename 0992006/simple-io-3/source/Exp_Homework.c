<<<<<<< HEAD
/* Exp_Homework.c
*
* Created on: 2011. 9. 22.
* Author: Minsuk Lee
*/
=======
/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */
>>>>>>> a4d24ee626d460082994459bcd1fac047b1839c8

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"
<<<<<<< HEAD

portTickType start_time = 0;
u8 barled = 0x00;
u8 led = 0x80;
u8 Barled2_on = FALSE;

#define NUM_STATE 9
#define NUM_INPUT 3

static void f_sClick(void *p) {
printf("<S>\n");
if (Barled2_on == FALSE) {

barled = (barled >> 1) + led;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, 0x00);

if (barled == 0xFF) {
Barled2_on = TRUE;
barled = 0x00;
writeb_virtual_io(BARLED2, 0x80);
}

} else {

barled = (barled >> 1) + led;
writeb_virtual_io(BARLED1, 0xFF);
writeb_virtual_io(BARLED2, barled);

if (barled == 0xFF) {
writeb_virtual_io(BARLED1, 0xFF);
writeb_virtual_io(BARLED2, 0xFF);
}

}

}

static void f_lClick(void *p) {
printf("<L>\n");
barled = 0xFF;
Barled2_on = FALSE;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, 0);

}

static void f_ssdClick(void *p) {
printf("<SS>\n");

if (Barled2_on == TRUE) {

barled = (barled - led) << 1;
writeb_virtual_io(BARLED1, 0xFF);
writeb_virtual_io(BARLED2, barled);

if (barled == 0x00) {
Barled2_on = FALSE;
barled = 0xFF;
}
} else {
barled = (barled - led) << 1;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, 0x00);
}

}

static void f_sldClick(void *p) {
printf("<SL>\n");
barled = 0xFC;
Barled2_on = FALSE;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, 0);
}

static void f_lsdClick(void *p) {
printf("<LS>\n");
barled = 0xFF;
Barled2_on = TRUE;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, barled);
}

static void f_lldClick(void *p) {
printf("<LL>\n");
barled = 0x00;
Barled2_on = FALSE;
writeb_virtual_io(BARLED1, barled);
writeb_virtual_io(BARLED2, barled);
}

static void f_ts(void *p) {
start_time = xTaskGetTickCount();
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
{ 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, //state0
{ 1, { 1, 3, 2 }, { NULL, f_ts, NULL } }, //state1
{ 0, { 2, 4, 0 }, { NULL, f_ts, NULL } }, //state2
{ 1, { 5, 0, 3 }, { f_ts, f_sClick, NULL } }, //state3 Short Click
{ 1, { 7, 0, 4 }, { f_ts, f_lClick, NULL } }, //state4 Long Click
{ 1, { 5, 0, 6 }, { NULL, f_ssdClick, NULL } }, //state5 Short-Short Double Click
{ 0, { 6, 0, 0 }, { NULL, f_sldClick, NULL } }, //state6 Short-Long Double Click
{ 1, { 7, 0, 8 }, { NULL, f_lsdClick, NULL } }, //state7 Long-Short Double Click
{ 0, { 8, 0, 0 }, { NULL, f_lldClick, NULL } }, //state8 Long-Long Double Click
};

void Exp_3_Homework(void) {

int state;
int input;

printf("EXP_3_Homework\n");

state = 0;
writeb_virtual_io(BARLED1, 0);
writeb_virtual_io(BARLED2, 0);

while (1) {
/* Step 0: Generate Input Event */
if (SM[state].check_timer) {
if ((xTaskGetTickCount() - start_time) >= MSEC2TICK(200) ) {
input = TO;
goto do_action;
}
}
if (NDS_SWITCH() & KEY_A) {
input = SW_ON;
} else {
input = SW_OFF;
}

/* Step 1: Do Action */
do_action: if (SM[state].action[input]) {
SM[state].action[input](NULL);
}
/* Step 2: Set Next State */
state = SM[state].next_state[input];

if (NDS_SWITCH() & KEY_START)
break;
vTaskDelay(MSEC2TICK(50) );
}

while (NDS_SWITCH() & KEY_START)
vTaskDelay(MSEC2TICK(10) ); // Wait while START KEY is being pressed
=======
portTickType short_timer;
u16 barled;

#define NUM_STATE 1
#define NUM_INPUT 1

// Actions

struct state_machine_x {
 int check_timer; //
 int next_state[NUM_INPUT];
 void (* action[NUM_INPUT])(void *p);
};


static
void
f_led2_on(void *p)
{
 writeb_virtual_io(BARLED1, 0);
 writeb_virtual_io(BARLED2, 0x01); // 일단 실험용(move) - 1번째 것 켜지게
}

static
void
f_led2_off(void *p)
{
 writeb_virtual_io(BARLED1, 0);
 writeb_virtual_io(BARLED2, (0x01>>7));// 일단 실험용(move) - 8번째 것 켜지게
}


static
void
f_every_led_on(void *p)
{
 writeb_virtual_io(BARLED1, 0xFF);
 writeb_virtual_io(BARLED2, 0xFF); // 다 켜지게 (ok)
}


static
void
f_every_led_off(void *p)
{
 writeb_virtual_io(BARLED1, 0);
 writeb_virtual_io(BARLED2, 0); // 다 꺼지게 (ok)
}


static
void
f_all_led1_on(void *p)
{
 writeb_virtual_io(BARLED1, 0xFF);// LED1 다 켜지게
 writeb_virtual_io(BARLED2, 0);
}

static
void
f_led1_on(void *p)
{
 writeb_virtual_io(BARLED1, 0x01);//일단 실험용(move) - 1번째 것 켜지게
 writeb_virtual_io(BARLED2, 0);
}


static
void
f_ts(void *p)
{
	short_timer = xTaskGetTickCount();
}


enum { SW_ON, SW_OFF, TO };

struct state_machine_x SM[NUM_STATE] = {
 // 여기다가 작성!
/*
 // SW_ON              SW_OFF           TO//to = time out
  // timeout = 2번상태
  // 눌리면 1
  // 꺼지면 0
*/


     //스위치 스위치  타임아웃하고
     //누른상태 뗄상태  나서 뭐하냐?
  { 0, { 1, 0, 0 }, { f_ts, NULL, NULL } }, // state 0 - 기본값은 뭐...
  { 1, { 1, 0, 2 }, {f_ts , f_led2_on, f_ts} }, // state 1
              /*
             누르고 나서 - 바로떼는거하고
                바로안떼는 것도 잇고

                 더블클릭 처리도해야되고
                 시부럴

              */
  { 0, { 2, 0, 0 }, { f_ts, NULL, NULL } }  // state 2
            /*
             이게 스위치가 눌린 뒤 200ms지난 채 눌려있는 상태니까
             그 담에 떼는거하고

            */


};


void
Exp_3_Homework(void)
{
    // variables

 while (1) {
  /* Step 0: Generate Input Event */

  /* Step 1: Do Action */

  /* Step 2: Set Next State */

  if (NDS_SWITCH() & KEY_START)
   break;
  vTaskDelay(MSEC2TICK(50));
 }
 while (NDS_SWITCH() & KEY_START)
  vTaskDelay(MSEC2TICK(10));  // Wait while START KEY is being pressed
>>>>>>> a4d24ee626d460082994459bcd1fac047b1839c8
}

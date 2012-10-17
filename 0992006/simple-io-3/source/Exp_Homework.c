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
}

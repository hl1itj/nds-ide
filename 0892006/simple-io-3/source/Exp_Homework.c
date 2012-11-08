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

u16 barled1 = 0x00;
u16 barled2 = 0x00;

#define NUM_STATE 9

#define NUM_INPUT 3

// Actions

static

void f_led_S(void *p) {

    u8 tmpled;

    if(barled1==0x00)
        barled1=0x80;
    else if(barled1==0xff && barled2==0){
        barled2=0x80;
    }
    else if(barled1==0xff && barled2!=0){
        tmpled = barled2 >> 1;
        barled2 = tmpled | barled2;

    }

    else{
        tmpled = barled1 >> 1;
        barled1 = tmpled | barled1;
    }
    writeb_virtual_io(BARLED1, barled1);

    writeb_virtual_io(BARLED2, barled2);


}

static

void f_led_SS(void *p) {
    u8 tmpled;
        if(barled1==0xff && barled2!=0){
            tmpled = barled2<<1;
            barled2 = tmpled;
        }
        else if(barled1!=0 && barled2==0){
            tmpled = barled1 << 1;
            barled1 = tmpled ;
        }

    writeb_virtual_io(BARLED1, barled1);
    writeb_virtual_io(BARLED2, barled2);

}

static

void f_led_SL(void *p) {

    barled1 = 0xFC;
    barled2 = 0x00;
    writeb_virtual_io(BARLED1,barled1);

    writeb_virtual_io(BARLED2, barled2);

}

static

void f_led_L(void *p) {

    barled1 = 0xFF;
    barled2 = 0x00;
    writeb_virtual_io(BARLED1, barled1);

    writeb_virtual_io(BARLED2, barled2);

}

static

void f_led_LS(void *p) {

    barled1 = 0xFF;
    barled2 = 0xFF;
    writeb_virtual_io(BARLED1, barled1);

    writeb_virtual_io(BARLED2, barled2);

}

static

void f_led_LL(void *p) {

	barled1 = 0x00;
	barled2 = 0x00;

    writeb_virtual_io(BARLED1, barled1);

    writeb_virtual_io(BARLED2, barled2);



}

static

void f_ts(void *p) {

    short_timer = xTaskGetTickCount();

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

        { 0, { 1, 0, 0 }, { f_ts, NULL, NULL } },
        { 1, { 1, 3, 2 }, { NULL,f_ts, NULL } },
        { 0, { 2, 4, 2 }, { NULL, f_ts, NULL } },
        { 1, {5, 3, 0 }, { f_ts, NULL, f_led_S } },
        { 1, { 6, 4, 0 }, { f_ts,NULL, f_led_L } },
        { 1, { 5, 0, 7 }, { NULL, f_led_SS, NULL } },
        { 1, { 6, 0, 8 }, { NULL, f_led_LS, NULL } },
        { 0, { 7, 0, 7 }, { NULL,f_led_SL, NULL } },
        { 0, { 8, 0, 8 }, { NULL, f_led_LL, NULL } }

};

void Exp_3_Homework(void) {

    int state;
    int input;

    printf("EXP_3_HomeWork\n");

    barled1 = 0x00;
    barled2 = 0x00;

    state = 0;       // Initial State 0 : All LED Off
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

        if (NDS_SWITCH() & KEY_A)
            input = SW_ON;
        else
            input = SW_OFF;

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
        vTaskDelay(MSEC2TICK(10) );  // Wait while START KEY is being pressed

}

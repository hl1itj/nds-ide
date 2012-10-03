/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */

#include <stdio.h>			// C-Standard Header
#include <time.h>
#include <stdlib.h>

#include "FreeRTOS.h"		// Free RTOS Headers
#include "task.h"

#include <nds.h>			// NDS / Sevencore Board Headers
#include <sevencore_io.h>
#include "realio.h"

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void
Exp_1_Homework_A(void)
{
	u16 sw;
	u8  led_state = 0x08;
	int  key_pressed=0;
	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		writeb_virtual_io(BARLED1, led_state);
		sw = NDS_SWITCH();

		if ((key_pressed==0) && (sw & KEY_LEFT) && (led_state<=0x40)){
			key_pressed=1;
			led_state  *=2;
		}

		if ((key_pressed==0) && (sw & KEY_RIGHT) && (led_state>=0x02)){
			key_pressed=1;
			led_state  /=2;
		}

		if((key_pressed==1) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)))
			key_pressed=0;

		if (sw & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}// Wait while START KEY is being pressed

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void
Exp_1_Homework_B(void)
{
	u16 sw;
	u8  led_state1 = 0x08;
	u8  led_state2 = 0x00;
	int  key_pressed=0;

	while (1) {
		writeb_virtual_io(BARLED1, led_state1);
		writeb_virtual_io(BARLED2, led_state2);

		sw = NDS_SWITCH();
		if ((key_pressed==0) && (sw & KEY_LEFT)){
			key_pressed=1;
			if(led_state1==0x80){
				led_state1=0x00;
				led_state2=0x01;
			}
			else if(led_state2==0x80){
				led_state2=0x00;
				led_state1=0x01;
			}
			else{
				led_state1 *=2;
				led_state2 *=2;
			}
		}

		if ((key_pressed==0) && (sw & KEY_RIGHT)){
			key_pressed=1;
			if(led_state1==0x01){
				led_state1=0x00;
				led_state2=0x80;
			}
			else if(led_state2==0x01){
				led_state2=0x00;
				led_state1=0x80;
			}
			else{
				led_state1 /=2;
				led_state2 /=2;
			}
		}

		if((key_pressed==1) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)))
			key_pressed=0;
		if (sw & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

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
// 일단 A는 finish
/*
 *
 * */
void
Exp_1_Homework_A(void)
{
	u16 sw;
	u8 led_state = 0x01;
	u8  key_pressed = FALSE;
	writeb_virtual_io(BARLED1, led_state);

	while(1){
		sw = NDS_SWITCH();

		// left section
		if (((key_pressed == FALSE) && (sw & KEY_LEFT))) {
			key_pressed = TRUE;
			if((led_state == (0x01 << 7))){
				led_state = 0x01 <<7 ;
			}
			else
				led_state = led_state << 1;
			writeb_virtual_io(BARLED1, led_state);
		}
		if (((key_pressed == TRUE) && (sw & KEY_LEFT))) {
			key_pressed = FALSE;

		}

		//right section
		if (((key_pressed == FALSE) && (sw & KEY_RIGHT))) {
			key_pressed = TRUE;
			if((led_state == (0x01))){
				led_state = 0x01;}
			else{
				led_state = led_state >> 1;
			}
			writeb_virtual_io(BARLED1, led_state);
		}
		if (((key_pressed == TRUE) && (sw & KEY_RIGHT))) {
			key_pressed = FALSE;

		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);

	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}





// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void
Exp_1_Homework_B(void)
{
	u16 sw;
	u8 led_state = 0x01;
	u8  key_pressed = FALSE;
	writeb_virtual_io(BARLED2, led_state);
	writeb_virtual_io(BARLED1, led_state);

	while(1){
		sw = NDS_SWITCH();

		// left section
		if (((key_pressed == FALSE) && (sw & KEY_L))) {
			key_pressed = TRUE;

			led_state = led_state << 1;
			writeb_virtual_io(BARLED1, led_state);
		}
		if (((key_pressed == TRUE) && (sw & KEY_L))) {
			key_pressed = FALSE;

		}

		//right section
		if (((key_pressed == FALSE) && (sw & KEY_R))) {
			key_pressed = TRUE;
			if((led_state == (0x01))){
				led_state = 0x01;}
			else{
				led_state = led_state >> 1;
			}
			writeb_virtual_io(BARLED1, led_state);
		}
		if (((key_pressed == TRUE) && (sw & KEY_R))) {
			key_pressed = FALSE;

		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);

	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}



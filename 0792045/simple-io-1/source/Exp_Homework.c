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
void Exp_1_Homework_A(void) {
	u16 sw;
	u16 led_state = 0x01;
	u8 R_key_pressed = FALSE;
	u8 L_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	// writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();

		if (((R_key_pressed == FALSE) && (sw & KEY_RIGHT) && led_state != 0x01)) {
			led_state = led_state >> 1;
			R_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		} else if ((R_key_pressed == TRUE) && !(sw & KEY_RIGHT))
			R_key_pressed = FALSE;

		if (((L_key_pressed == FALSE) && (sw & KEY_LEFT) && led_state != 0x80)) {
			led_state = led_state << 1;
			L_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		} else if ((L_key_pressed == TRUE) && !(sw & KEY_LEFT))
			L_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);        // Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	u16 led_state = 0x01;

	u8 R_key_pressed = FALSE;
	u8 L_key_pressed = FALSE;

	u8 state = FALSE; // TRUE 오른쪽 FALSE 왼쪽

	writeb_virtual_io(BARLED1, led_state);
	while (1) {
		sw = NDS_SWITCH();

		if ((R_key_pressed == FALSE) && (sw & KEY_RIGHT) && !((led_state == 0x01)
				&& (state == TRUE))) {
			if ((led_state == 0x01) && (state != TRUE)) { //왼쪽에서 오른쪽으로 넘어감
				led_state = 0x80;
				state = TRUE;
				writeb_virtual_io(BARLED1, 0x00);
			} else
				led_state = led_state >> 1;

			if (state == TRUE)
				writeb_virtual_io(BARLED2, led_state);
			else
				writeb_virtual_io(BARLED1, led_state);
			R_key_pressed = TRUE;
		} else if ((R_key_pressed == TRUE) && !(sw & KEY_RIGHT))
			R_key_pressed = FALSE;


		if ((L_key_pressed == FALSE) && (sw & KEY_LEFT) && !((led_state == 0x80)
				&& (state == FALSE))) {
			if ((led_state == 0x80) && (state != FALSE)) { //오른쪽에서 왼쪽으로 넘어감
				led_state = 0x01;
				state = FALSE;
				writeb_virtual_io(BARLED2, 0x00);
			} else
				led_state = led_state << 1;

			if (state == FALSE)
				writeb_virtual_io(BARLED1, led_state);
			else
				writeb_virtual_io(BARLED2, led_state);
			L_key_pressed = TRUE;
		}  else if ((L_key_pressed == TRUE) && !(sw & KEY_LEFT))
			L_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

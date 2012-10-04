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
<<<<<<< HEAD
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();

		if (((key_pressed == FALSE) && (sw & KEY_RIGHT) && led_state != 0x01)) {
			led_state = led_state >> 1;
			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		}
		if ((key_pressed == TRUE) && (sw & KEY_RIGHT))
			key_pressed = FALSE;

		if (((key_pressed == FALSE) && (sw & KEY_LEFT) && led_state != 0x80)) {
			led_state = led_state << 1;
			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		}
		if ((key_pressed == TRUE) && (sw & KEY_LEFT))
			key_pressed = FALSE;
=======

	u8 R_key_pressed = FALSE;
	u8 L_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0x00);
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
>>>>>>> 8bdbe889a783a5b12b4bdef55375e88438caba09

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
<<<<<<< HEAD
		vTaskDelay(10);		// Wait while START KEY is being pressed
=======
		vTaskDelay(10);        // Wait while START KEY is being pressed
>>>>>>> 8bdbe889a783a5b12b4bdef55375e88438caba09
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	u16 led_state = 0x01;
<<<<<<< HEAD
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();

		if (((key_pressed == FALSE) && (sw & KEY_RIGHT) && led_state != 0x01)) {
			if (led_state == 0x01 && BARLED1 == 0x01) {
				led_state = 0x80;
				key_pressed = TRUE;
				writeb_virtual_io(BARLED1, 0);
				writeb_virtual_io(BARLED2, led_state);
			} else if (BARLED1 == 0x01) {
				led_state = led_state >> 1;
				key_pressed = TRUE;
				writeb_virtual_io(BARLED2, led_state);
			} else {
				led_state = led_state >> 1;
				key_pressed = TRUE;
				writeb_virtual_io(BARLED1, led_state);
			}
		}

		if ((key_pressed == TRUE) && (sw & KEY_RIGHT))
			key_pressed = FALSE;

		if (((key_pressed == FALSE) && (sw & KEY_LEFT) && led_state != 0x80)) {
			led_state = led_state << 1;
			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		}
		if ((key_pressed == TRUE) && (sw & KEY_LEFT))
			key_pressed = FALSE;
=======

	u8 R_key_pressed = FALSE;
	u8 L_key_pressed = FALSE;

	u8 state = FALSE; // TRUE 오른쪽 FALSE 왼쪽

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0x00);
	while (1) {
		sw = NDS_SWITCH();

		if ((R_key_pressed == FALSE) && (sw & KEY_R)) {
			if ((led_state == 0x01) && (state != TRUE)) { //왼쪽에서 오른쪽으로 넘어감
				led_state = 0x80;
				state = TRUE;
				writeb_virtual_io(BARLED1, 0x00);
			} else if ((led_state == 0x01) && (state != FALSE)){//오른쪽에서 왼쪽으로 넘어감
				led_state = 0x80;
				state = FALSE;
				writeb_virtual_io(BARLED2, 0x00);
			}
			else
				led_state = led_state >> 1;

			if (state == TRUE)
				writeb_virtual_io(BARLED2, led_state);
			else
				writeb_virtual_io(BARLED1, led_state);
			R_key_pressed = TRUE;
		} else if ((R_key_pressed == TRUE) && !(sw & KEY_R))
			R_key_pressed = FALSE;


		if ((L_key_pressed == FALSE) && (sw & KEY_L)) {
			if ((led_state == 0x80) && (state != FALSE)) { //오른쪽에서 왼쪽으로 넘어감
				led_state = 0x01;
				state = FALSE;
				writeb_virtual_io(BARLED2, 0x00);
			}else if ((led_state == 0x80) && (state != TRUE)) { //왼쪽에서 오른감쪽으로 넘어
				led_state = 0x01;
				state = TRUE;
				writeb_virtual_io(BARLED1, 0x00);
			} else
				led_state = led_state << 1;

			if (state == FALSE)
				writeb_virtual_io(BARLED1, led_state);
			else
				writeb_virtual_io(BARLED2, led_state);
			L_key_pressed = TRUE;
		}  else if ((L_key_pressed == TRUE) && !(sw & KEY_L))
			L_key_pressed = FALSE;
>>>>>>> 8bdbe889a783a5b12b4bdef55375e88438caba09

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
<<<<<<< HEAD
		vTaskDelay(10);		// Wait while START KEY is being pressed

=======
		vTaskDelay(10);
>>>>>>> 8bdbe889a783a5b12b4bdef55375e88438caba09
}

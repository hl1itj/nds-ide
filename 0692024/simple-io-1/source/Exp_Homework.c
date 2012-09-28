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
	u8 led_state = 0x08;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);
	while (1) {

		sw = NDS_SWITCH();

		if (((key_pressed == FALSE) && (sw & KEY_LEFT))) {

			key_pressed = TRUE;

			if (led_state >= 0x80) {
				key_pressed = FALSE;
			} else {
				led_state = led_state * 2;
			}
			writeb_virtual_io(BARLED1, led_state);

		}

		if (((key_pressed == FALSE) && (sw & KEY_RIGHT))) {
			key_pressed = TRUE;
			if (led_state <= 0x01) {
				key_pressed = FALSE;
			} else {
				led_state = led_state / 2;
			}
			writeb_virtual_io(BARLED1, led_state);

		}

		if ((key_pressed == TRUE) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	u8 Rled_state = 0x08;
	u8 Gled_state = 0;
	u8 key_pressed = FALSE;
	int swap = 0; // swap = 0 = BARLED1, else BARLED2

	writeb_virtual_io(BARLED1, Rled_state);
	writeb_virtual_io(BARLED2, Gled_state);
	while (1) {

		sw = NDS_SWITCH();

		if (((key_pressed == FALSE) && (sw & KEY_L))) {

			key_pressed = TRUE;

			if (Rled_state == 0x80) {
				Rled_state = 0;
				Gled_state = 0x01;
				swap = 1;
			} else {
				if (swap == 0) {
					Rled_state = Rled_state * 2;
				} else {
					if (Gled_state == 0x80) {
						Rled_state = 0x01;
						Gled_state = 0;
						swap = 0;
					} else {
						Gled_state = Gled_state * 2;
					}
				}
			}

			writeb_virtual_io(BARLED1, Rled_state);
			writeb_virtual_io(BARLED2, Gled_state);

		}

		if (((key_pressed == FALSE) && (sw & KEY_R))) {
			key_pressed = TRUE;

			if (Rled_state == 1) {
				Rled_state = 0;
				Gled_state = 0x80;
				swap = 1;
			} else {
				if (swap == 0) {
					Rled_state = Rled_state / 2;
				} else {
					if (Gled_state == 1) {
						Rled_state = 0x80;
						Gled_state = 0;
						swap = 0;
					} else {
						Gled_state = Gled_state / 2;
					}
				}
			}

			writeb_virtual_io(BARLED1, Rled_state);
			writeb_virtual_io(BARLED2, Gled_state);

		}

		if ((key_pressed == TRUE) && (!(sw & KEY_L)) && (!(sw & KEY_R)))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
	vTaskDelay(10);		// Wait while START KEY is being pressed
}

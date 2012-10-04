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

void Exp_1_Homework_A(void) {
	u16 sw;
	u8 key_pressed = FALSE;
	short int led_state = 0x0001;

	//writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED1, led_state);

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED2, 0);

		if (((key_pressed == FALSE) && (sw & KEY_LEFT))) {
			key_pressed = TRUE;
			if (!(led_state & 0x0080))
				led_state = led_state * 2;
			writeb_virtual_io(BARLED1, led_state);

		}

		if ((key_pressed == TRUE) && (sw == 0))
			key_pressed = FALSE;

		if (((key_pressed == FALSE) && (sw & KEY_RIGHT))) {
			key_pressed = TRUE;
			if (led_state != 0x0001) //max right
				led_state = led_state / 2;
			writeb_virtual_io(BARLED1, led_state);
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
void Exp_1_Homework_B(void) {
	u16 sw;
	u8 key_pressed = FALSE;
	u16 cur_barled = BARLED1;
	short int led_state = 0x0001;

	//writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(cur_barled, led_state);

	while (1) {
		sw = NDS_SWITCH();

		if ((key_pressed == FALSE) && ((sw & KEY_L) || (sw & KEY_R))) {
			key_pressed = TRUE;

			if (sw & KEY_L) {

				if (!(led_state == 0x0080))
					led_state = led_state * 2;

				else {
					if ((led_state == 0x0080) && (cur_barled == BARLED1)) { //If it is max left and state of LED is pressed 'KEY_L'
						writeb_virtual_io(cur_barled, 0);
						led_state = 0x0001;
						cur_barled = BARLED2;
					}

					if ((led_state == 0x0080) && (cur_barled == BARLED2)) { //If it is max right and state of LED is pressed 'KEY_R'
						writeb_virtual_io(cur_barled, 0);
						led_state = 0x0001;
						cur_barled = BARLED1;
					}
				}
			}
			if (sw & KEY_R) {

				if (!(led_state == 0x0001))
					led_state = led_state / 2;

				else {
					if ((led_state == 0x0001) && (cur_barled == BARLED2)) { //If it is max right and state of LED is pressed 'KEY_R'
						writeb_virtual_io(cur_barled, 0);
						led_state = 0x0080;
						cur_barled = BARLED1;
					}

					if ((led_state == 0x0001) && (cur_barled == BARLED1)) { //If it is max left and state of LED is pressed 'KEY_L'
						writeb_virtual_io(cur_barled, 0);
						led_state = 0x0080;
						cur_barled = BARLED2;
					}
				}

			}

			writeb_virtual_io(cur_barled, led_state);
		}

		if ((key_pressed == TRUE) && (sw == 0))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);         // Wait while START KEY is being pressed
}

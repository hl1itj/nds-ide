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

	u8 led_state = 0x80;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();
		if (key_pressed == TRUE && (!(sw & KEY_RIGHT)) && (!(sw & KEY_LEFT))) {
			key_pressed = FALSE;
		}
		if (NDS_SWITCH() & KEY_RIGHT && key_pressed == FALSE
				&& led_state >= 0x02) {
			led_state = led_state >> 1;
			writeb_virtual_io(BARLED1, led_state);
			key_pressed = TRUE;

		}

		if (NDS_SWITCH() & KEY_LEFT && key_pressed == FALSE
				&& led_state <= 0x40) {
			led_state = led_state << 1;
			writeb_virtual_io(BARLED1, led_state);
			key_pressed = TRUE;
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

	u8 led_state = 0x80;
	u8 key_pressed = FALSE;
	u8 onoff = TRUE;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();
		//연속적인 키 입력을 체크
		if (key_pressed == TRUE && (!(sw & KEY_RIGHT)) && (!(sw & KEY_LEFT)) && (!(sw & KEY_R)) && (!(sw & KEY_L))) {
			key_pressed = FALSE;
		}
		//오른쪽 방향키 입력을 체크
		if (NDS_SWITCH() & KEY_RIGHT && key_pressed == FALSE
				&& led_state >= 0x02) {
			led_state = led_state >> 1;
			if (onoff == TRUE) {
				writeb_virtual_io(BARLED1, led_state);
				writeb_virtual_io(BARLED2, 0);
			} else {
				writeb_virtual_io(BARLED2, led_state);
				writeb_virtual_io(BARLED1, 0);
			}
			key_pressed = TRUE;

		}
		//왼쪽 방향키 입력을 체크
		if (NDS_SWITCH() & KEY_LEFT && key_pressed == FALSE
				&& led_state <= 0x40) {
			led_state = led_state << 1;
			if (onoff == TRUE) {
				writeb_virtual_io(BARLED1, led_state);
				writeb_virtual_io(BARLED2, 0);
			} else {
				writeb_virtual_io(BARLED2, led_state);
				writeb_virtual_io(BARLED1, 0);
			}
			key_pressed = TRUE;
		}
		//R버튼 입력을 체크
		if (NDS_SWITCH() & KEY_R && key_pressed == FALSE && led_state == 0x01) {
			led_state = 0x80;
			key_pressed=TRUE;

			if (onoff == TRUE)
				onoff = FALSE;
			else
				onoff = TRUE;

			if (onoff == TRUE) {
				writeb_virtual_io(BARLED1, led_state);
				writeb_virtual_io(BARLED2, 0);
			} else {
				writeb_virtual_io(BARLED2, led_state);
				writeb_virtual_io(BARLED1, 0);
			}
		}
		//L버튼 입력을 체크
		if (NDS_SWITCH() & KEY_L && key_pressed == FALSE && led_state == 0x80) {
			led_state = 0x01;
			key_pressed=TRUE;

			if (onoff == TRUE)
				onoff = FALSE;
			else
				onoff = TRUE;

			if (onoff == TRUE) {
				writeb_virtual_io(BARLED1, led_state);
				writeb_virtual_io(BARLED2, 0);
			} else {
				writeb_virtual_io(BARLED2, led_state);
				writeb_virtual_io(BARLED1, 0);
			}

		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

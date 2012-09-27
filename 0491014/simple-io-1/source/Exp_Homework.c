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
	u8 led_state = 0x01;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();
		if ((sw == KEY_RIGHT) && (led_state != 0x01))
			led_state = led_state >>1;

		if ((sw == KEY_LEFT) && (led_state != 0x80))
			led_state = led_state <<1;

		writeb_virtual_io(BARLED1, led_state);

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void
Exp_1_Homework_B(void)
{
	u16 sw;
	u8 a_b = TRUE;
	u8 led_state = 0x01;

	writeb_virtual_io(BARLED1, led_state);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();
		if ((sw == KEY_R) && (led_state != 0x01))
			led_state = led_state >>1;
		else if ((sw == KEY_R) && (led_state == 0x01)) {
			led_state = 0x80;
			if (a_b)
				a_b = FALSE;
			else
				a_b = TRUE;
		}
		if ((sw == KEY_L) && (led_state != 0x80))
			led_state = led_state <<1;
		else if ((sw == KEY_L) && (led_state == 0x80)) {
			led_state = 0x01;
			if (a_b)
				a_b = FALSE;
			else
				a_b = TRUE;
		}
		if (a_b){
			writeb_virtual_io(BARLED1, led_state);
			writeb_virtual_io(BARLED2, 0);
		}
		else{
			writeb_virtual_io(BARLED1, 0);
			writeb_virtual_io(BARLED2, led_state);
		}
		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);

}

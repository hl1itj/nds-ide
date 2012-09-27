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
	u8 i = 0x80;

	while (1) {
		sw = NDS_SWITCH();

		if ((NDS_SWITCH() & KEY_LEFT) && (i < 0x80)) {
			i = i << 1;
		}
		else if ((NDS_SWITCH() & KEY_RIGHT) && (i > 0x01)) {
			i = i >> 1;
		}
		writeb_virtual_io(BARLED1, i);
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	u16 i = 0x8000;

	while (1) {
		sw = NDS_SWITCH();

		if (NDS_SWITCH() & KEY_LEFT) {
			if (i == 0x8000)
				i = 0x0001;
			else
				i = i << 1;
		}
		else if (NDS_SWITCH() & KEY_RIGHT) {
			if (i == 0x0001)
				i = 0x8000;
			else
				i = i >> 1;
		}
		if (i <= 0x8000 && i >= 0x0100) {
			writeb_virtual_io(BARLED1, i >> 8);
			writeb_virtual_io(BARLED2, 0x0000);
		}
		else {
			writeb_virtual_io(BARLED1, 0x0000);
			writeb_virtual_io(BARLED2, i);
		}
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

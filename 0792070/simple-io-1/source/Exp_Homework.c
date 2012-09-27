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
	u16 num = 0x01;
	u8 keypressed = FALSE;
	u16 sw;

	writeb_virtual_io(BARLED1, num);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (!keypressed && (sw & KEY_LEFT)) {
			if (num == 0x80)
				continue;
			num = num << 1;
			keypressed = TRUE;
		} else if (!keypressed && (sw & KEY_RIGHT)) {
			if (num == 0x01)
				continue;
			num = num >> 1;
			keypressed = TRUE;
		}

		writeb_virtual_io(BARLED1, num);

		if (keypressed && (!(sw & KEY_LEFT) && !(sw & KEY_RIGHT)))
			keypressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 num = 0x80;
	u8 keypressed = FALSE;
	u16 sw;
	u16 preposition = BARLED1;

	writeb_virtual_io(BARLED1, num);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (!keypressed && (sw & KEY_L)) {
			if (keypressed && (sw & KEY_L))
				continue;
			num = num << 1;
			keypressed = TRUE;
//			if (num >= 0x80)
//				num = 0x80;
		} else if (!keypressed && (sw & KEY_R)) {
			if (keypressed && (sw & KEY_R))
				continue;
			num = num >> 1;
			keypressed = TRUE;
//			if (num <= 0x01)
//				num = 0x01;
		}

		if (keypressed && (!(sw & KEY_L) && !(sw & KEY_R)))
			keypressed = FALSE;

		if ((sw & KEY_L) && preposition == BARLED1 && num > 0x80) {
			writeb_virtual_io(preposition, 0);
			preposition = BARLED2;
			num = 0x01;
		} else if ((sw & KEY_R) && preposition == BARLED2 && num < 0x01) {
			writeb_virtual_io(preposition, 0);
			preposition = BARLED1;
			num = 0x80;
		} else if ((sw & KEY_R) && preposition == BARLED1 && num < 0x01) {
			writeb_virtual_io(preposition, 0);
			preposition = BARLED2;
			num = 0x80;
		} else if ((sw & KEY_L) && preposition == BARLED2 && num > 0x80) {
			writeb_virtual_io(preposition, 0);
			preposition = BARLED1;
			num = 0x01;
		}

		writeb_virtual_io(preposition, num);

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

/*
 * Exp_Sample.c
 *
 *  Created on: 2010. 10. 6.
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

// As is
void
Exp_1_Sample_A(void)
{
	u16 sw;

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, sw >> 8);
		writeb_virtual_io(BARLED2, sw & 0xFF);

		if (NDS_SWITCH() & KEY_START) //
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// Toggle
void
Exp_1_Sample_B(void)
{
	u16 sw;
	u8  led_state = FALSE;
	u8  key_pressed = FALSE;

	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();

		if (((key_pressed == FALSE) && (sw & KEY_R))) {
			key_pressed = TRUE;
			led_state = !led_state;
			writeb_virtual_io(BARLED2, led_state);
		}

		if ((key_pressed == TRUE) && (!(sw & KEY_R)))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

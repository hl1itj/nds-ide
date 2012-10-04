#include <stdio.h>            // C-Standard Header
#include <time.h>
#include <stdlib.h>

#include "FreeRTOS.h"        // Free RTOS Headers
#include "task.h"

#include <nds.h>            // NDS / Sevencore Board Headers
#include <sevencore_io.h>
#include "realio.h"

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void Exp_1_Homework_A(void) {
	u16 sw, num = 0x08;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, num);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (key_pressed == FALSE && (sw & KEY_LEFT) && num != 0x80) {
			key_pressed = TRUE;
			num = num << 1;
			writeb_virtual_io(BARLED1, num);
		}
		if (key_pressed == FALSE && (sw & KEY_RIGHT) && num != 0x01) {
			key_pressed = TRUE;
			num = num >> 1;
			writeb_virtual_io(BARLED1, num);
		}
		if ((key_pressed == TRUE) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)))
			key_pressed = FALSE;

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
	u16 sw, num=0x80 , position = BARLED1;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, num);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (key_pressed == FALSE && (sw & KEY_L) && num != 0x80) {
			key_pressed = TRUE;
			num = num << 1;
			writeb_virtual_io(position, num);
		}
		if (key_pressed == FALSE && (sw & KEY_R) && num != 0x01) {
			key_pressed = TRUE;
			num = num >> 1;
			writeb_virtual_io(position, num);
		}
		if (key_pressed == FALSE && (sw & KEY_L)
				&& num == 0x80&& position == BARLED1) {
			key_pressed = TRUE;
			position = BARLED2;
			num = 0x01;
			writeb_virtual_io(BARLED1, 0);
			writeb_virtual_io(position, num);
		}
		if (key_pressed == FALSE && (sw & KEY_R) && position == BARLED2
				&& num == 0x01) {
			key_pressed = TRUE;
			position = BARLED1;
			num = 0x80;
			writeb_virtual_io(BARLED2, 0);
			writeb_virtual_io(position, num);
		}

		if (key_pressed == FALSE && (sw & KEY_L)
				&& num == 0x80&& position == BARLED2) {
			key_pressed = TRUE;
			position = BARLED1;
			num = 0x01;
			writeb_virtual_io(BARLED2, 0);
			writeb_virtual_io(position, num);
		}
		if (key_pressed == FALSE && (sw & KEY_R)
				&& num == 0x01&& position == BARLED1) {
			key_pressed = TRUE;
			position = BARLED2;
			num = 0x80;
			writeb_virtual_io(BARLED1, 0);
			writeb_virtual_io(position, num);
		}
		if ((key_pressed == TRUE) && (!(sw & KEY_L)) && (!(sw & KEY_R)))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

//#define MAX_UPON 1024

u8 temp[NUM_7SEG_LED];

static void Init_LED() {

	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

}

static void Init_Temp() {

	int i;

	for (i = 0; i < NUM_7SEG_LED; i++) {

		temp[i] = 0x80 + (i << 4);
	}

}

void Exp_4_Homework_A(void) {
	// Variables
	u8 key, scan = 0;
	u8 position[NUM_7SEG_LED] =
			{ 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
	u8 swBtn_check = FALSE;
	u8 sw1_key_pressed = FALSE;
	u8 key_pressed = FALSE;

	int i;

	Init_Temp();
	Init_LED();

	while (1) {

		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:

			if (sw1_key_pressed == FALSE) {

				sw1_key_pressed = TRUE;

				if (swBtn_check == FALSE)
					swBtn_check = TRUE;
				else
					swBtn_check = FALSE;

				Init_Temp();
				Init_LED();

				sw1_key_pressed = TRUE;

			}

			break;

		}

		if (key_pressed == FALSE) {

			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;

			switch (readb_virtual_io(KEY_MATRIX)) {

			case 8:
				key += 1;
				break;
			case 4:
				key += 2;
				break;
			case 2:
				key += 3;
				break;
			case 1:
				key += 4;
				if (key == 16)
					key = 0;
				break;
			default:
				key = 255;
				break;
			}

			scan++;
			if (scan == 4)
				scan = 0;

			if (key < 16) {

				if (swBtn_check == TRUE) {

					for (i = 0; i < NUM_7SEG_LED - 1; i++) {

						temp[i] = temp[i + 1];
					}

					temp[7] = key;

					for (i = 0; i < NUM_7SEG_LED; i++) {

						writeb_virtual_io(SEG7LED, temp[i] + position[i]);

					}
				}

				else {

					for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

						temp[i] = temp[i - 1];
					}

					temp[0] = key;

					for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

						writeb_virtual_io(SEG7LED, temp[i] + position[i]);

					}

				}

				key_pressed = TRUE;

			}

		}

		if ((key_pressed == TRUE) && readb_virtual_io(KEY_MATRIX) == 0) {
			key_pressed = FALSE;
		}

		if ((sw1_key_pressed == TRUE)
				&& (!(readb_virtual_io(PUSH_SW) & VIRTUAL_SW_1))) {

			sw1_key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}


#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	// Variables
	int i;

	u8 key, scan = 0;
	u8 reverse = FALSE;
	u8 pressed = FALSE;
	u8 led[8];

	for (i = 0; i < 8; i++) {
		led[i] = 0x80;
	}

	while (1) {
		if (!pressed && readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
			if (reverse == FALSE)
				reverse = TRUE;
			else
				reverse = FALSE;
			for (i = 0; i < 8; i++) {
				led[i] = 0x80;
			}
			writeb_virtual_io(SEG7LED, 0x70 + led[0]);
			writeb_virtual_io(SEG7LED, 0x60 + led[1]);
			writeb_virtual_io(SEG7LED, 0x50 + led[2]);
			writeb_virtual_io(SEG7LED, 0x40 + led[3]);
			writeb_virtual_io(SEG7LED, 0x30 + led[4]);
			writeb_virtual_io(SEG7LED, 0x20 + led[5]);
			writeb_virtual_io(SEG7LED, 0x10 + led[6]);
			writeb_virtual_io(SEG7LED, 0x00 + led[7]);

			pressed = TRUE;
		}
		if (!pressed) {
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

				if (!reverse) {
					for (i = 7; i > 0; i--) {
						led[i] = led[i - 1];
					}
					led[0] = key;
				}

				else {
					for (i = 0; i < 8; i++) {
						led[i] = led[i + 1];
					}
					led[7] = key;
				}

				writeb_virtual_io(SEG7LED, 0x70 + led[0]);
				writeb_virtual_io(SEG7LED, 0x60 + led[1]);
				writeb_virtual_io(SEG7LED, 0x50 + led[2]);
				writeb_virtual_io(SEG7LED, 0x40 + led[3]);
				writeb_virtual_io(SEG7LED, 0x30 + led[4]);
				writeb_virtual_io(SEG7LED, 0x20 + led[5]);
				writeb_virtual_io(SEG7LED, 0x10 + led[6]);
				writeb_virtual_io(SEG7LED, 0x00 + led[7]);

				pressed = TRUE;
			}
		}

		if (pressed && readb_virtual_io(PUSH_SW) != VIRTUAL_SW_1
				&& readb_virtual_io(KEY_MATRIX) == 0) {
			pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );     // Wait while START KEY is being pressed
}

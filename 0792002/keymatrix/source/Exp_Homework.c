#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	u8 mode = FALSE, sw = TRUE, key, scan = 0,led[7],sw_pressed=FALSE,sw2;
	int i, j = 0;

	for (i = 0; i < NUM_7SEG_LED; i++)
		led[i] = 0x80;

	while (1) {
		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:
			if (sw) {
				j = 0;
				mode = !mode;
				for (i = 0; i < NUM_7SEG_LED; i++){				// Turn Off All
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
					led[i] = 0x80;
				}
				sw = FALSE;
			}
			break;
		default:
			sw = TRUE;
			break;
		}
		if(sw_pressed && (readb_virtual_io(KEY_MATRIX) != sw2 || !readb_virtual_io(KEY_MATRIX)))
			sw_pressed = FALSE;
		if (!sw_pressed) {
		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;
		sw2=readb_virtual_io(KEY_MATRIX);
		switch (sw2) {
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
			if (mode) {
				led[0] = key;
				for (i = 0; i < j; i++)
					writeb_virtual_io(SEG7LED, led[i] + ((0x10) * i));
				for (i = 7; i > 0; i--)
					led[i] = led[i - 1];
			} else if (!mode) {
				led[0] = key + 0x70;
				for (i = 0; i < j; i++)
					writeb_virtual_io(SEG7LED, led[i] - ((0x10) * i));
				for (i = 7; i > 0; i--)
					led[i] = led[i - 1];
			}
			sw_pressed=TRUE;
		}

		if (j != 8 && key != 255)
			j++;
		}
		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(30) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}


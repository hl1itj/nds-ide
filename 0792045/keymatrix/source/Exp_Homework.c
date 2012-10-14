#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	u8 state = TRUE, sw_s = TRUE, key, scan = 0;
	u8 led[7];
	int i, j = 0;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	while (1) {

		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:
			if (sw_s) {
				j = 0;
				state = ~state;
				for (i = 0; i < NUM_7SEG_LED; i++)				// Turn Off All
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
				sw_s = FALSE;
			}
			break;
		default:
			sw_s = TRUE;
			break;
		}
		if (state == TRUE) {
			//writeb_virtual_io(SEG7LED, 0x01);
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
				led[0] = key;
				for (i = 0; i < j; i++)
					writeb_virtual_io(SEG7LED, led[i] + ((0x10) * i));
				for (i = 7; i > 0; i--)
					led[i] = led[i - 1];
			}
			if (j != 8 && key != 255)
				j++;
		} else {
			//writeb_virtual_io(SEG7LED, 0x02);
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
				led[0] = key + 0x70;
				for (i = 0; i < j; i++)
					writeb_virtual_io(SEG7LED, led[i] - ((0x10) * i));
				for (i = 7; i > 0; i--)
					led[i] = led[i - 1];
			}
			if (j != 8 && key != 255)
				j++;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );	// Wait while START KEY is being pressed
}


#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	u8 state = FALSE, sw_s = FALSE, key_s = FALSE,
			scan = 0, led[NUM_7SEG_LED], key_m, key;
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)
		led[i] = 1 << 7;
	while (1) {
		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:
			if (!sw_s) {
				state = !state;
				for (i = 0; i < NUM_7SEG_LED; i++) {			// Turn Off All
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
					led[i] = 1 << 7;
				}
				sw_s = TRUE;
			}
			break;
		default:
			sw_s = FALSE;
			break;
		}
		if(key_s && (readb_virtual_io(KEY_MATRIX) != key_m || !readb_virtual_io(KEY_MATRIX)))
			key_s = FALSE;
		if (!key_s) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			key_m = readb_virtual_io(KEY_MATRIX);
			switch (key_m) {
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
				if (state == TRUE) {
					for (i = NUM_7SEG_LED - 1; i > 0; i--)
						led[i] = led[i - 1];
					led[0] = key;
				} else if (state == FALSE) {
					for (i = 0; i < NUM_7SEG_LED - 1; i++)
						led[i] = led[i + 1];
					led[NUM_7SEG_LED - 1] = key;
				}
				key_s = TRUE;
				for (i = 0; i < NUM_7SEG_LED; i++)
					writeb_virtual_io(SEG7LED, (i << 4) + led[i]);
			}
		}
		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );	// Wait while START KEY is being pressed
}

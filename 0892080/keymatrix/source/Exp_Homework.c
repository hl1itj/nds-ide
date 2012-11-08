#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	u8 key, scan = 0;
	u8 present_mode = 1;
	u8 present_key = FALSE;
	u8 past_key[8] = { 123, 123, 123, 123, 123, 123, 123, 123 };
	int i;
	int count = 0;

	for (i = 0; i < NUM_7SEG_LED; i++)
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	// Variables

	while (1) {
		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:
			if (present_mode == 1) {
				present_mode = 2;
				for (i = 0; i < NUM_7SEG_LED; i++)
					past_key[i] = 123;
				for (i = 0; i < NUM_7SEG_LED; i++)
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
			} else {
				present_mode = 1;
				for (i = 0; i < NUM_7SEG_LED; i++)
					past_key[i] = 123;
				for (i = 0; i < NUM_7SEG_LED; i++)
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
			}

			break;
		}
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
		if (present_key == FALSE) {
			if (key < 16) {
				if (present_mode == 1) {
					for (i = 6; i >= 0; i--)
						past_key[i + 1] = past_key[i];
					past_key[0] = key;

					for (i = 0; i < NUM_7SEG_LED; i++) {
							if (past_key[i] == 123)
								writeb_virtual_io(SEG7LED, 0x80 - (0x10 * i) + (i << 4));
							else
								writeb_virtual_io(SEG7LED,
																0x70 - (0x10 * i) + past_key[i]);

					}
				} else {
					for (i = 6; i >= 0; i--)
						past_key[i + 1] = past_key[i];
					past_key[0] = key;
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
					for (i = 0; i < NUM_7SEG_LED; i++) {
							if (past_key[i] == 123)
								writeb_virtual_io(SEG7LED, 0x80 + (0x10 * i) + (i >> 4));
							else
								writeb_virtual_io(SEG7LED,
																0x00 + (0x10 * i) + past_key[i]);

					}
				}
				count++;
			}

			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(MSEC2TICK(20) );
		}
		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(MSEC2TICK(10) );	// Wait while START KEY is being pressed
	}
}


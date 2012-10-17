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
	u8 mode = TRUE;
	u8 pressed = FALSE;
	u8 seg7leds[NUM_7SEG_LED] = { 0x80 };
	memset(seg7leds, 0x80, NUM_7SEG_LED);
	while (1) {
		if (!pressed && readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
			mode = mode ? FALSE : TRUE;
			for (i = 0; i < NUM_7SEG_LED; i++) {
				writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
				seg7leds[i] = 0x80;
			}
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
				if (mode) {
					for (i = 0; i < NUM_7SEG_LED - 1; i++) {
						seg7leds[i] = seg7leds[i + 1];
					}
					seg7leds[7] = key;
				} else {
					for (i = NUM_7SEG_LED - 1; i > 0; i--) {
						seg7leds[i] = seg7leds[i - 1];
					}
					seg7leds[0] = key;
				}
				for (i = 0; i < NUM_7SEG_LED; i++) {
					writeb_virtual_io(SEG7LED, (i << 4) + seg7leds[i]);
				}
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
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}


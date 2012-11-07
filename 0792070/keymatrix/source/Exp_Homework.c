#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

#define TOLEFT 0
#define TORIGHT 1

#define TRUE 1
#define FALSE 0

void init7SEG() {
	int i;
	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
}

void initseg7pos(u16 *seg7pos) { // seg Array init
	int i;
	for (i = 0; i < 8; i++) {
		seg7pos[i] = 1 << 7;
	}
}

u8 readKey(u8 key, u8 scan, u8 *prevkey) {
	key = scan * 4;

	*prevkey = readb_virtual_io(KEY_MATRIX);
	switch (*prevkey) {
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

	return key;
}

void leftShift(u16 *seg7pos) {
	int i;
	for (i = 0; i < 7; i++) {
		seg7pos[i] = seg7pos[i + 1];
	}
}

void rightShift(u16 *seg7pos) {
	int i;
	for (i = 7; i > 0; i--) {
		seg7pos[i] = seg7pos[i - 1];
	}
}

void Exp_4_Homework_A(void) {
	u8 key = 0, prevkey, scan = 0, state = TOLEFT, keypressed = FALSE;
	u16 seg7pos[8];
	int i;

	init7SEG();
	initseg7pos(seg7pos);

	while (1) {
		if (readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
			init7SEG();
			initseg7pos(seg7pos);
			if (state == TOLEFT) {
				state = TORIGHT;
				continue;
			} else if (state == TORIGHT) {
				state = TOLEFT;
				continue;
			}
		}

		if (keypressed == TRUE
				&& (readb_virtual_io(KEY_MATRIX) != prevkey
						|| !readb_virtual_io(KEY_MATRIX)))
			keypressed = FALSE;

		if (!keypressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = readKey(key, scan, &prevkey);
			scan++;
			if (scan == 4)
				scan = 0;
			if (key < 16) {
				if (state == TOLEFT) {
					leftShift(seg7pos);
					seg7pos[7] = key;
				} else if (state == TORIGHT) {
					rightShift(seg7pos);
					seg7pos[0] = key;
				}
				for (i = 0; i < NUM_7SEG_LED; i++)
					writeb_virtual_io(SEG7LED, (i << 4) + seg7pos[i]);
				keypressed = TRUE;
			}
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10

void
key_init(void)
{
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int
kbhit(void)
{
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8
getkey(void)
{
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	//while (pdPASS != xQueueReceive(KeyQueue, &key, 0))
	//		vTaskDelay(MSEC2TICK(5));
	return key;
}

void
Exp_5_Homework_A(void)
{
	u8 LED[8];
	u8 key;
	int i;

	key_init();

	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	}
	for (i = 0; i < 8; i++) {
		LED[i] = 0x80;
	}

	while (1) {

		key = getkey();

		if(key == 0) break;

		for (i = 7; i > 0; i--) {
			LED[i] = LED[i - 1];
		}

		LED[0] = key;

		for (i = 7; i >= 0; i--) {
			writeb_virtual_io(SEG7LED, (0x00 + (0x10 * i)) + LED[7 - i]);
		}
	}
}

void
Exp_5_Homework_B(void)
{
	u8 LED[8];
	u8 key, led = 0x80;
	u8 change = FALSE;
	int i;

	key_init();

	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	}
	for (i = 0; i < 8; i++) {
		LED[i] = 0x80;
	}

	while (key != 0) {

		if (kbhit() == 1) {
			key = getkey();

			for (i = 7; i > 0; i--) {
				LED[i] = LED[i - 1];
			}

			LED[0] = key;

			for (i = 7; i >= 0; i--) {
				writeb_virtual_io(SEG7LED, (0x00 + (0x10 * i)) + LED[7 - i]);
			}
		} else {
			writeb_virtual_io(BARLED1, led);
			if (change) {
				if (led < 0x80) {
					led *= 2;
				} else {
					change = FALSE;
					led /= 2;
				}
			} else {
				if (led > 0x01) {
					led /= 2;
				} else {
					change = TRUE;
					led *= 2;
				}
			}
			vTaskDelay(MSEC2TICK(500) );
		}
		if (NDS_SWITCH() & KEY_START)
			break;

	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	u8 key, select, scan = 0;
	u8 key_On = TRUE;

	while (1) {
		if (key_On) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			select = readb_virtual_io(KEY_MATRIX);
			switch (select) {
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

			if (scan == 4) {
				scan = 0;
			}

			if (key < 16) {
				xQueueSend(KeyQueue, &key, 0);
				key_On = FALSE;
			}
		} else {
			if ((key_On == FALSE) && (readb_virtual_io(KEY_MATRIX) != select)) {
				key_On = TRUE;
			}
		}
		vTaskDelay(MSEC2TICK(20) );
	}
}

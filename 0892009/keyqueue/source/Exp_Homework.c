#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <semphr.h>
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10

void key_init(void) {
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int kbhit(void) {
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8 getkey(void) {
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	//while (pdPASS != xQueueReceive(KeyQueue, &key, 0))
	//		vTaskDelay(MSEC2TICK(5));
	return key;
}

void Exp_5_Homework_A(void) {
	int i;
	u8 key;
	u8 seg7leds[NUM_7SEG_LED] = { 0x80 };
	memset(seg7leds, 0x80, NUM_7SEG_LED);
	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
		seg7leds[i] = 0x80;
	}
	while (1) {
		key = getkey();
		if (key > 0 && key < 16) {
			for (i = NUM_7SEG_LED - 1; i > 0; i--) {
				seg7leds[i] = seg7leds[i + 1];
			}
			seg7leds[0] = key;
			for (i = 0; i < NUM_7SEG_LED; i++) {
				writeb_virtual_io(SEG7LED, (i << 4) + seg7leds[i]);
			}

		} else if (key == 0x00) {
			break;
		}
	}
}
void Exp_5_Homework_B(void) {
	int i;
	u8 led_state = 0x80;
	u8 key;
	u8 seg7leds[NUM_7SEG_LED] = { 0x80 };
	memset(seg7leds, 0x80, NUM_7SEG_LED);
	portTickType xLastWakeTime = xTaskGetTickCount();
	while (1) {
		writeb_virtual_io(BARLED1, led_state);
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );
		if (kbhit()) {
			key = getkey();
			if (key < 16) {
				for (i = NUM_7SEG_LED - 1; i > 0; i--) {
					seg7leds[i] = seg7leds[i + 1];
				}
				seg7leds[0] = key;
				for (i = 0; i < NUM_7SEG_LED; i++) {
					writeb_virtual_io(SEG7LED, (i << 4) + seg7leds[i]);
				}
			}
		}

		if (NDS_SWITCH() & KEY_START) {
			break;
		}
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START) {
		vTaskDelay(10);
	}
}

portTASK_FUNCTION(Key_Task, pvParameters) {
// Variables
	u8 key, scan = 0;
	u8 pressed = FALSE;

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
			pressed = TRUE;
		}
	}

	if (pressed && readb_virtual_io(KEY_MATRIX) == 0) {
		xQueueSend(KeyQueue, &key, 0);
		pressed = FALSE;
	}
}

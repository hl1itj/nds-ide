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
	u8 seg7leds[NUM_7SEG_LED];
	key_init();
	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
		seg7leds[i] = 0x80;
	}
	while ((key = getkey()) > 0) {
		for (i = NUM_7SEG_LED - 1; i > 0; i--) {
			seg7leds[i] = seg7leds[i - 1];
		}
		seg7leds[0] = key;
		for (i = 0; i < NUM_7SEG_LED; i++) {
			writeb_virtual_io(SEG7LED, (i << 4) + seg7leds[i]);
		}
	}
}
void Exp_5_Homework_B(void) {
	int i;
	int check;
	u8 led_state = 0x80;
	u8 key;
	portTickType xLastWakeTime = xTaskGetTickCount();
	u8 mode = FALSE;
	u8 seg7leds[NUM_7SEG_LED];
	key_init();
	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
		seg7leds[i] = 0x80;
	}
	check = 0;
	while (1) {

		writeb_virtual_io(BARLED1, led_state);

		if (!mode) {
			led_state = led_state >> 1;
			if (led_state == 0x01) mode = TRUE;
		} else {
			led_state = led_state << 1;
			if (led_state == 0x80) mode = FALSE;
		}
		check = 0;

		while (check < 500) {
			vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(1) );
			check++;
			if (kbhit()) {
				key = getkey();
				for (i = NUM_7SEG_LED - 1; i > 0; i--) {
					seg7leds[i] = seg7leds[i - 1];
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
	}
	while (NDS_SWITCH() & KEY_START) {
		vTaskDelay(10);
	}
}

portTASK_FUNCTION(Key_Task, pvParameters) {
	u8 key, scan = 0;
	u8 pressed = FALSE;
	while (1) {
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
				if (key == 16) key = 0;
				break;
			default:
				key = 255;
				break;
			}
			scan++;
			if (scan == 4) scan = 0;
			if (key < 16) {
				xQueueSend(KeyQueue, &key, 0);
				pressed = TRUE;
			}
			vTaskDelay(MSEC2TICK(1) );

		}

		if (pressed && readb_virtual_io(KEY_MATRIX) == 0) {
			vTaskDelay(MSEC2TICK(1) );
			pressed = FALSE;
		}
		vTaskDelay(MSEC2TICK(1) );
	}
}

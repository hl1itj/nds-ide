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
#define EXIT 0
#define LED_OFF 0x00
#define LED_1ST 0x01
#define LED_8TH 0x80
#define OFFSET_MOVEMENT 2

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

void init_7SEG_LED() {
	int i;
	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	}
}

void Exp_5_Homework_A(void) {
	u8 key, key_values[NUM_7SEG_LED];
	int i, input_key_count = 0;

	init_7SEG_LED();
	key_init();

	while (1) {

		if ((key = getkey()) == EXIT)
			break;

		for (i = NUM_7SEG_LED - 1; i > 0; i--) {
			key_values[i] = key_values[i - 1];
		}

		key_values[0] = key;
		if (input_key_count < NUM_7SEG_LED) {
			input_key_count++;
		}
		for (i = 0; i < input_key_count; i++) {
			writeb_virtual_io(SEG7LED,
					0x70 - ((0x01 << 4) * i) + key_values[i]);
		}
	}
}

void Exp_5_Homework_B(void) {
	u8 led_flow = TRUE, barled = LED_8TH;
	u8 key, key_values[NUM_7SEG_LED];
	int i, input_key_count = 0, time = 0;

	init_7SEG_LED();
	key_init();

	while (1) {
		portTickType xLastWakeTime = xTaskGetTickCount();
		time += xLastWakeTime;
		if (time >= 500) {
			writeb_virtual_io(BARLED1, barled);
			if (led_flow) {
				if (barled > LED_1ST) {
					barled = barled / OFFSET_MOVEMENT;
				} else {
					led_flow = FALSE;
					barled = barled * OFFSET_MOVEMENT;
				}
			} else {
				if (barled < LED_8TH) {
					barled = barled * OFFSET_MOVEMENT;
				} else {
					led_flow = TRUE;
					barled = barled / OFFSET_MOVEMENT;
				}
			}

			time = 0;
		}
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );

		if (kbhit() == 1) {
			key = getkey();

			for (i = NUM_7SEG_LED - 1; i > 0; i--) {
				key_values[i] = key_values[i - 1];
			}
			key_values[0] = key;
			if (input_key_count < NUM_7SEG_LED) {
				input_key_count++;
			}

			for (i = 0; i < input_key_count; i++) {
				writeb_virtual_io(SEG7LED,
						0x70 - ((0x01 << 4) * i) + key_values[i]);
			}

		}
		if (NDS_SWITCH() & KEY_START)
						break;

	}
	key_init();
}

portTASK_FUNCTION(Key_Task, pvParameters) {
	u8 key, scan;
	int time;

	while (1) {
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
			if (xTaskGetTickCount() - time > MSEC2TICK(200) ) {
				xQueueSend(KeyQueue, &key, 0);
			}
			time = xTaskGetTickCount();
		}
		vTaskDelay(MSEC2TICK(20) );
	}
}


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"
portTickType short_timer;
extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10
#define MAX_SIZE 8
#define INIT 99
u8 temp[MAX_SIZE] = { 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };

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
static
void f_ts() {

	short_timer = xTaskGetTickCount();

}
void Exp_5_Homework_A(void) {
	u8 led_state[MAX_SIZE];
	u8 key;
	int i;
	for (i = 0; i < MAX_SIZE; i++)
		led_state[i] = INIT;
	for (i = 0; i < NUM_7SEG_LED; i++)                    // Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	while (1) {
		key = getkey();
		if (key == 0)
			break;
		for (i = MAX_SIZE - 1; i > 0; i--) {
			led_state[i] = led_state[i - 1];
		}
		led_state[0] = key;

		for (i = MAX_SIZE - 1; i >= 0; i--) {
			if (led_state[i] != INIT)
				writeb_virtual_io(SEG7LED, temp[i] + led_state[i]);
		}
	}
	vTaskDelay(MSEC2TICK(10) );

}

void Exp_5_Homework_B(void) {
	u8 led = 0x80;
	int chn;
	u8 led_state[MAX_SIZE];
	u8 key;
	int i;
	chn = 0;

	for (i = 0; i < MAX_SIZE; i++)
		led_state[i] = INIT;
	for (i = 0; i < NUM_7SEG_LED; i++)                    // Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	f_ts();
	while (1) {
		if (!kbhit()) {
			if ((xTaskGetTickCount() - short_timer) == MSEC2TICK(500) ) {
				writeb_virtual_io(BARLED1, led);
				if (led == 0x01)
					chn = 1;
				else if (led == 0x80)
					chn = 0;

				if (chn == 1)
					led = led << 1;
				else if (chn == 0)
					led = led >> 1;
				f_ts();
			}

		} else {
			key = getkey();
			for (i = MAX_SIZE - 1; i > 0; i--) {
				led_state[i] = led_state[i - 1];
			}
			led_state[0] = key;

			for (i = MAX_SIZE - 1; i >= 0; i--) {
				if (led_state[i] != INIT)
					writeb_virtual_io(SEG7LED, temp[i] + led_state[i]);
			}
		}
		if (NDS_SWITCH() & KEY_START)
			break;

		vTaskDelay(MSEC2TICK(10) );

	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );
}

portTASK_FUNCTION(Key_Task, pvParameters) {

	u8 key, scan = 0;
	int i;
	int key_pressed;
	key_pressed = 0;
	for (i = 0; i < NUM_7SEG_LED; i++)                    // Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	while (1) {
		if (key_pressed == 1 && readb_virtual_io(KEY_MATRIX) == 0) {
			key_pressed = 0;
		}
		if (key_pressed == 0) {
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
				key_pressed = 1;
				xQueueSend(KeyQueue, &key, 0);
			}

		}
		vTaskDelay(MSEC2TICK(50) );
	}
}

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

u8 temp[NUM_7SEG_LED];
u8 position[NUM_7SEG_LED] = { 0x70, 0x60, 0x50, 0x40, 0x30, 0x20, 0x10, 0x00 };
portTickType xLastWakeTime;

static void Init_Temp() {

	int i;

	for (i = 0; i < NUM_7SEG_LED; i++) {

		temp[i] = 0x80 + (i << 4);
	}

}

static void Init_LED() {

	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

}

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

	u8 key;
	int i;

	key_init();

	while (1) {

		key = getkey();

		if (key != 0) {

			for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

				temp[i] = temp[i - 1];
			}

			temp[0] = key;

			for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

				writeb_virtual_io(SEG7LED, temp[i] + position[i]);

			}

		} else {
			break;

		}

	}
}

void Exp_5_Homework_B(void) {

	u8 key;
	u8 led_position = 0x80;
	u8 turn = FALSE;

	int i;

	Init_Temp();
	Init_LED();
	key_init();

	//writeb_virtual_io(BARLED1, led_position);

	while (1) {

		if (kbhit() == 1) {

			key = getkey();

			for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

				temp[i] = temp[i - 1];
			}

			temp[0] = key;

			for (i = NUM_7SEG_LED - 1; i >= 0; i--) {

				writeb_virtual_io(SEG7LED, temp[i] + position[i]);

			}

		} else {

			if (led_position > 0x01 && turn == FALSE) {

				writeb_virtual_io(BARLED1, led_position);
				vTaskDelay(MSEC2TICK(500) );
				led_position = led_position / 2;

				if (led_position == 0x01) {

					turn = TRUE;
				}

			}

			if (led_position < 0x80 && turn == TRUE) {

				writeb_virtual_io(BARLED1, led_position);
				vTaskDelay(MSEC2TICK(500) );
				led_position = led_position * 2;

				if (led_position == 0x80) {

					turn = FALSE;
				}
			}

			if (NDS_SWITCH() & KEY_START) {

				Init_Temp();
				Init_LED();
				key_init();

				break;
			}


		}

	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );

}

portTASK_FUNCTION(Key_Task, pvParameters) {
	u8 key, scan = 0;
	u8 key_pressed = FALSE;

	Init_Temp();

	while (1) {

		if (key_pressed == FALSE) {

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

				xQueueSend(KeyQueue, &key, 0);
				key_pressed = TRUE;

			}

		}

		if ((key_pressed == TRUE) && readb_virtual_io(KEY_MATRIX) == 0) {
			key_pressed = FALSE;
		}

		vTaskDelay(MSEC2TICK(20) );
	}
}


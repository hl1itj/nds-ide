#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG        	10
#define KEY_0               0
#define MIN_LED             0x01
#define MAX_LED             0x80
#define LED_DELAY				500
#define SEG7LED_ON(x)     	writeb_virtual_io(SEG7LED, x)
#define LED1(x)             writeb_virtual_io(BARLED1, x)

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
	//        vTaskDelay(MSEC2TICK(5));
	return key;
}

void
Exp_5_Homework_A(void)
{
	int i;
	int cnt = 0;
	u8 key_arr[NUM_7SEG_LED];
	u8 key;

	for (i = 0; i < NUM_7SEG_LED; i++)    // Turn Off All
		SEG7LED_ON(0x80 + (i << 4));

	key_init();

	while(1) {

		key = getkey();
		if (key == KEY_0) break;

		if (cnt == NUM_7SEG_LED) {
			for(i = 0; i < cnt - 1; i++)
				key_arr[i] = key_arr[i + 1];
			key_arr[cnt - 1] = key;
		}
		else {
			cnt++;
			key_arr[cnt - 1] = key;
		}

		for(i = 0; i < cnt; i++)
			SEG7LED_ON((0x70 - (i << 4)) + key_arr[cnt - i - 1]);
	}
}

void
Exp_5_Homework_B(void)
{
	int i;
	int cnt = 0;
	int tick_count = LED_DELAY;
	int tick_time = LED_DELAY / 500;
	u8 key_arr[NUM_7SEG_LED];
	u8 key;
	u8 sw = FALSE;
	u8 barled = MAX_LED;

	portTickType xLastWakeTime = xTaskGetTickCount();

	for (i = 0; i < NUM_7SEG_LED; i++)    // Turn Off All
		SEG7LED_ON(0x80 + (i << 4));

	key_init();

	while (1) {
		if (tick_count == LED_DELAY) {
			tick_count = 0;
			LED1(barled);

			if (sw == FALSE)
				if (barled > MIN_LED)
					barled /= 2;
				else {
					barled *= 2;
					sw = TRUE;
				}
			else
				if (barled < MAX_LED)
					barled *= 2;
				else {
					barled /= 2;
					sw = FALSE;
				}
		}
		if (kbhit()) {
			key = getkey();

			if (cnt == NUM_7SEG_LED) {
				for(i = 0; i < cnt - 1; i++)
					key_arr[i] = key_arr[i + 1];
				key_arr[cnt - 1] = key;
			}
			else {
				cnt++;
				key_arr[cnt - 1] = key;
			}

			for(i = 0; i < cnt; i++)
				SEG7LED_ON((0x70 - (i << 4)) + key_arr[cnt - i - 1]);
		}

		if (NDS_SWITCH() & KEY_START)
			break;

		tick_count += tick_time;
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(tick_time));
	}
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	// Variables
	u8 key, scan = 0;
	u8 key_pressed = FALSE;
	u8 pre_line;

	while (1) {

		//for (i = 0; i < 100000; i++);
		// Fill Here

		if (!key_pressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;

			pre_line = readb_virtual_io(KEY_MATRIX);
			switch (readb_virtual_io(KEY_MATRIX)) {
			case 8 : key += 1; break;
			case 4 : key += 2; break;
			case 2 : key += 3; break;
			case 1 : key += 4; if (key == 16) key = 0; break;
			default : key = 255; break;
			}

			scan++;
			if (scan == 4)
				scan = 0;

			if (key < 16) {
				key_pressed = TRUE;
				xQueueSend(KeyQueue, &key, 0);
			}
		}

		if (key_pressed
				&& ((readb_virtual_io(KEY_MATRIX) == 0)
						|| (readb_virtual_io(KEY_MATRIX) != pre_line)))
			key_pressed = FALSE;
		vTaskDelay(MSEC2TICK(25));
	}
}

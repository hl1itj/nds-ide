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
	int i, j =0;
	u8 led[7];
	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	while (1) {
		if (getkey() == 0)
					break;
		led[0] = getkey() + 0x70;
		for (i = 0; i < j; i++)
			writeb_virtual_io(SEG7LED, led[i] - ((0x10) * i));
		for (i = 7; i > 0; i--)
			led[i] = led[i - 1];
		if (j != 8)
			j++;

		vTaskDelay(MSEC2TICK(30) );
	}
	key_init();
}

void Exp_5_Homework_B(void) {
	int i, j =0;
	u8 led[7];
	u8 barled = 0x80;
	u8 state; //TRUE 일 경우 오른쪽으로 이동 FALSE 일 경우 왼쪽으로 이동

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	while (1) {
		portTickType xLastWakeTime = xTaskGetTickCount();

		if(kbhit()){
			led[0] = getkey() + 0x70;
			for (i = 0; i < j; i++)
				writeb_virtual_io(SEG7LED, led[i] - ((0x10) * i));
			for (i = 7; i > 0; i--)
				led[i] = led[i - 1];
			if (j != 8)
				j++;
		}
		writeb_virtual_io(BARLED1, barled);
			if (barled == 0x80)
				state = TRUE;
			else if (barled == 0x01)
				state = FALSE;
			if (state == TRUE)
				barled = barled >> 1;
			else
				barled = barled << 1;
			vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
		if (NDS_SWITCH() & KEY_START)
				break;
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait for releasing START KEY
	key_init();
}

portTASK_FUNCTION(Key_Task, pvParameters) {
	u8 key, scan = 0;
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
		if (key < 16)
			xQueueSend(KeyQueue, &key, portMAX_DELAY);
		vTaskDelay(MSEC2TICK(10));
	}
}

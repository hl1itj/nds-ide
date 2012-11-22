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
	u8 key;
	short int position = 0x0070; //7segment에서의 시작지점 결정
	int cur_point = 8; // 8번째 맨 오른쪽이 초기값
	short int all_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	short int new_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {

		key = getkey();
		new_key[0] = key;

		for (i = 0; i < 8; i++) {
			position = 0x70 - (0x10 * i);
			writeb_virtual_io(SEG7LED, position + new_key[i]);
		}

		for (i = 0; i < 7; i++)
			all_key[i + 1] = new_key[i];
		for (i = 0; i < 8; i++)
			new_key[i] = all_key[i];

		if (key == 0x00)
			break;
		vTaskDelay(MSEC2TICK(30));
	}
}

void Exp_5_Homework_B(void) {
	u8 key;
	short int position = 0x0070; //7segment에서의 시작지점 결정
	int cur_point = 8; // 8번째 맨 오른쪽이 초기값
	short int all_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	short int new_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	u8 barled = 0;
	portTickType xLastWakeTime = xTaskGetTickCount();
	int i;
	int direct = 0; // 0 is LEFT , 1 is RIGHT
	short int led_state = 0x0001;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		writeb_virtual_io(BARLED1, led_state);

		if (led_state == 0x0080) {
			//led_state = 0x0001;
			direct = 1;
		} else if (led_state == 0x0001) {
			//led_state = 0x0080;
			direct = 0;
		}
		if (direct == 1)
			led_state = led_state / 2;
		else if (direct == 0)
			led_state = led_state * 2;

		if (kbhit()) {
			key = getkey();
			new_key[0] = key;

			for (i = 0; i < 8; i++) {
				position = 0x70 - (0x10 * i);
				writeb_virtual_io(SEG7LED, position + new_key[i]);
			}

			for (i = 0; i < 7; i++)
				all_key[i + 1] = new_key[i];
			for (i = 0; i < 8; i++)
				new_key[i] = all_key[i];
		}
		if (NDS_SWITCH() & KEY_START) //
			break;

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}

portTASK_FUNCTION( Key_Task, pvParameters) {
	/*
	 *KeyMatrix를 scan하여 앞서 선언된 KeyQueue에 xQueueSend()를 이용하여
	 *KeyMatrix를 눌린 Key (0x00 ~ 0x0F)를 넣 도록 작성된다. */
	u8 key, in, scan = 0;
	int key_pressed = 0;
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {

		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;
		switch (in = readb_virtual_io(KEY_MATRIX)) {
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
		if ((key < 16) {
			if(key_pressed == 0)) {
			key_pressed = (scan << 4) + in;
			xQueueSend(KeyQueue, &key, 0);
		    }
		}

		if ((key_pressed >> 4) == scan)
			if((key_pressed & 0x0f) != in)
				key_pressed = 0;

		vTaskDelay(MSEC2TICK(30));
	}
}

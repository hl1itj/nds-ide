#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

enum {
	TO_RIGHT, TO_LEFT
};

void Exp_4_Homework_A(void) {
	u8 key, scan = 0;
	int direction = TO_LEFT; // 처음에는 왼쪽으로
	short int position = 0x0070; //7segment에서의 시작지점 결정
	int cur_point = 8; // 8번째 맨 오른쪽이 초기값
	short int all_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	short int new_key[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };
	int count = 0;
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		if (readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
			//초기화
			//writeb_virtual_io(SEG7LED,0x80);//1000 0000으로 불끄라는 소리
			for (i = 0; i < NUM_7SEG_LED; i++) // Turn Off All
				writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

			for (i = 0; i < 8; i++){
				all_key[i] = 0x80;
				new_key[i] = 0x80;
			}

			//방향 변환
			if (direction == TO_LEFT) {
				direction = TO_RIGHT;

			} else if(direction == TO_RIGHT) {
				direction = TO_LEFT;
			}
		}

		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;
		switch (readb_virtual_io(KEY_MATRIX)) {
		case 8:key += 1;break;
		case 4:key += 2;break;
		case 2:key += 3;break;
		case 1:key += 4;if (key == 16)key = 0;break;
		default:key = 255;break;
		}
		scan++;
		if (scan == 4)
			scan = 0;
		if ((key < 16) && (direction == TO_LEFT)) {

			new_key[0] = key;
			if (count == 8)
				count = 0;
			else
				count++;
			for (i = 0; i < 8; i++) {
				position = 0x70 - (0x10 * i);
				writeb_virtual_io(SEG7LED, position + new_key[i]);
			}

			for (i = 0; i < 7; i++)
					all_key[i+1] = new_key[i];
			for (i = 0; i < 8; i++)
						new_key[i] = all_key[i];

		} else if ((key < 16) && (direction == TO_RIGHT)) {
			 new_key[0] = key;
			if (count == 8)
				count = 0;
			else
				count++;
			for (i = 0; i < 8; i++) {
				position = 0x00 + (0x10 * i);
				writeb_virtual_io(SEG7LED, position + new_key[i]);
			}

			for (i = 0; i < 7; i++)
					all_key[i+1] = new_key[i];
			for (i = 0; i < 8; i++)
						new_key[i] = all_key[i];
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(30) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait for releasing START KEY
}


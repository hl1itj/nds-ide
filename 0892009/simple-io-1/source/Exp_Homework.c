/*
 * Exp_Homework.c
 *
 *  Created on: 2011. 9. 22.
 *      Author: Minsuk Lee
 */

#include <stdio.h>			// C-Standard Header
#include <time.h>
#include <stdlib.h>

#include "FreeRTOS.h"		// Free RTOS Headers
#include "task.h"

#include <nds.h>			// NDS / Sevencore Board Headers
#include <sevencore_io.h>
#include "realio.h"

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void Exp_1_Homework_A(void) {
	u16 sw;
	// led_state 0000 0000 : 왼쪽부터 순서대로 1이면 on, 0이면 off
	u8 led_state = 0x01; // 0000 0001 이므로 제일 오른쪽 불이 on인 상태.
	u8 key_pressed = FALSE;

	while (1) {
		sw = NDS_SWITCH();
		if (key_pressed == FALSE) {
			if ((sw & KEY_LEFT) && (led_state < 0x80)) {
				led_state = led_state << 1;
			}
			if ((sw & KEY_RIGHT) && (led_state > 0x01)) {
				led_state = led_state >> 1;
			}
			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state);
		}

		if ((key_pressed == TRUE) && (!(sw & KEY_LEFT) && !(sw & KEY_RIGHT))) {
			key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START) { // START 키가 눌리면 반복문을 빠져나감
			break;
		}
		vTaskDelay(50); // 우선순위가 낮은 다른 프로세스가 있다면 실행권을 넘겨줌
	}
	while (NDS_SWITCH() & KEY_START) {
		vTaskDelay(10);
	}
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	// led_state 0000 0000 0000 0000 : 왼쪽 8개는 BARLED1, 나머지는 BARLED2, 왼쪽부터 순서대로 1이면 on, 0이면 off
	u16 led_state = 0x0001; // BARLED2 제일 오른쪽 불이 on인 상태
	u8 key_pressed = FALSE;

	while (1) {
		sw = NDS_SWITCH(); // 스위치 입력을 최초 한번만 받기 위해 sw란 변수에 NDS_SWITCH()를 저장

		if (key_pressed == FALSE) {
			if (sw & KEY_L) { // L 키를 눌렀을 때,
				if (led_state == 0x8000) { // 현재 상태가 BARLED1의 제일 왼쪽 불이 들어와 있다면,
					led_state = 0x0001; // BARLED2의 제일 오른쪽 불을 on
				} else {
					led_state = led_state << 1; // 그렇지 않으면 왼쪽으로 한 칸씩 옮김
				}
			} else if (sw & KEY_R) { // R 키를 눌렀을 때,
				if (led_state == 0x0001) { // 현재 상태가 BARLED2의 제일 오른 불이 들어와 있다면,
					led_state = 0x8000; // BARLED1의 제일 왼쪽 불을 on
				} else {
					led_state = led_state >> 1; // 그렇지 않으면 오쪽으로 한 칸씩 옮김
				}
			}
			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led_state >> 8); // led_state의 왼쪽 8개를 BARLED1에 출력
			writeb_virtual_io(BARLED2, led_state & 0xFF); // led_state의 오른쪽 8개를 BARLED2에 출력
		}

		if ((key_pressed == TRUE) && (!(sw & KEY_L) && !(sw & KEY_R))) {
			key_pressed = FALSE;
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

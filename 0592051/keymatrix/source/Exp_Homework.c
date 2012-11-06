#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void reset(u8 *LED) {
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++) {
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
	}
	for (i = 0; i < 8; i++) {
		LED[i] = 0x80;
	}
}

void Exp_4_Homework_A(void) {

	u8 key, scan = 0;
	u8 Mode_Change = FALSE;
	u8 LED[8];

	int Check_Time1, Check_Time2;
	int i;


	reset(LED);

	while (1) {
		if(xTaskGetTickCount()-Check_Time1 > MSEC2TICK(200))
			switch (readb_virtual_io(PUSH_SW)) {
			case VIRTUAL_SW_1:
				reset(LED);
				Mode_Change = !Mode_Change;
				Check_Time1 = xTaskGetTickCount();
				break;

			case VIRTUAL_SW_2:
				break;
			case VIRTUAL_SW_3:
				break;
			case VIRTUAL_SW_4:
				break;

			default:
				break;
			}

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

		if (scan == 4) {
			scan = 0;
		}

		if (key < 16) {

			if(xTaskGetTickCount()-Check_Time2 > MSEC2TICK(200)){
				for (i = 7; i > 0; i--) {
					LED[i] = LED[i - 1];
				}
			}

			LED[0] = key;
			Check_Time2 = xTaskGetTickCount();
		}

		if (Mode_Change) {
			for (i = 0; i < 8; i++) {
				writeb_virtual_io(SEG7LED, (0x00 + (0x10 * i)) + LED[i]);
			}
		} else {
			for (i = 7; i >= 0; i--) {
				writeb_virtual_io(SEG7LED, (0x00 + (0x10 * i)) + LED[7 - i]);
			}
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}

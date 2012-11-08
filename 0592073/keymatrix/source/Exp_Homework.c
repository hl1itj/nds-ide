#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void Exp_4_Homework_A(void) {
	u8 key;
	u8 led_flow = FALSE;
	u8 v1_key_pressed = FALSE;
	u8 scan = 0;
	int time;
	u8 key_values[NUM_7SEG_LED] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int i, input_key_count = 0;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		switch (readb_virtual_io(PUSH_SW)) {
		case VIRTUAL_SW_1:
			if (v1_key_pressed == FALSE) {
				led_flow = ~led_flow;
				v1_key_pressed = TRUE;
				input_key_count = 0;
				for (i = 0; i < NUM_7SEG_LED; i++) {
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
				}
			}
			break;
		default: /* no effect  */
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
		if (scan == 4)
			scan = 0;

		if (key < 16) {
			if (xTaskGetTickCount() - time > MSEC2TICK(200) ) {

				if (led_flow == FALSE) { // << move
					if (input_key_count == NUM_7SEG_LED) {
						for (i = NUM_7SEG_LED - 1; i > 0; i--) {
							key_values[i] = key_values[i - 1];
						}
						key_values[0] = key;
					} else {
						key_values[input_key_count] = key;
						input_key_count++;
					}

					for (i = 0; i < input_key_count; i++) {
						writeb_virtual_io(SEG7LED,
								0x70 - ((0x01 << 4) * i) + key_values[i]);
					}

				} else { // >> move
					if (input_key_count == NUM_7SEG_LED) {
						for (i = NUM_7SEG_LED - 1; i > 0; i--) {
							key_values[i] = key_values[i - 1];
						}
						key_values[0] = key;
					} else {
						key_values[input_key_count] = key;
						input_key_count++;
					}
					for (i = 0; i < input_key_count; i++) {
						writeb_virtual_io(SEG7LED,
								((0x01 << 4) * i) + key_values[i]);
					}
				}
			}
			time = xTaskGetTickCount();
		}

		if ((v1_key_pressed == TRUE)
				&& !(readb_virtual_io(PUSH_SW) & VIRTUAL_SW_1)) {
			v1_key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20) );
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10) );		// Wait while START KEY is being pressed
}


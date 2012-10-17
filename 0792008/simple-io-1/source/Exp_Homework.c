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

#define LED_MIN 0x01 // led min value
#define LED_MAX 0x80 // led max value
#define LED1 FALSE // led1 value
#define LED2 TRUE // led2 value

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void Exp_1_Homework_A(void) {
	u16 sw;
	u16 led = 0x01;
	u8 r_key_pressed = FALSE;
	u8 l_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (((r_key_pressed == FALSE) && (sw & KEY_RIGHT))) {
			r_key_pressed = TRUE;
			if (led == LED_MIN)
				writeb_virtual_io(BARLED1, LED_MIN);
			else {
				led /= 2;
				writeb_virtual_io(BARLED1, led);
			}

		}

		if ((r_key_pressed == TRUE) && (!(sw & KEY_RIGHT)))
			r_key_pressed = FALSE;

		if (((l_key_pressed == FALSE) && (sw & KEY_LEFT))) {
			l_key_pressed = TRUE;
			if (led == LED_MAX)
				writeb_virtual_io(BARLED1, LED_MAX);
			else {
				led *= 2;
				writeb_virtual_io(BARLED1, led);
			}
		}

		if ((l_key_pressed == TRUE) && (!(sw & KEY_LEFT)))
			l_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void) {
	u16 sw;
	u16 led = 0x01;
	u8 led_num = LED2; // led_num = FALSE : LED1 , TRUE : LED2
	u8 r_key_pressed = FALSE;
	u8 l_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, 0);
	writeb_virtual_io(BARLED2, led);

	while (1) {
		sw = NDS_SWITCH();

		if (((r_key_pressed == FALSE) && (sw & KEY_R))) {
			r_key_pressed = TRUE;
			if (led_num == LED2) {
				if (led > LED_MIN) {
					led /= 2;
					writeb_virtual_io(BARLED2, led);
				} else {
					led_num = LED1;
					led = LED_MAX;
					writeb_virtual_io(BARLED2, 0);
					writeb_virtual_io(BARLED1, led);
				}
			} else {
				if (led > LED_MIN) {
					led /= 2;
					writeb_virtual_io(BARLED1, led);
				} else  {
					led_num = LED2;
					led = LED_MAX;
					writeb_virtual_io(BARLED1, 0);
					writeb_virtual_io(BARLED2, led);
				}
			}
		}

		if ((r_key_pressed == TRUE) && (!(sw & KEY_R)))
			r_key_pressed = FALSE;

		if (((l_key_pressed == FALSE) && (sw & KEY_L))) {
			l_key_pressed = TRUE;
			if (led_num == LED2) {
				if (led < LED_MAX) {
					led *= 2;
					writeb_virtual_io(BARLED2, led);
				} else  {
					led_num = LED1;
					led = LED_MIN;
					writeb_virtual_io(BARLED2, 0);
					writeb_virtual_io(BARLED1, led);
				}
			} else {
				if (led < LED_MAX) {
					led *= 2;
					writeb_virtual_io(BARLED1, led);
				} else  {
					led_num = LED2;
					led = LED_MIN;
					writeb_virtual_io(BARLED1, 0);
					writeb_virtual_io(BARLED2, led);
				}
			}
		}

		if ((l_key_pressed == TRUE) && (!(sw & KEY_L)))
			l_key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

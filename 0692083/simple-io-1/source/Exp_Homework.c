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

#define LED_MIN 0x01
#define LED_MAX 0x80
#define LED_OFF 0x00
#define SHIFT 2

struct state_A {

	u16 sw;
	u8 led_position;
	u8 led_state;
	u8 left_key_pressed;
	u8 right_key_pressed;

};

struct state_B {

	u16 sw;
	u8 left_led_position;
	u8 right_led_position;
	u8 left_led_state;
	u8 right_led_state;
	u8 l_key_pressed;
	u8 r_key_pressed;
};

///////// HOMEWORK A Function //////////////////

void SetOnLeftKeyPressed(struct state_A *aState) {

	aState->left_key_pressed = TRUE;
	aState->led_state = !aState->led_state;
	aState->led_position = aState->led_position * SHIFT;
	writeb_virtual_io(BARLED1, aState->led_position);

}

void SetOnRightKeyPressed(struct state_A *aState) {

	aState->right_key_pressed = TRUE;
	aState->led_state = !aState->led_state;
	aState->led_position = aState->led_position / SHIFT;
	writeb_virtual_io(BARLED1, aState->led_position);

}

///////// HOMEWORK B Function /////////////////////

void SetOnLKeyPressedToLeftLed(struct state_B *bState) {

	if (bState->left_led_state) {

		bState->l_key_pressed = TRUE;
		bState->left_led_state = !bState->left_led_state;
		bState->left_led_position = bState->left_led_position * SHIFT;
		writeb_virtual_io(BARLED1, bState->left_led_position);
	}

	// else { exception } ..
}

void SetOnLKeyPressedToRightLed(struct state_B *bState) {

	if (bState->right_led_state) {

		bState->l_key_pressed = TRUE;
		bState->right_led_state = !bState->right_led_state;
		bState->right_led_position = bState->right_led_position * SHIFT;
		writeb_virtual_io(BARLED2, bState->right_led_position);
	}

	// else { exception } ..
}

void SetOnRKeyPressedToLeftLed(struct state_B *bState) {

	if (bState->left_led_state) {

		bState->r_key_pressed = TRUE;
		bState->left_led_state = !bState->left_led_state;
		bState->left_led_position = bState->left_led_position / SHIFT;
		writeb_virtual_io(BARLED1, bState->left_led_position);
	}

	// else { exception } ..
}

void SetOnRKeyPressedToRightLed(struct state_B *bState) {

	if (bState->right_led_state) {

		bState->r_key_pressed = TRUE;
		bState->right_led_state = !bState->right_led_state;
		bState->right_led_position = bState->right_led_position / SHIFT;
		writeb_virtual_io(BARLED2, bState->right_led_position);
	}

	// else { exception } ..
}
////////////////////////////////////////////////////////

void Exp_1_Homework_A(void) {

	struct state_A homeworkA;

	homeworkA.led_position = LED_MIN;
	homeworkA.led_state = FALSE;
	homeworkA.left_key_pressed = FALSE;
	homeworkA.right_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, homeworkA.led_position);
	writeb_virtual_io(BARLED2, 0);

	while (1) {

		homeworkA.sw = NDS_SWITCH();

		if ((homeworkA.left_key_pressed == FALSE)
				&& (homeworkA.sw & KEY_LEFT)) {

			if (homeworkA.led_position < LED_MAX) { // led_state < 2E7 and KEY_LEFT

				SetOnLeftKeyPressed(&homeworkA);

			}

		}

		if ((homeworkA.left_key_pressed == TRUE)
				&& (!(homeworkA.sw & KEY_LEFT))) {

			homeworkA.left_key_pressed = FALSE;

		}

		if ((homeworkA.right_key_pressed == FALSE)
				&& (homeworkA.sw & KEY_RIGHT)) {

			if (homeworkA.led_position > LED_MIN) { // led_state < 2E0 and KEY_RIGHT

				SetOnRightKeyPressed(&homeworkA);
			}

		}

		if ((homeworkA.right_key_pressed == TRUE)
				&& (!(homeworkA.sw & KEY_RIGHT))) {

			homeworkA.right_key_pressed = FALSE;
		}

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

	struct state_B homeworkB;

	homeworkB.left_led_position = LED_MIN;
	homeworkB.right_led_position = LED_MIN;

	homeworkB.l_key_pressed = FALSE;
	homeworkB.r_key_pressed = FALSE;

	homeworkB.left_led_state = TRUE;
	homeworkB.right_led_state = FALSE;

	writeb_virtual_io(BARLED1, homeworkB.left_led_state);
	writeb_virtual_io(BARLED2, homeworkB.right_led_state);

	while (1) {

		homeworkB.sw = NDS_SWITCH();

		//////////////////////////////////////////////////////////////////////////////////

		if ((homeworkB.l_key_pressed == FALSE) && (homeworkB.sw & KEY_L)
				&& (homeworkB.left_led_state)) {

			if (homeworkB.left_led_position < LED_MAX) {

				SetOnLKeyPressedToLeftLed(&homeworkB);
				homeworkB.left_led_state = TRUE;

			} else {

				homeworkB.l_key_pressed = TRUE;
				homeworkB.left_led_state = FALSE;
				homeworkB.right_led_state = TRUE;

				homeworkB.left_led_position = LED_MIN;
				homeworkB.right_led_position = LED_MIN;

				writeb_virtual_io(BARLED1, LED_OFF);
				writeb_virtual_io(BARLED2, homeworkB.right_led_position);

			}
		}

		if ((homeworkB.l_key_pressed == FALSE) && (homeworkB.sw & KEY_L)
				&& (homeworkB.right_led_state)) { //left key & right led

			if (homeworkB.right_led_position < LED_MAX) {

				SetOnLKeyPressedToRightLed(&homeworkB);
				homeworkB.right_led_state = TRUE;

			} else {

				homeworkB.l_key_pressed = TRUE;
				homeworkB.right_led_state = FALSE;
				homeworkB.left_led_state = TRUE;

				homeworkB.right_led_position = LED_MIN;
				homeworkB.left_led_position = LED_MIN;

				writeb_virtual_io(BARLED2, LED_OFF);
				writeb_virtual_io(BARLED1, homeworkB.left_led_position);

			}
		}

		if ((homeworkB.l_key_pressed == TRUE) && (!(homeworkB.sw & KEY_L))) {

			homeworkB.l_key_pressed = FALSE;

		}

		///////////////////////////////////////////////////////////////////////////////

		if ((homeworkB.r_key_pressed == FALSE) && (homeworkB.sw & KEY_R)
				&& (homeworkB.left_led_state)) { //right key & left led

			if (homeworkB.left_led_position > LED_MIN) {

				SetOnRKeyPressedToLeftLed(&homeworkB);
				homeworkB.left_led_state = TRUE;

			} else {

				homeworkB.r_key_pressed = TRUE;
				homeworkB.left_led_state = FALSE;
				homeworkB.right_led_state = TRUE;

				writeb_virtual_io(BARLED1, LED_OFF);
				homeworkB.right_led_position = LED_MAX;
				homeworkB.left_led_position = LED_MAX;
				writeb_virtual_io(BARLED2, homeworkB.left_led_position);

			}
		}

		if ((homeworkB.r_key_pressed == FALSE) && (homeworkB.sw & KEY_R)
				&& (homeworkB.right_led_state)) {

			if (homeworkB.right_led_position > LED_MIN) {

				SetOnRKeyPressedToRightLed(&homeworkB);
				homeworkB.right_led_state = TRUE;

			} else {

				homeworkB.r_key_pressed = TRUE;
				homeworkB.right_led_state = FALSE;
				homeworkB.left_led_state = TRUE;

				writeb_virtual_io(BARLED2, LED_OFF);
				homeworkB.left_led_position = LED_MAX;
				homeworkB.right_led_position = LED_MAX;
				writeb_virtual_io(BARLED1, homeworkB.left_led_position);

			}
		}

		////////////////////////////////////////////////////////////////////

		if ((homeworkB.r_key_pressed == TRUE) && (!(homeworkB.sw & KEY_R))) {

			homeworkB.r_key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;

		vTaskDelay(50);

	}

	while (NDS_SWITCH() & KEY_START)

		vTaskDelay(10);

}

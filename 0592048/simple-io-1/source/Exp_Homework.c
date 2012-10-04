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
void
Exp_1_Homework_A(void)
{
	u16 sw;

	u8 button;
	button = 0x01;
	u8  key_pressed = FALSE;

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, button);
		writeb_virtual_io(BARLED2, 0x00);

		if ((key_pressed == FALSE) && (sw & KEY_LEFT)) {
			if(button < 0x80){
				button = button << 1;
				key_pressed = TRUE;
<<<<<<< HEAD
				writeb_virtual_io(BARLED1, button);
=======
>>>>>>> 6a4d6c0f1089a112a9207475bac5a69dd0a6a7e2
			}
			writeb_virtual_io(BARLED1, button);
		}else if ((key_pressed == FALSE) && (sw & KEY_RIGHT)) {
			if(button > 0x01){
				button = button >> 1;
				key_pressed = TRUE;
<<<<<<< HEAD
				writeb_virtual_io(BARLED1, button);
=======
>>>>>>> 6a4d6c0f1089a112a9207475bac5a69dd0a6a7e2
			}
		}

		if (key_pressed == TRUE){
			if(!(sw & KEY_RIGHT) && !(sw & KEY_LEFT))
				key_pressed = FALSE;
		}

		if(key_pressed == TRUE){
			if(!(sw & KEY_LEFT) && !(sw & KEY_RIGHT))
				key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void
Exp_1_Homework_B(void)
{
	u16 sw;

	u8 button;
	button = 0x00;

	u8 button1;
	button1 = 0x01;
	u8  key_pressed = FALSE;

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED1, button);
		writeb_virtual_io(BARLED2, button1);

		if ((key_pressed == FALSE) && (sw & KEY_L)) {
			if((button < 0x80) && (button1 == 0x00)){
				button = button << 1;
			}else if(button == 0x80){
				button = 0x00;
				button1 = 0x01;
			}else if((button1 < 0x80) && (button == 0x00)){
				button1 = button1 << 1;
			}else if(button1 == 0x80){
				button = 0x01;
				button1 = 0x00;
			}

			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, button);
			writeb_virtual_io(BARLED2, button1);
		}else if ((key_pressed == FALSE) && (sw & KEY_R)) {
			if((button > 0x01) &&( button1 == 0x00)){
				button = button >> 1;
			}else if(button == 0x01){
				button = 0x00;
				button1 = 0x80;
			}else if((button1 > 0x01) && (button == 0x00)){
				button1 = button1 >> 1;
			}else if(button1 == 0x01){
				button = 0x80;
				button1 = 0x00;
			}

			key_pressed = TRUE;
			writeb_virtual_io(BARLED1, button);
			writeb_virtual_io(BARLED2, button1);
		}

		if (key_pressed == TRUE){
			if(!(sw & KEY_R) && !(sw & KEY_L))
				key_pressed = FALSE;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

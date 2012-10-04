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

#define OFFSET_MOVEMENT 2
#define LED_OFF 0x00
#define LED_1ST 0x01
#define LED_8TH 0x80
// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void
Exp_1_Homework_A(void)
{
	u16 sw;
	u16 offset = LED_1ST;
	u8 L_key_pressed = FALSE;
	u8 R_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, LED_1ST);
	writeb_virtual_io(BARLED2, LED_OFF);
		while (1) {
			sw = NDS_SWITCH();

			if (((L_key_pressed == FALSE) && (sw & KEY_L))){
				L_key_pressed = TRUE;
				if(offset < LED_8TH){
					offset = offset * OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
			} else if (((R_key_pressed == FALSE) && (sw & KEY_R))){
				R_key_pressed = TRUE;
				if(offset > LED_1ST){
					offset = offset / OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
			}

			if ((L_key_pressed == TRUE) && (!(sw & KEY_L))){
				L_key_pressed = FALSE;
			} else if ((R_key_pressed == TRUE) && (!(sw & KEY_R))){
				R_key_pressed = FALSE;
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
	u16 LED1_offset = LED_1ST;
	u16 LED2_offset = LED_OFF;
	u8 L_key_pressed = FALSE;
	u8 R_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, LED_1ST);
	writeb_virtual_io(BARLED2, LED_OFF);
		while (1) {
			sw = NDS_SWITCH();

			if (((L_key_pressed == FALSE) && (sw & KEY_L))){
				L_key_pressed = TRUE;
				if(LED2_offset == LED_OFF) { // Located BARLED1
					if(LED1_offset < LED_8TH){ // normal moving
						LED1_offset = LED1_offset * OFFSET_MOVEMENT;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					} else { // jumping BARLED2 1st block
						LED1_offset = LED_OFF;
						LED2_offset = LED_1ST;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					}
				} else { // Located BARLED2
					if(LED2_offset < LED_8TH){ // normal moving
						LED2_offset = LED2_offset * OFFSET_MOVEMENT;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					} else { // jumping BARLED1 1st block
						LED1_offset = LED_1ST;
						LED2_offset = LED_OFF;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					}
				}
			} else if (((R_key_pressed == FALSE) && (sw & KEY_R))){
				R_key_pressed = TRUE;
				if(LED1_offset == LED_OFF){ // Located BARLED2
					if(LED2_offset > LED_1ST){ // normal moving
						LED2_offset = LED2_offset / OFFSET_MOVEMENT;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					} else { // jumping BARLED1 8th block
						LED1_offset = LED_8TH;
						LED2_offset = LED_OFF;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					}
				} else { //Located BARLED1
					if (LED1_offset > LED_1ST) { // normal moving
						LED1_offset = LED1_offset / OFFSET_MOVEMENT;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);
					} else { // jumping BARLED2 8th block
						LED1_offset = LED_OFF;
						LED2_offset = LED_8TH;
						writeb_virtual_io(BARLED1, LED1_offset);
						writeb_virtual_io(BARLED2, LED2_offset);

					}
				}

			}

			if ((L_key_pressed == TRUE) && (!(sw & KEY_L))){
				L_key_pressed = FALSE;
			} else if ((R_key_pressed == TRUE) && (!(sw & KEY_R))){
				R_key_pressed = FALSE;
			}

			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
		}
		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(10);		// Wait while START KEY is being pressed
}

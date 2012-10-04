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

#define MIN_LED 0x01
#define MAX_LED 0x80
#define LED1(x) writeb_virtual_io(BARLED1, x)
#define LED2(x) writeb_virtual_io(BARLED2, x)

// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void Exp_1_Homework_A(void)
{
	u16 sw;
	u16 led = MIN_LED;
	u8 keyRight_pressed = FALSE;
	u8 keyLeft_pressed = FALSE;

	LED1(MIN_LED);
	LED2(0);

	while (1) {
		sw = NDS_SWITCH();

		if((sw & KEY_RIGHT) && (keyRight_pressed == FALSE)) {
			keyRight_pressed = TRUE;
			if(led > MIN_LED) led/=2;
			LED1(led);
		}
		else if((sw & KEY_LEFT) && (keyLeft_pressed == FALSE)) {
			keyLeft_pressed = TRUE;
			if(led < MAX_LED) led*=2;
			LED1(led);
		}

		if((keyRight_pressed == TRUE) && !(sw & KEY_RIGHT))
			keyRight_pressed = FALSE;

		if((keyLeft_pressed == TRUE) && !(sw & KEY_LEFT))
			keyLeft_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right
void Exp_1_Homework_B(void)
{
	u16 sw;
	u16 led = MIN_LED;
	u8 bar_sw = FALSE;	// TRUE = LED1 : FALSE = LED2
	u8 keyR_pressed = FALSE;
	u8 keyL_pressed = FALSE;

	LED1(0);
	LED2(led);

	while (1) {
		sw = NDS_SWITCH();

		if((sw & KEY_R) && (keyR_pressed == FALSE)) {
			keyR_pressed = TRUE;
			if(led > MIN_LED)
				led/=2;
			else {
				bar_sw = (bar_sw==TRUE)? FALSE : TRUE;
				led = MAX_LED;
			}

			if(bar_sw == TRUE){
				LED2(0); LED1(led);}
			else {
				LED1(0); LED2(led);}
		}
		else if((sw & KEY_L) && (keyL_pressed == FALSE)) {
			keyL_pressed = TRUE;
			if(led < MAX_LED)
				led*=2;
			else {
				bar_sw = (bar_sw==TRUE)? FALSE : TRUE;
				led = MIN_LED;
			}

			if(bar_sw == TRUE){
				LED2(0); LED1(led);}
			else {
				LED1(0); LED2(led);}
		}

		if((keyR_pressed == TRUE) && !(sw & KEY_R))
			keyR_pressed = FALSE;

		if((keyL_pressed == TRUE) && !(sw & KEY_L))
			keyL_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

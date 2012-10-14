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
	u16 led = 0x01;
	u8 L_key_pressed = FALSE;
	u8 R_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if(sw & KEY_LEFT){
			L_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led);
		}
		if((L_key_pressed == TRUE) && (sw == 0)){
				if(led < 0x80){
					led*=2;
					writeb_virtual_io(BARLED1, led);
				}
				L_key_pressed = FALSE;
		}

		if(sw & KEY_RIGHT){
			R_key_pressed = TRUE;
			writeb_virtual_io(BARLED1, led);
		}
		if((R_key_pressed == TRUE) && (sw == 0)){
				if(led > 0x01){
					led/=2;
					writeb_virtual_io(BARLED1, led);
				}
				R_key_pressed = FALSE;
		}

		if(NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
	}

	while(NDS_SWITCH() & KEY_START)
		vTaskDelay(10);

}

// LED Bar Left-and-Right & Round (BARLED 1 and BARLED 2)
// L key - going left, R key - going right

void
Exp_1_Homework_B(void)
{
		u16 sw;
		u16 led1 = 0x01;
		u16 led2 = 0x01;
		u8 L_key_pressed = FALSE;
		u8 R_key_pressed = FALSE;
		u8 Led_ch = FALSE;

		writeb_virtual_io(BARLED1, led1);
		writeb_virtual_io(BARLED2, 0);


		while (1) {
			sw = NDS_SWITCH();

			if(Led_ch == FALSE){
				writeb_virtual_io(BARLED1, led1);
				writeb_virtual_io(BARLED2, 0);
			}else{
				writeb_virtual_io(BARLED2, led2);
				writeb_virtual_io(BARLED1, 0);
			}

			if(sw & KEY_L){
				L_key_pressed = TRUE;
			}

			if((L_key_pressed == TRUE) && (sw == 0)){

				if(Led_ch == FALSE){
					if(led1 < 0x80){
						led1*=2;
					}else{
						led2 = 0x01;
						Led_ch = TRUE;
					}
				}else{
					if(led2 < 0x80){
						led2*=2;
					}else{
						led1 = 0x01;
						Led_ch = FALSE;
					}
				}
				L_key_pressed = FALSE;
			}

			if(sw & KEY_R){
				R_key_pressed = TRUE;
			}

			if((R_key_pressed == TRUE) && (sw == 0)){

				if(Led_ch == FALSE){
					if(led1 > 0x01){
						led1/=2;
					}else{
						led2 = 0x80;
						Led_ch = TRUE;
					}
				}else{
					if(led2 > 0x01){
						led2/=2;
					}else{
						led1 = 0x80;
						Led_ch = FALSE;
					}
				}
				R_key_pressed = FALSE;
			}

			if(NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
			}

		while(NDS_SWITCH() & KEY_START)
			vTaskDelay(10);
}

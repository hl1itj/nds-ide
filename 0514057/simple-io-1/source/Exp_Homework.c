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

		u16 sw,bsw=KEY_UP;
		int led=8;
		while (1) {
			sw = NDS_SWITCH();
			writeb_virtual_io(BARLED1, led);
			if(sw!=bsw){
				if (sw & KEY_RIGHT && led!=1)
					led=led/2;
				if (sw & KEY_LEFT && led!=128)
					led=led*2;
			}
			bsw=sw;
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

		u16 sw,bsw=KEY_UP;
		int led=8,LR=1;
		while (1) {
			sw = NDS_SWITCH();

			if(LR==1)
			{
				writeb_virtual_io(BARLED2,0);
				writeb_virtual_io(BARLED1,led);
			}
			else
			{
				writeb_virtual_io(BARLED1,0);
				writeb_virtual_io(BARLED2,led);
			}


			if(sw!=bsw){
				if (sw & KEY_R)
				{
					if(led!=1)
						led=led/2;
					else
						led=128, LR=~LR;
				}
				if (sw & KEY_L)
				{
					if(led!=128)
						led=led*2;
					else
						led=1, LR=~LR;
				}
			}

			bsw=sw;
			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
		}
		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(10);		// Wait while START KEY is being pressed

}

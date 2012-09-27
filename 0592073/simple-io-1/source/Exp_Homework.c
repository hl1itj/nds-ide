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
#define BARLED1_MAX 128
#define BARLED1_MIN 1
#define OFFSET_MOVEMENT 2
// LED Bar Left-and-Right (BARLED 1)
// LEFT key - going left, RIGHT key - going right
void
Exp_1_Homework_A(void)
{
	u16 sw;
	u16 offset = 0x01;
	writeb_virtual_io(BARLED1, 1);
	writeb_virtual_io(BARLED2, 0);
		while (1) {
			sw = NDS_SWITCH();

			if(sw & KEY_L){
				if(offset < BARLED1_MAX){
					offset = offset * OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
			} else if (sw & KEY_R){
				if(offset > BARLED1_MIN){
					offset = offset / OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
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
	u16 offset = 0x100;
	writeb_virtual_io(BARLED1, 1);
	writeb_virtual_io(BARLED2, 0);
		while (1) {
			sw = NDS_SWITCH();
			/*
			if(sw & KEY_L){

			} else if (sw & KEY_R){

			}

*/
			if(sw & KEY_L){
				if ( (offset/256) > 0 ) { //BARLED1 LED ON
					offset = offset * 2;
					writeb_virtual_io(BARLED1, offset/256);
					writeb_virtual_io(BARLED2, 0);
					if( offset == 32768 ){
						offset = 0x01;
						writeb_virtual_io(BARLED1, 0);
						writeb_virtual_io(BARLED2, 1);
					}
				} else if (( offset/256 ) == 0){ //BARLED2 LED ON
					offset = offset * 2;
					writeb_virtual_io(BARLED1, 0);
					writeb_virtual_io(BARLED2, offset);
				}
			} else if (sw & KEY_R){
				if ( (offset/256) > 0 ) { //BARLED1 LED ON
					offset = offset / 2;
					writeb_virtual_io(BARLED1, offset/256);
					writeb_virtual_io(BARLED2, 0);
				} else if ( (offset/256 == 0) ) { //BARLED2 LED ON
					offset = offset / 2;
					writeb_virtual_io(BARLED1, 0);
					writeb_virtual_io(BARLED2, offset);
					if( offset == 1 ){
						offset = 32768;
						writeb_virtual_io(BARLED1, offset/256);
						writeb_virtual_io(BARLED2, 0);
					}
				}
			}

			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
		}
		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(10);		// Wait while START KEY is being pressed
}

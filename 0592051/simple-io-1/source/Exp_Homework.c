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

	writeb_virtual_io(BARLED1, led);
	writeb_virtual_io(BARLED2, 0);
	while (1) {
		sw = NDS_SWITCH();

		if(sw & KEY_L){
			if(led < 0x80){
				led*=2;
				writeb_virtual_io(BARLED1, led);
			}
		}

		if(sw & KEY_R){
			if(led > 0x01){
				led/=2;
				writeb_virtual_io(BARLED1, led);
			}
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
		u16 led2 = 0x80;

		writeb_virtual_io(BARLED1, led1);
		writeb_virtual_io(BARLED2, 0);
		while (1) {
			sw = NDS_SWITCH();

			if(sw & KEY_L){
				if(led1 < 0x100){

					writeb_virtual_io(BARLED1, led1);
					writeb_virtual_io(BARLED2, 0);
					led1*=2;
					led2=0x01;
				}else{
					writeb_virtual_io(BARLED1, 0);
					if(led2 < 0x100){
						writeb_virtual_io(BARLED2, led2);
						led2*=2;
					}else{
						led1 = 0x01;
					}
				}
			}

			if(sw & KEY_R){
				if(led1 > 0x01){
					led1/=2;
					writeb_virtual_io(BARLED1, led1);
					writeb_virtual_io(BARLED2, 0);
					led2=0x80;
				}else{
					writeb_virtual_io(BARLED1, 0);
					if(led2 > 0X00){
						writeb_virtual_io(BARLED2, led2);
						if(led2 != 0x00)
						led2/=2;
					}else{
						led1 = 0x80;
					}
				}
			}

			if(NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
		}

		while(NDS_SWITCH() & KEY_START)
			vTaskDelay(10);

}

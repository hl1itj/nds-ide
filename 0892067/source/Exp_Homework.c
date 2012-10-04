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

void
Exp_1_Homework_A(void)
{
	u16 sw;
	u8  key_pressed = FALSE;
   short int led_state = 0x0001;

    //writeb_virtual_io(BARLED1, led_state);
    writeb_virtual_io(BARLED1, led_state);

	while (1) {
		sw = NDS_SWITCH();
		writeb_virtual_io(BARLED2,0);

		if (((key_pressed == FALSE) && (sw & KEY_LEFT))) {
			key_pressed = TRUE;
			if(!(led_state & 0x0080))
				led_state = led_state*2;
			writeb_virtual_io(BARLED1, led_state);

		}

		if ((key_pressed == TRUE) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)) )
			key_pressed = FALSE;

		if (((key_pressed == FALSE) && (sw & KEY_RIGHT))) {
			key_pressed = TRUE;
			if(led_state != 0x0001) //max right
				led_state = led_state/2;
			writeb_virtual_io(BARLED1, led_state);
		}

		if ((key_pressed == TRUE) && (!(sw & KEY_RIGHT)) && (!(sw & KEY_LEFT)))
			key_pressed = FALSE;

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
        u8  key_pressed = FALSE;
        short int led_state = 0x0001;

    //writeb_virtual_io(BARLED1, led_state);
    writeb_virtual_io(BARLED1, led_state);

        while (1) {
                sw = NDS_SWITCH();
                writeb_virtual_io(BARLED2,0);

                if (((key_pressed == FALSE) && (sw & KEY_LEFT))) {
                        key_pressed = TRUE;
                        if(!(led_state & 0x0080))// max right
                                led_state = led_state*2;
                        writeb_virtual_io(BARLED1, led_state);

                }

                if ((key_pressed == TRUE) && (!(sw & KEY_LEFT)) && (!(sw & KEY_RIGHT)) )
                        key_pressed = FALSE;

                if (((key_pressed == FALSE) && (sw & KEY_RIGHT))) {
                        key_pressed = TRUE;
                        if(led_state != 0x0001) //max left
                                led_state = led_state/2;
                        writeb_virtual_io(BARLED1, led_state);
                }

                if ((key_pressed == TRUE) && (!(sw & KEY_RIGHT)) && (!(sw & KEY_LEFT)))
                        key_pressed = FALSE;
   
                if(!(led_state & 0x0080)){ //If it is max right and state of LED is pressed 'KEY_L' 
                        if (((key_pressed == FALSE) && (sw & KEY_L))) {
                                led_state = 0x0001;
                                writeb_virtual_io(BARLED2, led_state);
                        }  
                 }

                 if(!(led_state & 0x0001)){//If it is max left and state of LED is pressed 'KEY_R'
                        if (((key_pressed == FALSE) && (sw & KEY_R))) {
                                led_state = 0x0080;
                                writeb_virtual_io(BARLED1, led_state);
                        }
                 }

                if (NDS_SWITCH() & KEY_START)
                        break;
                vTaskDelay(50);
        }
        while (NDS_SWITCH() & KEY_START)
                vTaskDelay(10);         // Wait while START KEY is being pressed
}

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void
Exp_4_Homework_A(void)
{
    u8 key[NUM_7SEG_LED], key_dump;
    u8 scan = 0;
    u8 turn_lr = TRUE;
    u8 sw_pressed = FALSE;
    u8 key_pressed = FALSE;
    u8 key_temp;
    int i;
    // Variables

   for (i = 0; i < NUM_7SEG_LED ; i++)
		key[i] = 255;

    while (1) {
		if ((readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) && !sw_pressed){
			  turn_lr = !turn_lr;
			  for (i = 0; i < NUM_7SEG_LED ; i++)
				  key[i] = 255;
			  sw_pressed = TRUE;
		}

		if (!key_pressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key_dump = scan * 4;
			key_temp = readb_virtual_io(KEY_MATRIX);
			switch (key_temp) {
				case 8 :  key_dump += 1; break;
				case 4 :  key_dump += 2; break;
				case 2 :  key_dump += 3; break;
				case 1 :  key_dump += 4; if (key_dump == 16) key_dump = 0; break;
				default : key_dump = 255; break;
			}
			scan++;
			if (scan == 4)
				scan = 0;


			if (key_dump < 16) {
				for (i = NUM_7SEG_LED; i > 0 ; i--)
					key[i] = key[i-1];
				key[0] = key_dump;
				key_pressed = TRUE;
			}
		}
		if (turn_lr) {
			for (i = 0; i < NUM_7SEG_LED; i++){
				if (key[i] < 16)
					writeb_virtual_io(SEG7LED, (0x70 - (i<<4)) + key[i]);
				else
					writeb_virtual_io(SEG7LED, 0x80 + ((NUM_7SEG_LED-1-i) << 4));
			}
		}
		else {
			for (i = 0; i < NUM_7SEG_LED; i++) {
				if (key[i] < 16)
					writeb_virtual_io(SEG7LED, (0x00 + (i<<4)) + key[i]);
				else
					writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
			}
		}

		if (sw_pressed && readb_virtual_io(PUSH_SW) != VIRTUAL_SW_1)
			sw_pressed = FALSE;
		if (key_pressed && (readb_virtual_io(KEY_MATRIX) != key_temp || !(readb_virtual_io(KEY_MATRIX))))
			key_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}


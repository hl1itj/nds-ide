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
    // Variables
    u8 key, scan = 0;
    int i;
    int check = 0x70;
    int cnt=0;
    u8 key_arr[8]={0,0,0,0,0,0,0,0};



	while (1) {
		if(readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1) {
			for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
				writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
		}

		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;
		switch (readb_virtual_io(KEY_MATRIX)) {
		  case 8 :  key += 1; break;
		  case 4 :  key += 2; break;
		  case 2 :  key += 3; break;
		  case 1 :  key += 4; if (key == 16) key = 0; break;
		  default : key = 255; break;
		}
		scan++;
		if (scan == 4)
			scan = 0;
		if (key < 16){
			key_arr[cnt]=key;
			cnt++;
			for(i = 0; i <8; i++){
				writeb_virtual_io(SEG7LED, check + key_arr[i]);
				check += 0x80;
			}
		}


		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(30));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}


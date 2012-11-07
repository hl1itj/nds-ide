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
    u8  key;
    u8  scan = 0;
    u8  bKey[4] = {255,255,255,255};
    u8  keyArr[8];
    u8  sw1 = FALSE;
    u8  keyPressd = FALSE;
    int i;
    int cnt = 0;



    for (i=0; i<8; i++)
    	keyArr[i] = 0x80;

	for (i=0; i<NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		if (readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1){
			if(!keyPressd){
				keyPressd =TRUE;
				if(sw1 == TRUE)
					sw1 = FALSE;
				else if(sw1 == FALSE)
					sw1 = TRUE;
				for (i=0; i<8; i++)
					keyArr[i] = 0x80;
			}
		}
		else
			keyPressd = FALSE;


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
			if(key != bKey[scan]){
				keyArr[cnt%8] = key;
				cnt++;
				bKey[scan] = key;
			}
		}
		else
			bKey[scan] = key;

		if(sw1 == FALSE){
			for(i=0; i<8; i++){
				writeb_virtual_io(SEG7LED, 0x00+(0x10*i) + keyArr[(cnt+i)%8]);
			}
		}
		else if(sw1 == TRUE){
			for(i=0; i<8; i++){
				writeb_virtual_io(SEG7LED, 0x70-(0x10*i) + keyArr[(cnt+i)%8] );
			}
		}

		if (NDS_SWITCH() & KEY_START)
			break;

		vTaskDelay(MSEC2TICK(30));
	}
	while (NDS_SWITCH() & KEY_START){
		vTaskDelay(MSEC2TICK(10));	// Wait for releasing START KEY
	}
}

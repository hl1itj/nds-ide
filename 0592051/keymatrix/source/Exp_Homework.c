#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void reset(void){
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)
			writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

}

void
Exp_4_Homework_A(void)
{
	u8 key, scan = 0;
	u8 Mode_Change = FALSE;



	while (1) {

		switch(readb_virtual_io(PUSH_SW)){
		case VIRTUAL_SW_1:
			reset();
			Mode_Change = TRUE;
			break;
		case VIRTUAL_SW_2:
			break;
		case VIRTUAL_SW_3:
			break;
		case VIRTUAL_SW_4:
			break;

		default:
			break;
		}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}


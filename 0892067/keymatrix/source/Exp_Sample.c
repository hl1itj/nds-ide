/*
 * Exp_Sample.c
 *
 *  Created on: 2010. 10. 11.
 *      Author: Minsuk Lee
 */

#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void
Exp_4_Sample_A(void)
{
    u8 key, scan = 0;
    int i;

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		switch (readb_virtual_io(PUSH_SW)) {
		  case VIRTUAL_SW_1 : writeb_virtual_io(SEG7LED, 0x01); break;
		  case VIRTUAL_SW_2 : writeb_virtual_io(SEG7LED, 0x02); break;
		  case VIRTUAL_SW_3 : writeb_virtual_io(SEG7LED, 0x03); break;
		  case VIRTUAL_SW_4 : writeb_virtual_io(SEG7LED, 0x04); break;
		  default :           writeb_virtual_io(SEG7LED, 0x80); break;
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
		if (key < 16)
			writeb_virtual_io(SEG7LED, 0x70 + key);

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(30));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait for releasing START KEY
}


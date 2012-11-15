#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG		10

void
key_init(void)
{
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int
kbhit(void)
{
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 1);
	return (ret == pdPASS);
}
u8
getkey(void)
{
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	//while (pdPASS != xQueueReceive(KeyQueue, &key, 0))
	//		vTaskDelay(MSEC2TICK(5));
	return key;
}

void
Exp_5_Homework_A(void)
{
    key_init();
	u8 key[8];
    u8 key_dump;
	writeb_virtual_io(BARLED1, 0x00);

    int i;
    // Variables

    for (i = 0; i < 8 ; i++)
 		key[i] = 20;

    while (1) {
    	key_dump = getkey();

    	if (key_dump == 0)
    		break;

		if (key_dump < 16) {
			for (i = 7; i > 0 ; i--) key[i] = key[i-1];
			key[0] = key_dump;
		}

		for (i = 0; i < 8; i++) {
			if (key[i] < 16)
				writeb_virtual_io(SEG7LED, (0x70 - (0x01<<4)*i) + key[i]);
			else
				writeb_virtual_io(SEG7LED, 0x80 + ((8-1-i) << 4));
		}
	}
}

void
Exp_5_Homework_B(void)
{
    key_init();
    u8 key[8];
    u8 key_dump;
    u8 barled = 0x80;
    u8 left_right = FALSE;
    portTickType xLastWakeTime = xTaskGetTickCount();
    int i;
    // Variables

    for (i = 0; i < 8 ; i++)
 		key[i] = 20;

	while (1) {

		if (kbhit()) {
			key_dump = getkey();
	    	if (key_dump == 200)
	    		break;
			if (key_dump < 16) {
				for (i = 7; i > 0 ; i--) key[i] = key[i-1];
				key[0] = key_dump;
			}
		}

		if ((xLastWakeTime+MSEC2TICK(500))<xTaskGetTickCount()) {
			xLastWakeTime = xTaskGetTickCount();
			if (barled == 0x80)
				left_right = TRUE;
			if (barled == 0x01)
				left_right = FALSE;
			if(left_right)
				barled = barled >> 1;
			else
				barled = barled << 1 ;
		}

		for (i = 0; i < 8; i++) {
			if (key[i] < 16)
				writeb_virtual_io(SEG7LED, (0x70 - (0x01<<4)*i) + key[i]);
			else
				writeb_virtual_io(SEG7LED, 0x80 + ((8-1-i) << 4));
		}

		writeb_virtual_io(BARLED1, barled);

	}

}

portTASK_FUNCTION(Key_Task, pvParameters)
{
    u8 key_dump;
    u8 scan = 0;
    u8 key_pressed = FALSE;
    u8 key_temp;
    // Variables


    while (1) {
		if (NDS_SWITCH() & KEY_START){
			key_dump = 200;
			xQueueSend(KeyQueue, &key_dump, 0);
		}

		if (key_pressed == FALSE) {
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
			if (scan == 4) scan = 0;

			if (key_dump < 16) {
				key_pressed = TRUE;
				xQueueSend(KeyQueue, &key_dump, 0);

			}
		}

		vTaskDelay(MSEC2TICK(20));
		if (key_pressed && (readb_virtual_io(KEY_MATRIX) != key_temp))	key_pressed = FALSE;

    }
}

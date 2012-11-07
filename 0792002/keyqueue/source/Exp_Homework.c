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
	int ret = xQueuePeek(KeyQueue, &key, 0);
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
	int i;
	u8 key[NUM_7SEG_LED];

	for(i=0; i<NUM_7SEG_LED; i++){
		writeb_virtual_io(SEG7LED,0x80+(i<<4));
		key[i]=1<<7;
	}

	while(1){

		for(i=0;i<NUM_7SEG_LED-1;i++){
			key[i]=key[i+1];
		}
		key[NUM_7SEG_LED-1]=getkey();
		if(key[NUM_7SEG_LED-1]==0)
			break;
		for(i=0; i<NUM_7SEG_LED;i++)
			writeb_virtual_io(SEG7LED,key[i]+(i<<4));

		vTaskDelay(MSEC2TICK(20));
	}
	key_init();
}

void
Exp_5_Homework_B(void)
{
	int i;
	u8 key[NUM_7SEG_LED];
	u8 barled=0x80;
	u8 mode=FALSE;

	for(i=0; i<NUM_7SEG_LED; i++){
		writeb_virtual_io(SEG7LED,0x80+(i<<4));
		key[i]=1<<7;
	}
	portTickType xLastWakeTime = xTaskGetTickCount();

	while(1){
		writeb_virtual_io(BARLED1,barled);

		if(xTaskGetTickCount()> xLastWakeTime +MSEC2TICK(450)){
			if(barled==0x80)
				mode=FALSE;
			else if(barled==0x01)
				mode=TRUE;
			if(mode==FALSE)
				barled=barled >> 1;
			else
				barled = barled <<1;
			vTaskDelayUntil(&xLastWakeTime,MSEC2TICK(500));
		}
		while(kbhit()){
			for(i=0;i<NUM_7SEG_LED;i++)
				key[i]=key[i+1];
			key[NUM_7SEG_LED-1]=getkey();

			for(i=0;i<NUM_7SEG_LED;i++)
				writeb_virtual_io(SEG7LED,key[i]+(i<<4));
		}
		if(NDS_SWITCH()&KEY_START)
			break;
	}
	while(NDS_SWITCH()&KEY_START)
		vTaskDelay(MSEC2TICK(10));
	key_init();
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	u8 key,scan=0,sw_pressed=FALSE,sw2;

	while(1){
		if(sw_pressed && (readb_virtual_io(KEY_MATRIX) != sw2 || !readb_virtual_io(KEY_MATRIX)))
				sw_pressed = FALSE;
			if (!sw_pressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			sw2=readb_virtual_io(KEY_MATRIX);

			switch (sw2) {
			case 8:
				key += 1;
				break;
			case 4:
				key += 2;
				break;
			case 2:
				key += 3;
				break;
			case 1:
				key += 4;
				if (key == 16)
					key = 0;
				break;
			default:
				key = 255;
				break;
			}
			scan++;
			if (scan == 4)
				scan = 0;

			if (key < 16) {
				xQueueSend(KeyQueue,&key, portMAX_DELAY);
				sw_pressed=TRUE;
			}
			vTaskDelay(MSEC2TICK(30));
	}
	}
}

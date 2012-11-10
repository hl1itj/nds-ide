#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

extern xQueueHandle KeyQueue;
#define MAX_KEY_LOG        10

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
	//        vTaskDelay(MSEC2TICK(5));
	return key;
}

typedef struct ArrayType{
	int Data[8]; //LED가 저장될 배열
	int Location; //현재 켜져있는 위치
}ArrayType;

void initArray(ArrayType *a){
	int i=0;
	while(i<8){
		a->Data[i]=16;
		i++;
	}
}

void display(ArrayType *a){
	int i=0;
	while(i<8){
		if(a->Data[i]!=16)
			writeb_virtual_io(SEG7LED, (i << 4) + a->Data[i]);
		i++;
	}
}

ArrayType array;

void
Exp_5_Homework_A(void)
{
	int i;
	u8 key;

    while(1){
    	if(!(key=getkey())){
    		for (i = 0; i < NUM_7SEG_LED; i++)    // Turn Off All
    			writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
    		initArray(&array);
    		break;
    	}

		if(array.Location!=0)
			array.Location--;
		for(i=array.Location; i<7; i++){
			array.Data[i]=array.Data[i+1];
		}
		array.Data[7]=key;
		display(&array);
	}
}

void
Exp_5_Homework_B(void)
{
	int count=0, i;
	u8 key;
	u16 sw;
	portTickType xLastWakeTime =xTaskGetTickCount();
	u8 led_state = 0x80;
	int rightMove=1; //led right state
	while(1){
		count++;
		if(count==5){
			if(rightMove && led_state==0x01){
				rightMove=0;
				led_state*=2;
			}
			else if(!rightMove && led_state==0x80){
				rightMove=1;
				led_state/=2;
			}
			else if(rightMove){
				led_state/=2;
			}
			else if(!rightMove){
				led_state*=2;
			}
			writeb_virtual_io(BARLED1, led_state);
			count=0;
		}
		if(kbhit()){
			key=getkey();
			if(array.Location!=0)
				array.Location--;
			for(i=array.Location; i<7; i++){
				array.Data[i]=array.Data[i+1];
			}
			array.Data[7]=key;
			display(&array);
		}
		sw = NDS_SWITCH();
		if(sw & KEY_START){
			for (i = 0; i < NUM_7SEG_LED; i++)    // Turn Off All
			    writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
			initArray(&array);
			break;
		}
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(100));
	}
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	u8 key, scan = 0;
	int key_pressed=0;
	int i;

	for (i = 0; i < NUM_7SEG_LED; i++)    // Turn Off All
			writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	initArray(&array);

	while (1) {
		if(key_pressed==0){
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			switch (readb_virtual_io(KEY_MATRIX)) {
			case 8 : key += 1; break;
			case 4 : key += 2; break;
			case 2 : key += 3; break;
			case 1 : key += 4; if (key == 16) key = 0; break;
			default : key = 255; break;
			}
			scan++;
			if (scan == 4)
				scan = 0;
			if (key < 16){
				key_pressed=1;
				xQueueSend(KeyQueue, &key, 0);
			}

		}
		if(readb_virtual_io(KEY_MATRIX)==0)
			key_pressed=0;

		vTaskDelay(MSEC2TICK(30));
	}
}

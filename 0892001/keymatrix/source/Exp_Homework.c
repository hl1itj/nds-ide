#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"
typedef struct ArrayType{
	int Data[8]; //LED가 저장될 배열
	int Location; //현재 켜져있는 위치
	int Is_Right; //1이면 오른쪽부터 0이면 왼쪽부터
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

void
Exp_4_Homework_A(void)
{
    u8 before_key, key, scan = 0,before_scan;
    int i;
    int key_pressed=0;
    int key_pressed2=0;
    ArrayType array;
    initArray(&array);
    array.Is_Right=1;//오른쪽 부터 켜지는 상태
    array.Location=8; //가장 오른쪽에 있는 상태

	for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
		writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

	while (1) {
		switch (readb_virtual_io(PUSH_SW)) {
		  case VIRTUAL_SW_1 :
			  if(key_pressed2==0){
				  key_pressed2=1;
				  for (i = 0; i < NUM_7SEG_LED; i++)					// Turn Off All
						writeb_virtual_io(SEG7LED, 0x80 + (i << 4));
				  initArray(&array);
				  if(array.Is_Right==1){
					  array.Is_Right=0;
					  array.Location=-1;
				  }
				  else if(array.Is_Right==0){
					  array.Is_Right=1;
					  array.Location=8;
				  }
			  }
			  break;

		  default : key_pressed2=0;
			  break;
		}

		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;
		switch (readb_virtual_io(KEY_MATRIX)) {
		  case 8 :  key += 1;
		  	  	  	  break;
		  case 4 :  key += 2;
		  	  	  	  break;
		  case 2 :  key += 3;
		  	  	  	  break;
		  case 1 :  key += 4;
		  	  	  	  if (key == 16)
		  	  	  		  key = 0;
		  	  	  	  break;
		  default : key = 255;
		  	  	  	  break;
		}
		scan++;
		if (scan == 4)
			scan = 0;
		if(before_scan==scan && key_pressed==1 && before_key!=key )
			key_pressed=0;
		if (key < 16 && key_pressed==0){
			before_scan=scan;
			before_key=key;
			key_pressed=1;
			if(array.Is_Right==1){
				if(array.Location!=0)
					array.Location--;
				for(i=array.Location; i<7; i++){
					array.Data[i]=array.Data[i+1];
				}
				array.Data[7]=key;
				display(&array);
			}
			else if(array.Is_Right==0){
				if(array.Location!=7)
					array.Location++;
				for(i=array.Location-1; i>=0; i--){
					array.Data[i+1]=array.Data[i];
				}
				array.Data[0]=key;
				display(&array);
			}
		}



		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(30));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait for releasing START KEY
}


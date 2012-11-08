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

enum {LEFT,RIGHT};

enum {NOPUSH,PUSH};

u8 key;

u8 key_pressed = 0;

u8 scan = 0;

u8 bKey[4] = {255,255,255,255};

u8 keyQue[8];

int i;

int keyQueFlag = 0;

int LeftRight = LEFT;

for (i=0; i<8; i++)

keyQue[i] = 0x80;

for (i=0; i<NUM_7SEG_LED; i++)

writeb_virtual_io(SEG7LED, 0x80 + (i << 4));

while (1) {

if (readb_virtual_io(PUSH_SW) == VIRTUAL_SW_1){

if(key_pressed == NOPUSH){

if(LeftRight == RIGHT)

LeftRight = LEFT;

else if(LeftRight == LEFT)

LeftRight = RIGHT;

for (i=0; i<8; i++)

keyQue[i] = 0x80;

key_pressed = PUSH;

}

}

else

key_pressed = NOPUSH;

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

if(key != bKey[scan]){

keyQue[keyQueFlag%8] = key;

keyQueFlag++;

bKey[scan] = key;

}

}

else

bKey[scan] = key;

if(LeftRight == RIGHT){

for(i=0; i<8; i++){

u8 ledPos = 0x00+(0x10*i);

u8 ledNum = keyQue[(keyQueFlag+i)%8];

writeb_virtual_io(SEG7LED, ledPos + ledNum);

}

}

else if(LeftRight == LEFT){

for(i=0; i<8; i++){

u8 ledPos = 0x70-(0x10*i);

u8 ledNum = keyQue[(keyQueFlag+i)%8];

writeb_virtual_io(SEG7LED, ledPos + ledNum );

}

}

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}


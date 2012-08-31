/*
 * Exp_Sample.c
 *
 *  Created on: 2011. 11. 9.
 *      Author: Minsuk Lee
 */

// Free RTOS Headers
// See AI reference Document in http://www.freertos.org/
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <nds.h>
#include <sevencore_io.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define BG_GFX			((u16*)0x6000000)		/**< \brief background graphics memory*/
/** \brief  Screen height in pixels */
#define SCREEN_HEIGHT 192
/** \brief  Screen width in pixels */
#define SCREEN_WIDTH  256

#define NUM_TASK 6

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_GREEN     RGB( 0, 31,  0) /* Bright Green	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)		//16
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)	//12

#define DIRECTION_RIGHT	0
#define DIRECTION_DOWN	1

struct parameters{
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};
struct parameters Param[NUM_TASK] = {
		{ "1", DIRECTION_RIGHT,	3,	 COLOR_RED,		60	},
		{ "2", DIRECTION_RIGHT,	6,	 COLOR_RED,		80	},
		{ "3", DIRECTION_RIGHT,	9,	 COLOR_RED,		100	},
		{ "4", DIRECTION_DOWN,	4,	 COLOR_GREEN,	90	},
		{ "5", DIRECTION_DOWN,	8,	 COLOR_GREEN,	120	},
		{ "6", DIRECTION_DOWN,	12,	 COLOR_GREEN,	150	},
};

portTASK_FUNCTION(Ball_Task, pvParameters);

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
draw_my_box(int pos_x, int pos_y, u16 color)
{
	int i, j;
    u32 *basePoint, pixel;

    pixel = (color << 16) + color;
    for (i = 0; i < BOX_HEIGHT; i++) {
 	    basePoint = (u32 *)BG_GFX +
 	      ((((pos_y * BOX_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * BOX_WIDTH) / 2;
    	for (j = 0; j < (BOX_WIDTH / 2); j++)
    		*basePoint++ = pixel;
	}
}

void
Exp_Sample(void)
{
    u8 key, old_key = -1;
    struct parameters *p;

    int i;
    for(i = 0, p = Param; i<NUM_TASK; i++, p++){
    	xTaskCreate(Ball_Task, (const signed char * ) p->taskname, 1024,
    			(void *)p, tskIDLE_PRIORITY + 5, NULL);
    }

    while (1) {
 		if (!kbhit()) {
 	 		if (NDS_SWITCH() & KEY_START)
 				break;
 			vTaskDelay(30);
 			continue;
 		}
 		key = getkey();
 		/*
 		if (key == old_key)
 			continue;

 		if (old_key >= 0)
 			draw_my_box(old_key, 8, COLOR_BLACK);	// Erase the Previous Box
 		draw_my_box(key, 8, COLOR_RED);	 			// Draw a New Box
 	    old_key = key;
 	    */
    }
	draw_my_box(old_key, 8, COLOR_BLACK);			// Erase the Previous Box
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}




portTASK_FUNCTION(Ball_Task, pvParameters)
{
    struct parameters *p = (struct parameters*)pvParameters;
    u8 pos = 0;
    u8 old_pos = -1;
    int mode = 0;
    //초기좌표설정
    if(p->direction == DIRECTION_RIGHT){
    	pos = p->basePoint;
    }else if(p->direction == DIRECTION_DOWN){
    	pos = (p->basePoint << 4);
    }

    while(1){
    	if (old_pos >= 0)
    		draw_my_box(old_pos>>4, old_pos&0x0f, COLOR_BLACK);	// Erase the Previous Box
    	draw_my_box(pos>>4, pos&0x0f, p->color);	 			// Draw a New Box

    	old_pos = pos;

    	if(p->direction == DIRECTION_RIGHT){
    		if(mode == 0)
    			pos = pos + 0x10;
    		else
    			pos = pos - 0x10;

    		if(pos >> 4 == (MAX_X-1))
    			mode = 1;
    		else if(pos >> 4 == 0)
    			mode = 0;
    	}else if(p->direction == DIRECTION_DOWN){
    		if(mode == 0)
    			pos += 0x01;
    		else
    			pos -= 0x01;

    		if((pos&0x0f) == (MAX_Y-1))
    			mode = 1;
    		else if((pos&0x0f) == 0)
    			mode = 0;
    	}
    	/*
    	if(p->direction == DIRECTION_RIGHT){
    		if(mode == 0){
    			if((pos&0xf0) == 0xf0)
    				mode = 1;
    			else
    				pos += 0x10;
    		}else{
    			if((pos&0xf0) == 0x00)
    				mode = 0;
    			else
    				pos -= 0x10;
    		}
    	}else if(p->direction == DIRECTION_DOWN){
    		if(mode == 0){
    			if((pos&0x0f) == 0xf0)
    				mode = 1;
    			else
    				pos += 0x01;
    		}else{
    			if((pos&0x0f) == 0x00)
    				mode = 0;
    			else
    				pos -= 0x01;
    		}
    	}
    	*/


    	vTaskDelay(p->delay);
    }
}

portTASK_FUNCTION(Key_Task, pvParameters)
{

/*	CASE 1. Input에서 kbhit() 적용한 경우		*/

	u8 key, scan= 0;
	u8 old_key = 0;
	u8 input_key = 0;
	u8 button = 0;

	key_init();

	//writeb_virtual_io(BARLED2, 0x88);
	while (1) {

		writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
		key = scan * 4;

		switch (input_key = readb_virtual_io(KEY_MATRIX)) {
		case 8: key += 1; break;
		case 4: key += 2; break;
		case 2: key += 3; break;
		case 1: key += 4; if (key == 16) key = 0; break;
		default: key = 255; break;
		}
		scan++;
		if (scan == 4)
			scan = 0;

		if(((old_key >> 4) == scan) && (old_key & 0x0f) != button){
			old_key = 0;
		}

		if (button == FALSE) {
			if (key < 16) {
				writeb_virtual_io(BARLED2, key);
				xQueueSend(KeyQueue, &key, 0);
				button = (scan << 4) + input_key;
			}
		}
		if((button >> 4) == scan && ((button & 0x0f) != input_key))
			button = FALSE;

		vTaskDelay(MSEC2TICK(10));
	}

}

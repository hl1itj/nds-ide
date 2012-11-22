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
#define MAX_WIDTH 		(SCREEN_WIDTH / BOX_WIDTH)		//16
#define MAX_HEIGHT		(SCREEN_HEIGHT / BOX_HEIGHT)	//12
#define BASIC_DIR	0
#define REVERSE_DIR	1
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
    u8 bef = -1;
    struct parameters *p;

    int i;
    for(i = 0, p = Param; i<NUM_TASK; i++, p++){
    	xTaskCreate(Ball_Task, (const signed char * ) p->taskname, 1024,
    			(void *)p, tskIDLE_PRIORITY + 5, NULL);
    }

    while (1){
    	if(NDS_SWITCH() & KEY_START)
    		break;
 		vTaskDelay(30);
    }

	//draw_my_box(bef, 8, COLOR_BLACK);
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}




portTASK_FUNCTION(Ball_Task, pvParameters)
{
	u8 location= 0;
	    u8 bef = -1;
	    int state = 0;
	struct parameters *para = (struct parameters*)pvParameters;

    if(para->direction == DIRECTION_RIGHT){
    	location = para->basePoint;
    }else if(para->direction == DIRECTION_DOWN){
    	location = (para->basePoint << 4);
    }

	if (bef >= 0)
		draw_my_box(bef>>4, bef&0x0f, COLOR_BLACK);
	draw_my_box(location>>4, location&0x0f,para->color);

	bef = location;

    do{

    	if(para->direction == DIRECTION_RIGHT){
    		if(state == BASIC_DIR)
    			location = location + 0x10;
    		//가던방향으로 계속 더한다
    		else if(state == REVERSE_DIR)
    			location = location - 0x10;
// 반대방향으로 뺴준다.
    		if(location >> 4 == (MAX_WIDTH-1))
    			state = REVERSE_DIR;
    		//끝에 닿았을때 방향을 전환해준다.
    		else if(location >> 4 == BASIC_DIR)
    			state = BASIC_DIR;
    	}else if(para->direction == DIRECTION_DOWN){
    		if(state == BASIC_DIR)
    			location = location + 0x01;
    		//상하방향일떄 계속 진행한다.
    		else if(state == REVERSE_DIR)
    			location = location - 0x01;
//상하방향일때 반대로 값을 빼준다.
    		if((location&0x0f) == (MAX_HEIGHT-1))
    			state = REVERSE_DIR;
    		//화면 끝에 닿았을때 반대방향으로 빼준다.
    		else if((location&0x0f) == BASIC_DIR)
    			state = BASIC_DIR;
    	}

    	if (bef >= 0)
        		draw_my_box(bef>>4, bef&0x0f, COLOR_BLACK);
        	draw_my_box(location>>4, location&0x0f, para->color);

        	bef = location;
        	vTaskDelay(para->delay);
    	}while(1);

    //	vTaskDelay(para->delay);
    }

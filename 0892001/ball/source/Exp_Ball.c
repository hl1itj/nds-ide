/*
 * Exp_Ball.c
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

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_GREEN     RGB(0,  31,  0) /* Bright Green */
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

// define added 11/10/2011
#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256
#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1
#define NUM_TASK 6

struct parameters{
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};


// fucntion added 11/10/2011


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
Exp_Ball(struct parameters *p)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	int currentPoint=0;
	int plusState=1;


    while (1) {
    	if(p->direction==DIRECTION_RIGHT)
    	 	draw_my_box(currentPoint, p->basePoint, p->color );
    	else if(p->direction==DIRECTION_DOWN)
    	 	draw_my_box(p->basePoint,currentPoint, p->color);// Draw a New Box

    	vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(300));

 		if(p->direction==DIRECTION_RIGHT)
 			draw_my_box(currentPoint, p->basePoint, COLOR_BLACK );	// Erase the Previous Box
 		else if(p->direction==DIRECTION_DOWN)
 			draw_my_box(p->basePoint,currentPoint, COLOR_BLACK);

 		if(p->direction==DIRECTION_RIGHT){
 			if(currentPoint==MAX_X-1 && plusState){
 				plusState=0;
 				currentPoint--;
 			}
 			else if(currentPoint==0 && !plusState){
 				plusState=1;
 				currentPoint++;
 			}
 			else if(plusState){
 				currentPoint++;
 			}
 			else if(!plusState)
 				currentPoint--;
 		}
 		else if(p->direction==DIRECTION_DOWN){
 			if(currentPoint==MAX_Y-1 && plusState){
 				plusState=0;
 				currentPoint--;
 			}
 			else if(currentPoint==0 && !plusState){
 				plusState=1;
 				currentPoint++;
 			}
 			else if(plusState){
 				currentPoint++;
 			}
 			else if(!plusState)
 				currentPoint--;
 		}

 		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(p->delay));

    }

}

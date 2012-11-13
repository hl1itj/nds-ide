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

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)
#define COLOR_GREEN	   RGB(0,  31,  0) /* Bright Green */

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

// define added 11/10/2011
#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT 192

#define NUM_TASK     6
#define DIRECTION_RIGHT 0
#define DIRECTION_LEFT 1
#define DIRECTION_DOWN 2
#define DIRECTION_UP 3

#define BOTTOM 0
#define LEFT 0

struct parameters {
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

struct pos {
	int x;
	int y;
};

void pos_init(struct pos *pos, struct parameters *p) {
	if (p->direction == DIRECTION_RIGHT) {
		pos->x = 0;
		pos->y = p->basePoint;
	} else if (p->direction == DIRECTION_DOWN) {
		pos->x = p->basePoint;
		pos->y = 0;
	}
}

void Exp_Homework(struct parameters *p) {
	struct pos pos, old_pos;

	pos_init(&pos, p);

	while (1) {
		draw_my_box(old_pos.x, old_pos.y, COLOR_BLACK);	// Erase the Previous Box
		draw_my_box(pos.x, pos.y, p->color);	 // Draw a New Box

		vTaskDelay(300);

		old_pos = pos;

		switch (p->direction) {
		case DIRECTION_RIGHT:
			if (pos.x < MAX_X - 1) {
				pos.x++;
			} else {
				pos.x--;
				p->direction = DIRECTION_LEFT;
			}
			break;
		case DIRECTION_LEFT:
			if (pos.x > BOTTOM) {
				pos.x--;
			} else {
				pos.x++;
				p->direction = DIRECTION_RIGHT;
			}
			break;
		case DIRECTION_DOWN:
			if (pos.y < MAX_Y - 1) {
				pos.y++;
			} else {
				pos.y--;
				p->direction = DIRECTION_UP;
			}
			break;
		case DIRECTION_UP:
			if (pos.y > LEFT) {
				pos.y--;
			} else {
				pos.y++;
				p->direction = DIRECTION_DOWN;
			}
			break;
		default:
			// no effect
			break;

		}
		vTaskDelay(p->delay);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

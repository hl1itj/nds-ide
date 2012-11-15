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
#define COLOR_GREEN     RGB( 0,  31,  0)
#define COLOR_BLACK	   RGB( 0,   0,   0)

#define DIRECTION_RIGHT		1
#define DIRECTION_LEFT		2
#define DIRECTION_DOWN  		3
#define DIRECTION_UP			4

#define BOX_WIDTH		16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)
#define MIN_X     0
#define MIN_Y     0

// define added 11/10/2011
#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT 192

struct parameters {
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

void
Exp_Homework(struct parameters *p)
{
	int pos_x, pos_y;
	int old_pos_x, old_pos_y;

	if(p->direction == DIRECTION_RIGHT) {
		pos_x = MIN_X;
		pos_y = p->basePoint;
	}
	else if(p->direction == DIRECTION_DOWN) {
		pos_x = p->basePoint;
		pos_y = MIN_Y;
	}

	while (1) {
		draw_my_box(old_pos_x, old_pos_y, COLOR_BLACK);
		draw_my_box(pos_x, pos_y, p->color);

		old_pos_x = pos_x;
		old_pos_y = pos_y;

		vTaskDelay(300);

		if(p->direction == DIRECTION_RIGHT) {
			if(pos_x < (MAX_X - 1)) {
				pos_x++;
			}
			else {
				p->direction = DIRECTION_LEFT;
				pos_x--;
			}
		}
		else if (p->direction == DIRECTION_LEFT) {
			if(pos_x > MIN_X) {
				pos_x--;
			}
			else {
				p->direction = DIRECTION_RIGHT;
				pos_x++;
			}
		}
		else if (p->direction == DIRECTION_DOWN) {
			if(pos_y < (MAX_Y -1)) {
				pos_y++;
			}
			else {
				p->direction = DIRECTION_UP;
				pos_y--;
			}
		}
		else {
			if(pos_y > MIN_Y) {
				pos_y--;
			}
			else {
				p->direction = DIRECTION_DOWN;
				pos_y++;
			}
		}
		vTaskDelay(p->delay);
	}

	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

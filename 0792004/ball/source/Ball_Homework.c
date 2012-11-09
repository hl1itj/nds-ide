// Free RTOS Headers
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "sevencore_io.h"

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

#define DIRECTION_UP		1
#define DIRECTION_DOWN	2
#define DIRECTION_LEFT	3
#define DIRECTION_RIGHT	4

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)

#define BOX_WIDTH		16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

struct parameters {
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

void Ball_Homework(struct parameters *p)
{
	int pos_x, pos_y;
	int old_pos_x, old_pos_y;
	int direction = p->direction;
	// int delay_check = 0;
	// const int DELAY = 10;

	if (direction == DIRECTION_RIGHT) {
		pos_x = 0;
		pos_y = p->basePoint;
	}
	else if (direction == DIRECTION_DOWN) {
		pos_x = p->basePoint;
		pos_y = 0;
	}

	old_pos_x = pos_x;
	old_pos_y = pos_y;

	// portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {

		// delay_check += DELAY;

		draw_my_box(old_pos_x, old_pos_y, COLOR_BLACK);
		draw_my_box(pos_x, pos_y, p->color);

		old_pos_x = pos_x;
		old_pos_y = pos_y;

				vTaskDelay(300);

		// if (delay_check == (300 + p->delay)) {
			// delay_check = 0;
			if (direction == DIRECTION_UP) {
				if (pos_y > 0)
					pos_y--;
				else {
					direction = DIRECTION_DOWN;
					pos_y++;
				}
			}
			else if (direction == DIRECTION_DOWN) {
				if (pos_y < (MAX_Y-1))
					pos_y++;
				else {
					direction = DIRECTION_UP;
					pos_y--;
				}
			}
			else if (direction == DIRECTION_LEFT) {
				if (pos_x > 0)
					pos_x--;
				else {
					direction = DIRECTION_RIGHT;
					pos_x++;
				}
			}
			else if (direction == DIRECTION_RIGHT) {
				if (pos_x < (MAX_X - 1))
					pos_x++;
				else {
					direction = DIRECTION_LEFT;
					pos_x--;
				}
			}
		//}
		vTaskDelay(p->delay);
		//vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(DELAY));
	}
}

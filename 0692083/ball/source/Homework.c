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

#include "sevencore_io.h"

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)
#define COLOR_GREEN     RGB(0,  31,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT 3

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

void draw_my_box(int pos_x, int pos_y, u16 color) {
	int i, j;
	u32 *basePoint, pixel;

	pixel = (color << 16) + color;
	for (i = 0; i < BOX_HEIGHT; i++) {
		basePoint = (u32 *) BG_GFX
				+ ((((pos_y * BOX_HEIGHT) + i) * SCREEN_WIDTH)
						+ pos_x * BOX_WIDTH) / 2;
		for (j = 0; j < (BOX_WIDTH / 2); j++)
			*basePoint++ = pixel;
	}
}

void Homework(struct parameters *p) {

	int x;
	int y;

	int before_x;
	int before_y;

	int direct = p->direction;

	if (direct == DIRECTION_DOWN) {

		x = p->basePoint;
		y = 0;

	}

	else if (direct == DIRECTION_RIGHT) {

		x = 0;
		y = p->basePoint;

	}

	before_x = x;
	before_y = y;

	while (1) {

		draw_my_box(before_x, before_y, COLOR_BLACK); // 지우기
		draw_my_box(x, y, p->color);

		printf("%d", p->delay);

		vTaskDelay(300);


		before_x = x;
		before_y = y;

		if (direct == DIRECTION_UP) {

			if (y > 0) {

				y--;
			}

			else {

				direct = DIRECTION_DOWN;
				y++;
			}

		}

		if (direct == DIRECTION_DOWN) {

			if (y < (MAX_Y - 1)) {

				y++;

			} else {

				direct = DIRECTION_UP;
				y--;
			}

		}

		if (direct == DIRECTION_RIGHT) {

			if (x < (MAX_X - 1)) {

				x++;
			}

			else {

				direct = DIRECTION_LEFT;
				x--;
			}
		}

		if (direct == DIRECTION_LEFT) {
			if (x > 0) {

				x--;
			}

			else {

				direct = DIRECTION_RIGHT;
				x++;

			}

		}

		vTaskDelay(p->delay);

	}

}

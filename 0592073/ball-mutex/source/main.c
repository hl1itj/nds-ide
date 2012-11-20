#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

#define COLOR_RED       RGB(31,  0,  0)
#define COLOR_GREEN     RGB(0,  31,  0)
#define COLOR_BLACK     RGB(0,   0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

#define DIRECTION_UP        1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3
#define DIRECTION_RIGHT		4

#define LEFT   0
#define BOTTOM 0

#define NUM_ROW		3
#define NUM_COL		3
#define NUM_TASK	(NUM_ROW + NUM_COL)

#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	 256
#define SCREEN_HEIGHT 192

#define FIRST_CROSS_X 4
#define SECOND_CROSS_X 8
#define THIRD_CROSS_X 12

#define FIRST_CROSS_Y 3
#define SECOND_CROSS_Y 6
#define THIRD_CROSS_Y 9

struct parameters {
	char *taskname;		// Task Name
	int direction;		// Current Moving Direction
	int basePoint;		// Starting Position
	u32 color;			// Ball Color
	int delay;			// Task Delay
};

struct parameters Param[NUM_TASK] = {
		{ "1", DIRECTION_RIGHT, 3, COLOR_RED,   350 },
		{ "2", DIRECTION_RIGHT, 6, COLOR_RED,   300 },
		{ "3", DIRECTION_RIGHT, 9, COLOR_RED,   320 },
		{ "4", DIRECTION_DOWN,  4, COLOR_GREEN, 370 },
		{ "5", DIRECTION_DOWN,  8, COLOR_GREEN, 400 },
		{ "6", DIRECTION_DOWN, 12, COLOR_GREEN, 330 }
};

struct pos {
	int x;
	int y;
};

static portTASK_FUNCTION(Ball_Task, pvParameters);
void InitDebug(void);
void vStartExpTasks(void);
void draw_my_box(int pos_x, int pos_y, u16 color);

xSemaphoreHandle xSemaphore[9];

int main(void) {
	InitDebug();
	init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()

	vStartExpTasks();
	vTaskStartScheduler();		// Never returns
	while (1)
		;
	return 0;
}

void InitDebug(void) {
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

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

void vStartExpTasks(void) {
	struct parameters *p;
	int i;

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	for (i = 0, p = Param; i < NUM_TASK; i++, p++) {
		xTaskCreate(Ball_Task, (const signed char *)(p->taskname), 1024,
				(void *)p, tskIDLE_PRIORITY + 5, NULL);

	}

	for (i = 0; i < (NUM_COL * NUM_ROW); i++) {
		vSemaphoreCreateBinary(xSemaphore[i]);
	}
}

void pos_init(struct pos *pos, struct parameters *p) {
	if (p->direction == DIRECTION_RIGHT) {
		pos->x = 0;
		pos->y = p->basePoint;
	} else if (p->direction == DIRECTION_DOWN) {
		pos->x = p->basePoint;
		pos->y = 0;
	}
}

static portTASK_FUNCTION(Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	struct pos pos, prev_pos;
	pos_init(&pos, p);

	while (1) {

		if (NDS_SWITCH() & KEY_R) {
			switch (pos.x) {
			case FIRST_CROSS_X:
				if (pos.y == FIRST_CROSS_Y) {
					xSemaphoreTake(xSemaphore[0], (portTickType)410);
				} else if (pos.y == SECOND_CROSS_Y) {
					xSemaphoreTake(xSemaphore[1], (portTickType)410);
				} else if (pos.y == THIRD_CROSS_Y) {
					xSemaphoreTake(xSemaphore[2], (portTickType)410);
				}
				break;
			case SECOND_CROSS_X:
				if (pos.y == FIRST_CROSS_Y) {
					xSemaphoreTake(xSemaphore[3], (portTickType)410);
				} else if (pos.y == SECOND_CROSS_Y) {
					xSemaphoreTake(xSemaphore[4], (portTickType)410);
				} else if (pos.y == THIRD_CROSS_Y) {
					xSemaphoreTake(xSemaphore[5], (portTickType)410);
				}
				break;
			case THIRD_CROSS_X:
				if (pos.y == FIRST_CROSS_Y) {
					xSemaphoreTake(xSemaphore[6], (portTickType)410);
				} else if (pos.y == SECOND_CROSS_Y) {
					xSemaphoreTake(xSemaphore[7], (portTickType)410);
				} else if (pos.y == THIRD_CROSS_Y) {
					xSemaphoreTake(xSemaphore[8], (portTickType)410);
				}
				break;
			default:
				// no effect
				break;
			}
		}
		draw_my_box(prev_pos.x, prev_pos.y, COLOR_BLACK);	// ���� ��ġ ����
		draw_my_box(pos.x, pos.y, p->color);		// �� ��ġ�� �׸�
		vTaskDelay(MSEC2TICK(p->delay) );		// Delay

		switch (pos.x) {
		case FIRST_CROSS_X:
			if (pos.y == FIRST_CROSS_Y) {
				xSemaphoreGive(xSemaphore[0]);
			} else if (pos.y == SECOND_CROSS_Y) {
				xSemaphoreGive(xSemaphore[1]);
			} else if (pos.y == THIRD_CROSS_Y) {
				xSemaphoreGive(xSemaphore[2]);
			}
			break;
		case SECOND_CROSS_X:
			if (pos.y == FIRST_CROSS_Y) {
				xSemaphoreGive(xSemaphore[3]);
			} else if (pos.y == SECOND_CROSS_Y) {
				xSemaphoreGive(xSemaphore[4]);
			} else if (pos.y == THIRD_CROSS_Y) {
				xSemaphoreGive(xSemaphore[5]);
			}
			break;
		case THIRD_CROSS_X:
			if (pos.y == FIRST_CROSS_Y) {
				xSemaphoreGive(xSemaphore[6]);
			} else if (pos.y == SECOND_CROSS_Y) {
				xSemaphoreGive(xSemaphore[7]);
			} else if (pos.y == THIRD_CROSS_Y) {
				xSemaphoreGive(xSemaphore[8]);
			}
			break;
		default:
			// no effect
			break;
		}
		prev_pos = pos;

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
			if (pos.x > LEFT) {
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
			if (pos.y > BOTTOM) {
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
	}
}

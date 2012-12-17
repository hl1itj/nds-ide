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

#define CROSS_NUM 9
#define WAIT_TIME 500

#define COLOR_RED       RGB(31,  0,  0)
#define COLOR_GREEN     RGB(0,  31,  0)
#define COLOR_BLACK     RGB(0,   0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)

#define DIRECTION_UP		1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3
#define DIRECTION_RIGHT		4

#define NUM_ROW		3
#define NUM_COL		3
#define NUM_TASK	(NUM_ROW + NUM_COL)

#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT 192

struct parameters {
	char *taskname;		// Task Name
	int direction;		// Current Moving Direction
	int basePoint;		// Starting Position
	u32 color;			// Ball Color
	int delay;			// Task Delay
};

struct parameters Param[NUM_TASK] = {
		{ "1", DIRECTION_RIGHT, 3, COLOR_RED, 350 }, { "2", DIRECTION_RIGHT, 6,
				COLOR_RED, 300 }, { "3", DIRECTION_RIGHT, 9, COLOR_RED, 320 }, {
				"4", DIRECTION_DOWN, 4, COLOR_GREEN, 370 }, { "5",
				DIRECTION_DOWN, 8, COLOR_GREEN, 400 }, { "6", DIRECTION_DOWN,
				12, COLOR_GREEN, 330 } };

static portTASK_FUNCTION(Ball_Task, pvParameters);
void InitDebug(void);
void vStartExpTasks(void);
void draw_my_box(int pos_x, int pos_y, u16 color);

xSemaphoreHandle xSemaphore[CROSS_NUM];

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

	for (i = 0, p = Param; i < NUM_TASK; i++, p++)
		xTaskCreate(Ball_Task, (const signed char *)(p->taskname), 1024,
				(void *)p, tskIDLE_PRIORITY + 5, NULL);

	// ���⿡ �ʿ��� �� ��ŭ Semaphore �ʱ�ȭ (vSemaphoreCreateBinary) <--------
	for (i = 0; i < CROSS_NUM; i++) {

		vSemaphoreCreateBinary(xSemaphore[i]);

	}

}

static portTASK_FUNCTION(Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	int x, y, prevX, prevY;

	// ���⿡ ��  Ball�� �ʱ� ��ġ (x,y) ���� <-------- ���� ���� ����
	int direct = p->direction;

	if (direct == DIRECTION_DOWN) {

		x = p->basePoint;
		y = 0;

	}

	else if (direct == DIRECTION_RIGHT) {

		x = 0;
		y = p->basePoint;

	}

	prevX = x;
	prevY = y;

	while (1) {

		// ���⿡ �� Ball�� �������� �ְ�, R Key�� ���ȴٸ�  Semaphore take <--------

		if (NDS_SWITCH() & KEY_R) {

			if (x == 4) {

				if (y == 3) {

					xSemaphoreTake(xSemaphore[0], WAIT_TIME);
				}

				if (y == 6) {

					xSemaphoreTake(xSemaphore[1], WAIT_TIME);
				}

				if (y == 9) {

					xSemaphoreTake(xSemaphore[2], WAIT_TIME);
				}

			}

			if (x == 8) {

				if (y == 3) {

					xSemaphoreTake(xSemaphore[3], WAIT_TIME);
				}

				if (y == 6) {

					xSemaphoreTake(xSemaphore[4], WAIT_TIME);
				}

				if (y == 9) {

					xSemaphoreTake(xSemaphore[5], WAIT_TIME);
				}

			}

			if (x == 12) {

				if (y == 3) {

					xSemaphoreTake(xSemaphore[6], WAIT_TIME);
				}

				if (y == 6) {

					xSemaphoreTake(xSemaphore[7], WAIT_TIME);
				}

				if (y == 9) {

					xSemaphoreTake(xSemaphore[8], WAIT_TIME);
				}

			}
		}

		draw_my_box(prevX, prevY, COLOR_BLACK);	// ���� ��ġ ����
		draw_my_box(x, y, p->color);			// �� ��ġ�� �׸�
		vTaskDelay(MSEC2TICK(p->delay) );		// Delay

		// ���⿡ �� Ball�� �������� ������, Semaphore Give <--------
		if (x == 4) {

			if (y == 3) {

				xSemaphoreGive(xSemaphore[0]);
			}

			if (y == 6) {

				xSemaphoreGive(xSemaphore[1]);
			}

			if (y == 9) {

				xSemaphoreGive(xSemaphore[2]);
			}

		}

		if (x == 8) {

			if (y == 3) {

				xSemaphoreGive(xSemaphore[3]);
			}

			if (y == 6) {

				xSemaphoreGive(xSemaphore[4]);
			}

			if (y == 9) {

				xSemaphoreGive(xSemaphore[5]);
			}

		}

		if (x == 12) {

			if (y == 3) {

				xSemaphoreGive(xSemaphore[6]);
			}

			if (y == 6) {

				xSemaphoreGive(xSemaphore[7]);
			}

			if (y == 9) {

				xSemaphoreGive(xSemaphore[8]);
			}

		}

		prevX = x;
		prevY = y;

		// ���� ��ġ ���  <----- ���� ���� ����
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

	}
}


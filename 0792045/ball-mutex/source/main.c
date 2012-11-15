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

#define DIRECTION_UP		1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3
#define DIRECTION_RIGHT		4

#define NUM_ROW		3
#define NUM_COL		3
#define NUM_TASK	(NUM_ROW + NUM_COL)

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

}
xSemaphoreHandle xSemaphore1, xSemaphore2, xSemaphore3, xSemaphore4,
		xSemaphore5, xSemaphore6, xSemaphore7, xSemaphore8, xSemaphore9;

static portTASK_FUNCTION(Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	int x, y, prevX, prevY;

	vSemaphoreCreateBinary(xSemaphore1);
	vSemaphoreCreateBinary(xSemaphore2);
	vSemaphoreCreateBinary(xSemaphore3);
	vSemaphoreCreateBinary(xSemaphore4);
	vSemaphoreCreateBinary(xSemaphore5);
	vSemaphoreCreateBinary(xSemaphore6);
	vSemaphoreCreateBinary(xSemaphore7);
	vSemaphoreCreateBinary(xSemaphore8);
	vSemaphoreCreateBinary(xSemaphore9);

	if (p->taskname == "1" || p->taskname == "2" || p->taskname == "3") {
		x = 0;
		y = p->basePoint;
	} else if (p->taskname == "4" || p->taskname == "5" || p->taskname == "6") {
		x = p->basePoint;
		y = 0;
	}

	while (1) {

		if (NDS_SWITCH() & KEY_R) {
			if ((x == 4 && y == 3))
				xSemaphoreTake(xSemaphore1, (portTickType)2000);
			else if ((x == 4 && y == 6))
				xSemaphoreTake(xSemaphore2, (portTickType)2000);
			else if ((x == 4 && y == 9))
				xSemaphoreTake(xSemaphore3, (portTickType)2000);
			else if ((x == 8 && y == 3))
				xSemaphoreTake(xSemaphore4, (portTickType)2000);
			else if ((x == 8 && y == 6))
				xSemaphoreTake(xSemaphore5, (portTickType)2000);
			else if ((x == 8 && y == 9))
				xSemaphoreTake(xSemaphore6, (portTickType)2000);
			else if ((x == 12 && y == 3))
				xSemaphoreTake(xSemaphore7, (portTickType)2000);
			else if ((x == 12 && y == 6))
				xSemaphoreTake(xSemaphore8, (portTickType)2000);
			else if ((x == 12 && y == 9))
				xSemaphoreTake(xSemaphore9, (portTickType)2000);
		}

		draw_my_box(prevX, prevY, COLOR_BLACK);
		draw_my_box(x, y, p->color);
		vTaskDelay(MSEC2TICK(p->delay) );

		prevX = x;
		prevY = y;

		if ((prevX == 4 && prevY == 3))
			xSemaphoreGive(xSemaphore1);
		else if ((prevX == 4 && prevY == 6))
			xSemaphoreGive(xSemaphore2);
		else if ((prevX == 4 && prevY == 9))
			xSemaphoreGive(xSemaphore3);
		else if ((prevX == 8 && prevY == 3))
			xSemaphoreGive(xSemaphore4);
		else if ((prevX == 8 && prevY == 6))
			xSemaphoreGive(xSemaphore5);
		else if ((prevX == 8 && prevY == 9))
			xSemaphoreGive(xSemaphore6);
		else if ((prevX == 12 && prevY == 3))
			xSemaphoreGive(xSemaphore7);
		else if ((prevX == 12 && prevY == 6))
			xSemaphoreGive(xSemaphore8);
		else if ((prevX == 12 && prevY == 9))
			xSemaphoreGive(xSemaphore9);

		if (p->taskname == "1" || p->taskname == "2" || p->taskname == "3") {
			if (p->direction == DIRECTION_RIGHT) {
				x++;
			} else if (p->direction == DIRECTION_LEFT) {
				x--;
			}
			if (p->direction == DIRECTION_RIGHT && x == 15) {
				p->direction = DIRECTION_LEFT;
			}
			if (p->direction == DIRECTION_LEFT && x == 0) {
				p->direction = DIRECTION_RIGHT;
			}
		} else if (p->taskname == "4" || p->taskname == "5"
				|| p->taskname == "6") {
			if (p->direction == DIRECTION_DOWN) {
				y++;
			} else if (p->direction == DIRECTION_UP) {
				y--;
			}
			if (p->direction == DIRECTION_DOWN && y == 11) {
				p->direction = DIRECTION_UP;
			}
			if (p->direction == DIRECTION_UP && y == 0) {
				p->direction = DIRECTION_DOWN;
			}
		}
	}
}
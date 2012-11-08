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
#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_BLACK     RGB( 0,  0,  0)
#define COLOR_GREEN     RGB( 0,  31,  0)

#define DIRECTION_LEFT  1
#define DIRECTION_RIGHT 2
#define DIRECTION_UP	   3
#define DIRECTION_DOWN  4

#define NUM_TASK 6

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define SCREEN_WIDTH	256
#define SCREEN_HEIGHT 192

#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)
#define BG_GFX			((u16*)0x6000000)

static portTASK_FUNCTION(Ball_Task, pvParameters);
void InitDebug(void);
void Exp_Sample(void);
void draw_my_box(int pos_x, int pos_y, u16 color);

int main(void) {
	InitDebug();
	init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()

	//xTaskCreate(Exp_Task, (const signed char * const)"Exp_Task", 2048, (void *)NULL, tskIDLE_PRIORITY + 9, NULL);

	Exp_Sample();
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
struct parameters {
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

struct parameters Param[NUM_TASK] = {
		{ "1", DIRECTION_RIGHT, 3, COLOR_RED, 50 }, { "2", DIRECTION_RIGHT, 6,
				COLOR_RED, 10 }, { "3", DIRECTION_RIGHT, 9, COLOR_RED, 100 }, {
				"4", DIRECTION_DOWN, 4, COLOR_GREEN, 20 }, { "5",
				DIRECTION_DOWN, 8, COLOR_GREEN, 70 }, { "6", DIRECTION_DOWN, 10,
				COLOR_GREEN, 150 } };

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

void Exp_Sample(void) {
	int i;
	struct parameters *p;

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	for (i = 0, p = Param; i < NUM_TASK; i++, p++) {
		xTaskCreate(Ball_Task, (const signed char *)(p->taskname), 1024,
				(void *)p, tskIDLE_PRIORITY + 5, NULL);

	}
}
static portTASK_FUNCTION( Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	int x, y;

	x = y = p->basePoint;

	while (1) {
		draw_my_box(x, y, p->color);

		vTaskDelay(MSEC2TICK(300) );

		draw_my_box(x, y, COLOR_BLACK);

		switch (p->direction) {
		case DIRECTION_RIGHT:
			x++;
			if (x == MAX_X - 1)
				p->direction = DIRECTION_LEFT;
			break;
		case DIRECTION_LEFT:
			x--;
			if (x == 0)
				p->direction = DIRECTION_RIGHT;
			break;
		case DIRECTION_UP:
			y--;
			if (y == 0)
				p->direction = DIRECTION_DOWN;
			break;
		case DIRECTION_DOWN:
			y++;
			if (y == MAX_Y - 1)
				p->direction = DIRECTION_UP;
			break;
		}
		vTaskDelay(MSEC2TICK(p->delay) );
	}
}


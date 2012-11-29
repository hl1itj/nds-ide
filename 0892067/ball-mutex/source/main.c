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

#define BG_GFX			((u16*)0x6000000)		/**< \brief background graphics memory*/
/** \brief  Screen height in pixels */
#define SCREEN_HEIGHT 192
/** \brief  Screen width in pixels */
#define SCREEN_WIDTH  256

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

static portTASK_FUNCTION( Ball_Task, pvParameters);
void InitDebug(void);
void vStartExpTasks(void);
void draw_my_box(int pos_x, int pos_y, u16 color);

// ���⿡ �ʿ��� �� ��ŭ�� Semaphore �ڵ� ����  <--------
xSemaphoreHandle xSemaphore[3][3];
s
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
	int i, j;

	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	for (i = 0, p = Param; i < NUM_TASK; i++, p++)
		xTaskCreate(Ball_Task, (const signed char *) (p->taskname), 1024,
				(void *) p, tskIDLE_PRIORITY + 5, NULL);

	// ���⿡ �ʿ��� �� ��ŭ Semaphore �ʱ�ȭ (vSemaphoreCreateBinary) <--------
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			vSemaphoreCreateBinary(xSemaphore[i][j]);

}

static portTASK_FUNCTION( Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	int x, y, before_X, before_Y;
	int current_X, current_Y;
	int state = 0;
	int flag = 0;
	// ���⿡ ��  Ball�� �ʱ� ��ġ (x,y) ���� <-------- ���� ���� ����

	before_X = -1;
	before_Y = -1;
	x = 0;
	y = 0;

	if (p->direction == DIRECTION_DOWN) {
		x = p->basePoint;
	} else if (p->direction == DIRECTION_RIGHT) {
		y = p->basePoint;
	}
	current_X = 0;
	current_Y = 0;

	while (1) {

		// ���⿡ �� Ball�� �������� �ְ�, R Key�� ���ȴٸ�  Semaphore take <--------

	if(x == 4)
	current_X = 0;
	else if(x == 8)
	current_X = 1;
	else if (x == 12)

	if(y == 3)
	current_Y = 0;
	else if(y == 6)
	current_Y = 1;
	else if(y ==9))
	current_Y = 2;
	else
	current_Y = -1;

	if ((current_Y >= 0) && (current_X >= 0) {
		if(NDS_SWITCH() && KEY_R) {
			flag = xSemaphoreTake(xSemaphore[current_X][current_Y],
					portMAX_DELAY);
		}
	}
	draw_my_box(before_X, before_Y, COLOR_BLACK);
	draw_my_box(x, y, p->color);
	vTaskDelay(MSEC2TICK(p->delay));		// Delay

	// ���⿡ �� Ball�� �������� ������, Semaphore Give <--------
	if (flag) {
		xSemaphoreGive(xSemaphore[current_X][current_Y]);
		flag = 0;
	}

	// ���� ��ġ ���  <----- ���� ���� ����
	before_X = x;
	before_Y = y;
	if (p->direction == DIRECTION_DOWN) {
		if state = y == (MAX_Y - 1) ? 1 : 0;

		if y+= state == 0 ? 1 : -1;

	} else if (p->direction == DIRECTION_RIGHT) {
		if x+= state == 0 ? 1 : -1;

		if state= x == (MAX_X - 1) ? 1 : 0;

	} else
		//do noting
}
}

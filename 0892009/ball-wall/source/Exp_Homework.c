// Free RTOS Headers
// See API reference Document in http://www.freertos.org/

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
#define COLOR_BLACK     RGB( 0,  0,  0)	/* Black : Zero	*/

#define BALL_WIDTH	8
#define BALL_HEIGHT	8
#define BALL_Y_POS	13
#define BALL_X_MAX	(SCREEN_WIDTH / BALL_WIDTH) // 256/8 = 32
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define WALL_WIDTH 32
#define WALL_HEIGHT 32 // MAX=8
#define WALL_Y_POS 4
#define WALL_X_MAX 7

#define BG_GFX ((u16*)0x6000000)

void draw_my_box(int pos_x, int pos_y, u16 color) {
	int i, j;
	u32 *basePoint, pixel;

	pixel = (color << 16) + color;
	for (i = 0; i < BALL_HEIGHT; i++) {
		basePoint = (u32 *) BG_GFX
				+ ((((pos_y * BALL_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * BALL_WIDTH) / 2;
		for (j = 0; j < (BALL_WIDTH / 2); j++)
			*basePoint++ = pixel;
	}
}

void draw_my_wall(int pos_x, int pos_y, u16 color) {
	int i, j;
	u32 *basePoint, pixel;
	pixel = (color << 16) + color;
	for (i = 0; i < WALL_HEIGHT; i++) {
		basePoint = (u32 *) BG_GFX
				+ ((((pos_y * WALL_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * WALL_WIDTH) / 2;
		for (j = 0; j < (WALL_WIDTH / 2); j++)
			*basePoint++ = pixel;
	}
}

extern xTaskHandle BallTask;
int wall = 0;
xSemaphoreHandle xSemaphore[WALL_X_MAX];

void Exp_8_Homework_A(void) {
	vTaskResume(BallTask);

	while (1) {



		if (NDS_SWITCH() & KEY_START) break;
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

void Exp_8_Homework_B(void) {
	int i;
	u8 key, prev_key = -1;
	key_init();
	vTaskResume(BallTask);
	for (i = 0; i < 7; i++) {
		vSemaphoreCreateBinary(xSemaphore[i]);
	}
	while (1) {
		if (kbhit()) {
			prev_key = key;
			key = getkey();
			if (key > WALL_X_MAX) continue;
			for (i = 0; i < prev_key; i++) {
				draw_my_wall(i, WALL_Y_POS, COLOR_BLACK);
				xSemaphoreGive(xSemaphore[i]);
			}
			wall = 0;
			for (i = 0; i < key; i++) {
				while (!xSemaphoreTake(xSemaphore[i], 0))
					;
				draw_my_wall(i, WALL_Y_POS, COLOR_WHITE);
				wall++;
			}
		}

		if (NDS_SWITCH() & KEY_START) break;
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

portTASK_FUNCTION(Ball_Task, pvParameters) {
	int x;
	x = BALL_X_MAX - 1;

	int direction = FALSE; // TRUE is right
	while (1) {
		draw_my_box(x, BALL_Y_POS, COLOR_RED);
		vTaskDelay(MSEC2TICK(1000 / (BALL_X_MAX - wall*4)) );
		draw_my_box(x, BALL_Y_POS, COLOR_BLACK);
		if (!direction) {
			x--;
			if (x % 4 == 0) {
				if (x == 0 || !xSemaphoreTake(xSemaphore[(x/4) - 1], 0)) {
					direction = TRUE;
				}
			}

		} else {
			if (x % 4 == 0) {
				xSemaphoreGive(xSemaphore[(x/4) - 1]);
			}
			x++;
			if (x == BALL_X_MAX - 1) {
				direction = FALSE;
			}
		}
	}
}

// Key Matrix Scanning Task

void key_init(void) {
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int kbhit(void) {
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8 getkey(void) {
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	return key;
}

portTASK_FUNCTION(Key_Task, pvParameters) {
	u8 key, scan = 0;
	u8 pressed = FALSE;
	while (1) {
		if (!pressed) {
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			switch (readb_virtual_io(KEY_MATRIX)) {
			case 8:
				key += 1;
				break;
			case 4:
				key += 2;
				break;
			case 2:
				key += 3;
				break;
			case 1:
				key += 4;
				if (key == 16) key = 0;
				break;
			default:
				key = 255;
				break;
			}
			scan++;
			if (scan == 4) scan = 0;
			if (key < 16) {
				xQueueSend(KeyQueue, &key, 0);
				pressed = TRUE;
			}
			vTaskDelay(MSEC2TICK(1) );
		}
		if (pressed && readb_virtual_io(KEY_MATRIX) == 0) {
			vTaskDelay(MSEC2TICK(1) );
			pressed = FALSE;
		}
		vTaskDelay(MSEC2TICK(1) );
	}
}
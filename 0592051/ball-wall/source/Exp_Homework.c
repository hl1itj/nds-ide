// Free RTOS Headers
// See API reference Document in http://www.freertos.org/

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

// NDS / Sevencore Board Headers
#include <nds.h>
#include <sevencore_io.h>

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define COLOR_RED       RGB(31,  0,  0) /* Bright Red  	*/
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)	/* Black : Zero	*/

#define BOX_WIDTH 8
#define BOX_HEIGHT 8
#define BOX_Y_POS 13
#define BOX_X_MAX	(SCREEN_WIDTH / BOX_WIDTH)

#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256

#define WALL_WIDTH 32
#define WALL_HEIGHT 32
#define WALL_Y_POS 2
#define WALL_X_MAX (SCREEN_WIDTH / WALL_WIDTH)

#define ARRAY_SIZE 7

// �ʿ��� global ����

u8 wall;
xSemaphoreHandle xSemaphore[ARRAY_SIZE];

void
draw_my_box(int pos_x, int pos_y, u16 color)
{
	int i, j;
    u32 *basePoint, pixel;

    pixel = (color << 16) + color;
    for (i = 0; i < BOX_HEIGHT; i++) {
 	    basePoint = (u32 *)BG_GFX +
 	      ((((pos_y * BOX_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * BOX_WIDTH) / 2;
    	for (j = 0; j < (BOX_WIDTH / 2); j++)
    		*basePoint++ = pixel;
	}
}

void
draw_my_wall(int pos_x, int pos_y, u16 color)
{
	int i, j;
	    u32 *basePoint, pixel;

	    pixel = (color << 16) + color;
	    for (i = 0; i < WALL_HEIGHT; i++) {
	 	    basePoint = (u32 *)BG_GFX +
	 	      ((((pos_y * WALL_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * WALL_WIDTH) / 2;
	    	for (j = 0; j < (WALL_WIDTH / 2); j++)
	    		*basePoint++ = pixel;
		}
}

extern xTaskHandle BallTask;

void
Exp_8_Homework_A(void)
{
	u8 key, x, prevX;
	int i;

    while (1) {
		if (kbhit() == 1) {
			key = getkey();

			if(key < WALL_X_MAX){
				prevX = x;
				x = key;
				draw_my_wall(prevX, WALL_Y_POS, COLOR_BLACK);
				draw_my_wall(x, WALL_Y_POS, COLOR_WHITE);
			}

			if(key == 15){
				for(i = 0; i < WALL_X_MAX; i++){
					draw_my_wall(i, WALL_Y_POS, COLOR_BLACK);
				}
			}
		}

		if (NDS_SWITCH() & KEY_START){
			draw_my_wall(x, WALL_Y_POS, COLOR_BLACK);
			break;
		}
		vTaskDelay(20);
    }
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);		// Wait while START KEY is being pressed
}

void
Exp_8_Homework_B(void)
{
	u8 key, x, prevX;
	int i;

	for(i=0; i<ARRAY_SIZE; i++){
		vSemaphoreCreateBinary(xSemaphore[i]);
	}

	vTaskResume(BallTask);

	while (1) {

		key = getkey();

		if (key < WALL_X_MAX) {
			prevX = x;
			x = key;

			for(i=0; i<prevX; i++){
				draw_my_wall(i, WALL_Y_POS, COLOR_BLACK);
			}

			for(i=0; i<x; i++){
				draw_my_wall(i, WALL_Y_POS, COLOR_WHITE);
			}
		}

		if (key == 15) {
			for (i = 0; i < WALL_X_MAX; i++) {
				draw_my_wall(i, WALL_Y_POS, COLOR_BLACK);
			}
		}

		if (NDS_SWITCH() & KEY_START) {
			draw_my_wall(x, WALL_Y_POS, COLOR_BLACK);
			break;
		}
		vTaskDelay(20);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

portTASK_FUNCTION(Ball_Task, pvParameters)
{
	u8 prevX, x;
	int i=28;
	u8 LEFT = TRUE;

	draw_my_box(31, 10, COLOR_WHITE);

	while (1) {
		prevX = x;
		x = i;

		draw_my_box(prevX, BOX_Y_POS, COLOR_BLACK);
		draw_my_box(x, BOX_Y_POS, COLOR_RED);

		if(LEFT){
			if(i==0){
				LEFT = FALSE;
			}else{
				i--;
			}
		}else{
			if(i==31){
				LEFT = TRUE;
			}else{
				i++;
			}
		}

		vTaskDelay(20);
	}
}

// Key Matrix Scanning Task

void
key_init(void)
{
	int i;
	u8 key;

	for (i = 0; i < MAX_KEY_LOG; i++)
		xQueueReceive(KeyQueue, &key, 0);
}

int
kbhit(void)
{
	u8 key;
	int ret = xQueuePeek(KeyQueue, &key, 0);
	return (ret == pdPASS);
}
u8
getkey(void)
{
	u8 key;
	xQueueReceive(KeyQueue, &key, portMAX_DELAY);
	return key;
}

portTASK_FUNCTION(Key_Task, pvParameters)
{
	u8 key, select, scan = 0;
		u8 key_On = TRUE;

		while (1) {
			if (key_On) {
				writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
				key = scan * 4;
				select = readb_virtual_io(KEY_MATRIX);
				switch (select) {
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
					if (key == 16)
						key = 0;
					break;
				default:
					key = 255;
					break;
				}

				scan++;

				if (scan == 4) {
					scan = 0;
				}

				if (key < 16) {
					xQueueSend(KeyQueue, &key, 0);
					key_On = FALSE;
				}
			} else {
				if ((key_On == FALSE) && (readb_virtual_io(KEY_MATRIX) != select)) {
					key_On = TRUE;
				}
			}
			vTaskDelay(MSEC2TICK(20) );
		}
}

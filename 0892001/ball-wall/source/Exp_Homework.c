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

#define BOX_WIDTH	8
#define BOX_HEIGHT	8
#define W_BOX_WIDTH 32
#define W_BOX_HEIGHT 32
#define BOX_Y_POS	13
#define BOX_X_MAX	(SCREEN_WIDTH / BOX_WIDTH)

#define RIGHT 0
#define LEFT 1

void key_init(void);
int kbhit(void);
u8 getkey(void);

int wall_location=0;
xSemaphoreHandle xSemaphore[8];

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
	for (i = 0; i < W_BOX_HEIGHT; i++) {
		basePoint = (u32 *)BG_GFX +
				((((pos_y * W_BOX_HEIGHT) + i) * SCREEN_WIDTH) + pos_x * W_BOX_WIDTH) / 2;
		for (j = 0; j < (W_BOX_WIDTH / 2); j++)
			*basePoint++ = pixel;
	}
}

extern xTaskHandle BallTask;

void
Exp_8_Homework_A(void)
{
	u8 key,prevkey;
    while (1) {
    	if(kbhit()){
    		key=getkey();
    		if(key<8){
    			draw_my_wall(prevkey,3,COLOR_BLACK);
    			draw_my_wall(key,3,COLOR_WHITE);
    			prevkey=key;
    		}
    	}
    	if(NDS_SWITCH() & KEY_START){
    		draw_my_wall(prevkey,3,COLOR_BLACK);
    	    break;
    	}
    }
}

void
Exp_8_Homework_B(void)
{
	u8 key;
	int i;
	for(i=0; i<8; i++)
	    	vSemaphoreCreateBinary(xSemaphore[i]);
    vTaskResume(BallTask);

	while (1) {
		if(kbhit()){
			key = getkey();
			if(key<8){
				if(key==0)
					wall_location=0;
				for(i=0; i<key; i++){
					while(!xSemaphoreTake(xSemaphore[i],0));
					draw_my_wall(i,3,COLOR_WHITE);
					wall_location=(i+1)*4;
				}
				for(i=key; i<8; i++){
					draw_my_wall(i,3,COLOR_BLACK);
				}
				for(i=0; i<8; i++)
					xSemaphoreGive(xSemaphore[i]);
			}
		}
	}
}

portTASK_FUNCTION(Ball_Task, pvParameters)
{
	int direction=LEFT;
	int x=BOX_X_MAX-1;
	int y=BOX_Y_POS;
	int prevCArea, CArea;
	portTickType xLastWakeTime =xTaskGetTickCount();

	while (1) {
		CArea=(int)(x/4);
		xSemaphoreTake(xSemaphore[CArea],0);

		draw_my_box(x,y,COLOR_RED);
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(1000/(BOX_X_MAX-wall_location)));
		draw_my_box(x,y,COLOR_BLACK);

		if(CArea!=prevCArea)
			xSemaphoreGive(xSemaphore[prevCArea]);

		if(direction==LEFT && x==wall_location){
			direction=RIGHT;
			x++;
		}
		else if(direction==RIGHT && x==BOX_X_MAX-1){
			direction=LEFT;
			x--;
		}
		else if(direction==LEFT)
			x--;
		else if(direction==RIGHT)
			x++;
		prevCArea=CArea;
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
	u8 key, scan = 0;
	int key_pressed=0;

	while (1) {
		if(key_pressed==0){
			writeb_virtual_io(KEY_MATRIX, 0x80 >> scan);
			key = scan * 4;
			switch (readb_virtual_io(KEY_MATRIX)) {
			case 8 : key += 1; break;
			case 4 : key += 2; break;
			case 2 : key += 3; break;
			case 1 : key += 4; if (key == 16) key = 0; break;
			default : key = 255; break;
			}
			scan++;
			if (scan == 4)
				scan = 0;
			if (key < 16){
				key_pressed=1;
				xQueueSend(KeyQueue, &key, 0);
			}

		}
		if(readb_virtual_io(KEY_MATRIX)==0)
			key_pressed=0;

		vTaskDelay(MSEC2TICK(10));
	}
}

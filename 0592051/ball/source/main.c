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
#define COLOR_GREEN     RGB( 0, 31,  0) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)

#define BOX_WIDTH	16
#define BOX_HEIGHT	16
#define MAX_X		(SCREEN_WIDTH / BOX_WIDTH)
#define MAX_Y		(SCREEN_HEIGHT / BOX_HEIGHT)


#define BG_GFX			((u16*)0x6000000)
#define SCREEN_WIDTH	256

#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT 2
#define DIRECTION_UP 3
#define DIRECTION_DOWN 4

#define NUM_TASK 6

static portTASK_FUNCTION(Ball_Task, pvParameters);
//static portTASK_FUNCTION(Exp_Task, pvParameters);
portTASK_FUNCTION(Key_Task, pvParameters);
//void Exp_Sample(void);
void InitDebug(void);

struct parameters{
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int deley;
};

struct parameters Param[NUM_TASK] = {
		{"1", DIRECTION_RIGHT, 3, COLOR_RED, 50},
		{"2", DIRECTION_RIGHT, 6, COLOR_RED, 10},
		{"3", DIRECTION_RIGHT, 9, COLOR_RED, 100},
		{"4", DIRECTION_DOWN, 4, COLOR_GREEN, 20},
		{"5", DIRECTION_DOWN, 8, COLOR_GREEN, 70},
		{"6", DIRECTION_DOWN, 12, COLOR_GREEN, 150}
};

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

int
main(void)
{
	int i;
	struct parameters *p;

	InitDebug();
	init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()

	xTaskCreate(Key_Task,
					     (const signed char * const)"Key_Task",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 10,
					     NULL);
	/*
	xTaskCreate(Exp_Task,
					     (const signed char * const)"Exp_Task",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 9,
					     NULL);
	 */
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);

	for (i = 0, p = Param; i < NUM_TASK; i++, p++) {
		xTaskCreate(Ball_Task, (const signed char *) (p->taskname), 1024,
				(void *)p, tskIDLE_PRIORITY + 5, NULL);
	}

	KeyQueue = xQueueCreate(MAX_KEY_LOG, sizeof(u8));

	vTaskStartScheduler();
	while(1)
		;
	return 0;
}

void
InitDebug(void)
{
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}
/*
static
portTASK_FUNCTION(Exp_Task, pvParameters)
{

	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);

    while(1){
    	Exp_Sample();
    }
}*/
// task add

portTASK_FUNCTION(Key_Task, pvParameters) {

	u8 key, scan = 0;
	u8 key_pressed = FALSE;
	key_init();

	while (1) {

		if (key_pressed == FALSE) {

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
				if (key == 16)
					key = 0;
				break;
			default:
				key = 255;
				break;
			}
			scan++;
			if (scan == 4)
				scan = 0;

			if ((key < 16)) {
				key_pressed = TRUE;
				xQueueSend(KeyQueue, &key, 0);
			}
		}

		if ((key_pressed == TRUE) && (readb_virtual_io(KEY_MATRIX) == 0))
			key_pressed = FALSE;

		vTaskDelay(MSEC2TICK(30));
	}
}

portTASK_FUNCTION(Ball_Task, pvParameters) {
	struct parameters *p = (struct parameters *) pvParameters;
	int v, h, prevV = 0, prevH = 0;
	u8 sw = FALSE;

	switch (p->direction) {
	case DIRECTION_RIGHT:
		h = p->basePoint;
		v = 1;
		break;
	case DIRECTION_DOWN:
		h = 1;
		v = p->basePoint;
		break;
	default:
		break;
	}

	while (1) {

		switch (p->direction) {
		case DIRECTION_RIGHT:
			draw_my_box(prevV, h, COLOR_BLACK);
			draw_my_box(v, h, p->color);
			vTaskDelay(MSEC2TICK(300) );
			draw_my_box(v, h, COLOR_BLACK);
			prevV = v;

			if (sw) {
				v--;
				if (v < 1) {
					sw = FALSE;
				}
			}else{
				v++;
				if(v > 14){
					sw = TRUE;
				}
			}

			break;
		case DIRECTION_DOWN:
			draw_my_box(v, prevH, COLOR_BLACK);
			draw_my_box(v, h, p->color);
			vTaskDelay(MSEC2TICK(300) );
			draw_my_box(v, h, COLOR_BLACK);
			prevH = h;
			if (sw) {
				h--;
				if (h < 1) {
					sw = FALSE;
				}

			} else {
				h++;
				if (h > 10) {
					sw = TRUE;
				}
			}
			break;
		default:
			break;

		}
		vTaskDelay(MSEC2TICK(p->deley) );
	}

}

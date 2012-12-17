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
#define COLOR_WHITE     RGB(31, 31, 31) /* Bright White */
#define COLOR_BLACK     RGB( 0,  0,  0)
#define COLOR_GREEN	   RGB( 0, 31,  0)

#define NUM_TASK 6

#define DIRECTION_RIGHT 0
#define DIRECTION_DOWN 1

//static portTASK_FUNCTION(Exp_Task, pvParameters);
static portTASK_FUNCTION(Ball_Task, pvParameters);
portTASK_FUNCTION(Key_Task, pvParameters);

struct parameters{
	char *taskname;
	int direction;
	int basePoint;
	u32 color;
	int delay;
};

struct parameters Param[NUM_TASK] = {
		{"1", DIRECTION_RIGHT, 3,  COLOR_RED, 50},
		{"2", DIRECTION_RIGHT, 6,  COLOR_RED, 10},
		{"3", DIRECTION_RIGHT, 9,  COLOR_RED, 100},
		{"4", DIRECTION_DOWN,  4,  COLOR_GREEN, 20},
		{"5", DIRECTION_DOWN,  8,  COLOR_GREEN, 70},
		{"6", DIRECTION_DOWN,  12, COLOR_GREEN, 150},
};
//void Exp_Sample(void);
void EXp_Homework(struct parameters *p);

void InitDebug(void);

int
main(void)
{
	int i;
	struct parameters * p;

	InitDebug();
	init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()




	for(i = 0, p = Param; i<NUM_TASK; i++, p++){
		xTaskCreate(Ball_Task,
				(const signed char  *)(p->taskname),
				1024,
				(void *)p,
				tskIDLE_PRIORITY + 5 ,
				NULL);
	}

	xTaskCreate(Key_Task,
					     (const signed char * const)"Key_Task",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 10,
					     NULL);
/*	xTaskCreate(Exp_Task,
					     (const signed char * const)"Exp_Task",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 9,
					     NULL);
*/
	KeyQueue = xQueueCreate(MAX_KEY_LOG, sizeof(u8));
	// Error Processing Needed !

	vTaskStartScheduler();		// Never returns
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

	while (1) {
		Exp_Sample();
	}
}
*/

static
portTASK_FUNCTION(Ball_Task, pvParameters)
{
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	struct parameters *p = (struct parameters *)pvParameters;
	while (1) {
		Exp_Homework(p);
	}
}

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

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

static portTASK_FUNCTION(Exp_Task, pvParameters);


void Exp_Sample(void);


void InitDebug(void);



int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()

	xTaskCreate(Exp_Task,
					     (const signed char * const)"Exp_Task",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 9,
					     NULL);

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

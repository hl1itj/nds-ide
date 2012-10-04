/*
 * main.c
 *
 *  Created on: 2011. 9. 24.
 *      Author: Minsuk Lee
 */

#include <stdio.h>          // C-Standard Header
#include <time.h>
#include <stdlib.h>

#include <FreeRTOS.h>       // Free RTOS Headers
#include <task.h>
#include <queue.h>
#include <semphr.h>

#include <nds.h>            // NDS / Sevencore Board Headers
#include <sevencore_io.h>

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Homework_1,
					     (const signed char * const)"Homework_1",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
					     NULL);
	xTaskCreate(Homework_2,
					     (const signed char * const)"Homework_2",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);
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
portTASK_FUNCTION(Homework_1, pvParameters)
{
	u16 sw, num = 0x08;
	u8 key_pressed = FALSE;

	writeb_virtual_io(BARLED1, num);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (key_pressed == FALSE && (sw & KEY_L) && num != 0x80) {
			key_pressed = TRUE;
			num = num << 1;
			writeb_virtual_io(BARLED1, num);
		}
		if (key_pressed == FALSE && (sw & KEY_R) && num != 0x01) {
			key_pressed = TRUE;
			num = num >> 1;
			writeb_virtual_io(BARLED1, num);
		}
		if ((key_pressed == TRUE) && (!(sw & KEY_L)) && (!(sw & KEY_R)))
			key_pressed = FALSE;


		vTaskDelay(50);
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(10);
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{
	u16 num=0x01;
	u8 state = TRUE;
	portTickType xLastWakeTime=xTaskGetTickCount();

	while (1) {
		if(num==0x80){
			state= FALSE;
			num=0x01;
		}
		else if(num==0x01)
			state=TRUE;

		if(state)
			num=num<<1;

		writeb_virtual_io(BARLED2, num);

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}
}

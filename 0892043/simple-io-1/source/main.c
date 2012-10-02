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

#include "card_spi.h"		// Debugging Headers
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Exp_1_Task, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED);	// Enable Virtual IO Devices
	init_printf();					// Initialize Bottom Screen for printf()

	xTaskCreate(Exp_1_Task,
			(const signed char * const)"Exp_1_Task",
			2048,
			(void *)NULL,
			tskIDLE_PRIORITY + 1,
			NULL);

	vTaskStartScheduler();		// Never returns
	while(1)
		;
	return 0;
}

void Exp_1_Sample_A(void);
void Exp_1_Sample_B(void);
void Exp_1_Homework_A(void);
void Exp_1_Homework_B(void);

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
portTASK_FUNCTION(Exp_1_Task, pvParameters )
{
	while (1) {
		//Exp_1_Sample_A();
		//Exp_1_Sample_B();

		Exp_1_Homework_A();
		Exp_1_Homework_B();
	}
}



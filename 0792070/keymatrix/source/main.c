#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"

static portTASK_FUNCTION(Exp_4_Task, pvParameters);

void InitDebug(void);

int
main(void)
{
	InitDebug();
	init_virtual_io(ENABLE_LED | ENABLE_SW | ENABLE_MATRIX);	// Enable Virtual LED's on Top Screen
	//init_printf();							// Initialize Bottom Screen for printf()

	xTaskCreate(Exp_4_Task,
				(const signed char * const)"Exp_4_Task",
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

void Exp_4_Sample_A(void);
void Exp_4_Homework_A(void);

static
portTASK_FUNCTION(Exp_4_Task, pvParameters)
{
	while (1) {
		Exp_4_Sample_A();
		Exp_4_Homework_A();
	}
}

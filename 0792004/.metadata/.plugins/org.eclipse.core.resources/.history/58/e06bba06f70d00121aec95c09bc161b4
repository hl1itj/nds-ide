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

#define MIN_LED 0x01
#define MAX_LED 0x80
#define LED1(x) writeb_virtual_io(BARLED1, x)
#define LED2(x) writeb_virtual_io(BARLED2, x)

<<<<<<< HEAD
static portTASK_FUNCTION(Sample_Task_1, pvParameters);
static portTASK_FUNCTION(Sample_Task_2, pvParameters);
=======
static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2

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
<<<<<<< HEAD
					     tskIDLE_PRIORITY + 2,
=======
					     tskIDLE_PRIORITY + 1,
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
					     NULL);
	xTaskCreate(Homework_2,
					     (const signed char * const)"Homework_2",
					     2048,
					     (void *)NULL,
<<<<<<< HEAD
					     tskIDLE_PRIORITY + 1,
=======
					     tskIDLE_PRIORITY + 2,
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
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
	u16 sw;
	u8 led = MIN_LED;
<<<<<<< HEAD
	u8 keyR_pressed = FALSE;
	u8 keyL_pressed = FALSE;

	LED1(MIN_LED);
=======
	u8 keyRight_pressed = FALSE;
	u8 keyLeft_pressed = FALSE;

	LED1(MIN_LED);
	LED2(0);
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2

	while (1) {
		sw = NDS_SWITCH();

<<<<<<< HEAD
		if((sw & KEY_R) && (keyR_pressed == FALSE)) {
			keyR_pressed = TRUE;
=======
		if((sw & KEY_R) && (keyRight_pressed == FALSE)) {
			keyRight_pressed = TRUE;
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
			if(led > MIN_LED) led/=2;
			LED1(led);
			printf("R");
		}
<<<<<<< HEAD
		else if((sw & KEY_L) && (keyL_pressed == FALSE)) {
			keyL_pressed = TRUE;
=======
		else if((sw & KEY_L) && (keyLeft_pressed == FALSE)) {
			keyLeft_pressed = TRUE;
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
			if(led < MAX_LED) led*=2;
			LED1(led);
			printf("L");
		}

<<<<<<< HEAD
		if((keyR_pressed == TRUE) && !(sw & KEY_R))
			keyR_pressed = FALSE;

		if((keyL_pressed == TRUE) && !(sw & KEY_L))
			keyL_pressed = FALSE;
=======
		if((keyRight_pressed == TRUE) && !(sw & KEY_R))
			keyRight_pressed = FALSE;

		if((keyLeft_pressed == TRUE) && !(sw & KEY_L))
			keyLeft_pressed = FALSE;

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(50);
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{
<<<<<<< HEAD
	portTickType xLastWakeTime = xTaskGetTickCount();
	u8 led = MIN_LED;

	while (1) {
		LED2(led);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
		led = led < MAX_LED ? led*2 : MIN_LED;
=======
	u8 barled = MIN_LED;
	portTickType xLastWakeTime = xTaskGetTickCount();

	while (1) {
		LED2(barled);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
		barled = (barled < MAX_LED) ? barled*2 : MIN_LED;
>>>>>>> a9e1eb13e766ec4b7389d6b84c11c3fcbd1e0ac2
	}
}

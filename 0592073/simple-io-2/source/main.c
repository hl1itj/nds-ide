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

#define OFFSET_MOVEMENT 2
#define LED_OFF 0x00
#define LED_1ST 0x01
#define LED_8TH 0x80

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
	u16 sw;
	u16 offset = LED_1ST;
	u8 L_key_pressed = FALSE;
	u8 R_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, LED_1ST);
	writeb_virtual_io(BARLED2, LED_OFF);
		while (1) {
			sw = NDS_SWITCH();
				if (((L_key_pressed == FALSE) && (sw & KEY_L))){
				L_key_pressed = TRUE;
				if(offset < LED_8TH){
					offset = offset * OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
			} else if (((R_key_pressed == FALSE) && (sw & KEY_R))){
				R_key_pressed = TRUE;
				if(offset > LED_1ST){
					offset = offset / OFFSET_MOVEMENT;
					writeb_virtual_io(BARLED1, offset);
				}
			}
				if ((L_key_pressed == TRUE) && (!(sw & KEY_L))){
				L_key_pressed = FALSE;
			} else if ((R_key_pressed == TRUE) && (!(sw & KEY_R))){
				R_key_pressed = FALSE;
			}
			if (NDS_SWITCH() & KEY_START)
				break;
			vTaskDelay(50);
		}

		while (NDS_SWITCH() & KEY_START)
			vTaskDelay(10);

		vTaskDelay(MSEC2TICK(500));
}

static
portTASK_FUNCTION(Homework_2, pvParameters)
{

	u8 barled = 0x01;
	portTickType xLastWakeTime = xTaskGetTickCount();

	writeb_virtual_io(BARLED2, barled);

	while (1) {
		if ( barled  < LED_8TH ) {
			barled = barled * OFFSET_MOVEMENT;
			writeb_virtual_io(BARLED2, barled);
		} else if ( barled == LED_8TH ) {
			barled = LED_1ST;
			writeb_virtual_io(BARLED2, barled);
		}

		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500));
	}

}

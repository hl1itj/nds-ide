<<<<<<< HEAD
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

#define LED_MIN 0x01
#define LED_MAX 0x80

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int main(void) {
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
	while (1)
		;
	return 0;
}

void InitDebug(void) {
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

static
portTASK_FUNCTION(Homework_1, pvParameters) {
	u16 sw;
	u16 led = LED_MIN;
	u8 r_key_pressed = FALSE;
	u8 l_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (((r_key_pressed == FALSE) && (sw & KEY_R))) {
			r_key_pressed = TRUE;
			printf("R");
			if (led == LED_MIN)
				writeb_virtual_io(BARLED1, LED_MIN);
			else {
				led /= 2;
				writeb_virtual_io(BARLED1, led);
			}

		}

		if ((r_key_pressed == TRUE) && (!(sw & KEY_R)))
			r_key_pressed = FALSE;

		if (((l_key_pressed == FALSE) && (sw & KEY_L))) {
			l_key_pressed = TRUE;
			printf("L");
			if (led == LED_MAX)
				writeb_virtual_io(BARLED1, LED_MAX);
			else {
				led *= 2;
				writeb_virtual_io(BARLED1, led);
			}
		}
		if ((l_key_pressed == TRUE) && (!(sw & KEY_L)))
			l_key_pressed = FALSE;

		vTaskDelay(50);
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters) {
	u8 barled = LED_MIN;
	portTickType xLastWakeTime = xTaskGetTickCount();


	while (1) {
		writeb_virtual_io(BARLED2, barled);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );
		if(barled < LED_MAX) barled *= 2;
		else barled = LED_MIN;
	}
}
=======
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

#define LED_MIN 0x01
#define LED_MAX 0x80

static portTASK_FUNCTION(Homework_1, pvParameters);
static portTASK_FUNCTION(Homework_2, pvParameters);

void InitDebug(void);

int main(void) {
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
	while (1)
		;
	return 0;
}

void InitDebug(void) {
#ifdef DEBUG
	irqInit();
	initSpi();
	initDebug();
	BreakPoint();
#endif
}

static
portTASK_FUNCTION(Homework_1, pvParameters) {
	u16 sw;
	u16 led = LED_MIN;
	u8 r_key_pressed = FALSE;
	u8 l_key_pressed = FALSE;

	writeb_virtual_io(BARLED1, led);
	writeb_virtual_io(BARLED2, 0);

	while (1) {
		sw = NDS_SWITCH();

		if (((r_key_pressed == FALSE) && (sw & KEY_R))) {
			r_key_pressed = TRUE;
			printf("R");
			if (led == LED_MIN)
				writeb_virtual_io(BARLED1, LED_MIN);
			else {
				led /= 2;
				writeb_virtual_io(BARLED1, led);
			}

		}

		if ((r_key_pressed == TRUE) && (!(sw & KEY_R)))
			r_key_pressed = FALSE;

		if (((l_key_pressed == FALSE) && (sw & KEY_L))) {
			l_key_pressed = TRUE;
			printf("L");
			if (led == LED_MAX)
				writeb_virtual_io(BARLED1, LED_MAX);
			else {
				led *= 2;
				writeb_virtual_io(BARLED1, led);
			}
		}
		if ((l_key_pressed == TRUE) && (!(sw & KEY_L)))
			l_key_pressed = FALSE;

		vTaskDelay(50);
	}
}

static
portTASK_FUNCTION(Homework_2, pvParameters) {
	u8 barled = LED_MIN;
	portTickType xLastWakeTime = xTaskGetTickCount();


	while (1) {
		writeb_virtual_io(BARLED2, barled);
		printf(".");
		vTaskDelayUntil(&xLastWakeTime, MSEC2TICK(500) );
		if(barled < LED_MAX) barled *= 2;
		else barled = LED_MIN;
	}
}
>>>>>>> 1fc079dd7be353aec78946b4dd04e80331385f73

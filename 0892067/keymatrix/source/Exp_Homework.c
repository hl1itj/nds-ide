#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "sevencore_io.h"

void
Exp_4_Homework_A(void)
{
    // Variables

	while (1) {

		// Fill Here

		if (NDS_SWITCH() & KEY_START)
			break;
		vTaskDelay(MSEC2TICK(20));
	}
	while (NDS_SWITCH() & KEY_START)
		vTaskDelay(MSEC2TICK(10));		// Wait while START KEY is being pressed
}


#include "FreeRTOS.h"
#include "task.h"
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <virtual_io.h>
#include "up.h"
#include "down.h"

#include <PA9.h>       // Include for PA_Lib

static portTASK_FUNCTION( testTask1, pvParameters );

PrintConsole bottomScreen;
touchPosition touch;
	
int main(void)
{
	init_virtual_io(ENABLE_ALL_DEVICES);

	xTaskCreate( testTask1,
					     (const signed char * const)"testTask1",
					     2048,
					     (  void * ) NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);

	vTaskStartScheduler();
	while(1);

	return 0;
}

int xx;

static portTASK_FUNCTION( testTask1, pvParameters )
{
	u8 val;

	while (1) {
		writeb_virtual_io(SEG7COM, 0x3);
		writeb_virtual_io(SEG7DATA, 0x8f);
		writeb_virtual_io(KEYMATRIX, 0x8 << 4);
		val = readb_virtual_io(KEYMATRIX);
		writeb_virtual_io(BARLED1, val);
		val = readb_virtual_io(PUSHSW);
		writeb_virtual_io(BARLED2, val);
		vTaskDelay(100);
	}
}


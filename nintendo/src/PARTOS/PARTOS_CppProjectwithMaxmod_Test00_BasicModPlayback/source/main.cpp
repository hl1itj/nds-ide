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

#include <maxmod9.h> // Include Maxmod

// Include the soundbank
#include "soundbank_bin.h"
#include "soundbank.h"

static portTASK_FUNCTION( testTask1, pvParameters );
	
int main(void)
{
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


static portTASK_FUNCTION( testTask1, pvParameters )
{
	// input here.

	// PAlib Initialization
	PA_Init();
	PA_LoadDefaultText(0, 0);

	// Initialize Maxmod
	mmInitDefaultMem((mm_addr)soundbank_bin);

	// Load the music
	mmLoad(MOD_MUSIC);

	// Display info text
	PA_OutputText(0, 1, 1, "Simple Maxmod example\n\nUse A to start the music,\nB to stop it");

	// Infinite loop to keep the program running
	while(true){
		// Play the module when the user presses A
		if(Pad.Newpress.A)
			mmStart(MOD_MUSIC, MM_PLAY_LOOP);
		else if(Pad.Newpress.B)
			mmStop();

		PA_WaitForVBL();
	}
}


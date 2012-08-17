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

	// Load and play the music
	mmLoad(MOD_MUSIC);
	mmStart(MOD_MUSIC, MM_PLAY_LOOP);

	// Load the sounds
	// Please note that they are regular .wav
	// files put into the audio folder
	mmLoadEffect(SFX_SFXA);
	mmLoadEffect(SFX_SFXB);

	// Display info text
	PA_OutputText(0, 1, 1, "Maxmod example:\nplaying music and sounds\n\nA - play sound effect #1\nB - play sound effect #2");

	// Infinite loop to keep the program running
	while(true){
		// Play the sounds when the user presses A or B
		if(Pad.Newpress.A)
			mmEffect(SFX_SFXA);
		if(Pad.Newpress.B)
			mmEffect(SFX_SFXB);

		PA_WaitForVBL();
	}
}


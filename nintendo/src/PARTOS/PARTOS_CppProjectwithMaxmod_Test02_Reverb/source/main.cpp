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

// Settings for the reverb engine
enum{
	rv_delay_left = 500,  // milliseconds
	rv_delay_right = 520,
	rv_rate = 32768,      // Hertz
	rv_format = 16        // 16-bit
};

static portTASK_FUNCTION( testTask1, pvParameters );
	
void setupReverb(){
	// Enable reverb system
	mmReverbEnable();

	// Calculate amount required and allocate memory
	// for both reverb channels
	void* rv_buffer_left;
	void* rv_buffer_right;

	int rv_size_left;
	int rv_size_right;
	rv_size_left = mmReverbBufferSize(rv_format, rv_rate, rv_delay_left);
	rv_size_right = mmReverbBufferSize(rv_format, rv_rate, rv_delay_right);

	rv_buffer_left  = malloc(rv_size_left * 4);
	rv_buffer_right = malloc(rv_size_right * 4);

	// Configure reverb settings
	mm_reverb_cfg config;

	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_FEEDBACK |
	               MMRF_PANNING | MMRF_DRYLEFT | MMRF_DRYRIGHT |
	               MMRF_RATE | MMRF_16BITLEFT | MMRF_16BITRIGHT |
	               MMRF_INVERSEPAN | MMRF_BOTH;

	config.memory = rv_buffer_left;   // Set memory target (for left)
	config.delay = rv_size_left;      // Set delay (for left)
	config.feedback = 1024;           // Set feedback to 50% (for both)
	config.panning = 0;               // Set panning to 0% (and inversed (100%) for right channel)
	config.rate = 16777216 / rv_rate; // Set sampling rate for both channels
	mmReverbConfigure(&config);       // Run configuration...

	// Configure remaining parameters (right channel memory and delay)
	config.flags = MMRF_MEMORY | MMRF_DELAY | MMRF_RIGHT;

	config.delay = rv_size_right;
	config.memory = rv_buffer_right;

	mmReverbConfigure(&config);
}

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

	// Setup the reverb engine
	setupReverb();

	// Load and play the music
	mmLoad(MOD_MUSIC);
	mmStart(MOD_MUSIC, MM_PLAY_LOOP);

	// Display info text
	PA_OutputText(0, 1, 1, "Maxmod reverb example\nTranslated to PAlib by fincs\n\nPress A to toggle reverb");

	bool reverb_active = false;

	// Infinite loop to keep the program running
	while(true){
		// Toggle reverb
		if(Pad.Newpress.A){
			reverb_active = !reverb_active;
			if(reverb_active){
				// start reverb
				mmReverbStart(MMRC_BOTH);

				// lower module volume & tempo
				// this creates a cool atmosphere
				mmSetModuleVolume(550);
				mmSetModuleTempo(800);
			}else{
				// stop reverb and restore volume and tempo
				mmReverbStop(MMRC_BOTH);
				mmSetModuleVolume(0x400);
				mmSetModuleTempo(0x400);
			}
		}

		PA_WaitForVBL();
	}
}


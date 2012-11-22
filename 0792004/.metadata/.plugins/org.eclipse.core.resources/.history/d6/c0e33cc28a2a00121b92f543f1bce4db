#include <FreeRTOS.h>
#include <nds.h>
#include "abcde.h"

int main (void) {
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG);
	bgInit(3,BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	decompress(abcdeBitmap, BG_GFX, LZ77Vram);

	vTaskStartScheduler();		// Never returns
		while(1)
			;

	return 0;
}

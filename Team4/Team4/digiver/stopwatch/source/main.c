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
//#include <sevencore_io.h>
#include "drunkenlogo.h"
#include "card_spi.h"
#include "gdbStub.h"
#include "gdbStubAsm.h"
#define TIMER_SPEED (BUS_CLOCK/1024)


typedef enum
{
    timerState_Stop,
    timerState_Pause,
    timerState_Running
}TimerStates;

//void InitDebug(void);
    uint ticks = 0;
    TimerStates state = timerState_Stop;
    
    int i, j = 0;
    int sc[100];
    int scp[100];
   
static portTASK_FUNCTION(Key_Press, pvParameters);
static
portTASK_FUNCTION(Touch_Press, pvParameters);
int
main(void)
{
					// Initialize Bottom Screen for printf()
	PrintConsole topScreen;
        videoSetMode(MODE_0_2D);
        vramSetBankA(VRAM_A_MAIN_BG);
        consoleInit(&topScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
        consoleSelect(&topScreen);

        videoSetModeSub(MODE_5_2D);
    	vramSetBankC(VRAM_C_SUB_BG);
        bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
    decompress(drunkenlogoBitmap, BG_GFX_SUB,  LZ77Vram);
	
	
	xTaskCreate(Key_Press,
					     (const signed char * const)"Key_Press",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 2,
					     NULL);
	
	xTaskCreate(Touch_Press,
					     (const signed char * const)"Touch_Press",
					     2048,
					     (void *)NULL,
					     tskIDLE_PRIORITY + 1,
					     NULL);
	vTaskStartScheduler();		// Never returns
	while(1)
		;
	return 0;
}



static
portTASK_FUNCTION(Key_Press, pvParameters)
{
    int down = keysDown();
        

    while(!(down & KEY_START))
    {int k=0;
        swiWaitForVBlank();
        consoleClear();
        scanKeys();
        down = keysDown();

        if(state == timerState_Running)
        {
            ticks += timerElapsed(0);
        }

        if(down & KEY_A)
        {
            if(state == timerState_Stop)
            {
                timerStart(0, ClockDivider_1024, 0, NULL);
                state = timerState_Running;
            }
            else if(state == timerState_Pause)
            {
                timerUnpause(0);
                state = timerState_Running;
                
            }
            else if(state == timerState_Running)
            {
                ticks += timerPause(0);
                state = timerState_Pause;
                sc[j] = ticks/TIMER_SPEED;
                scp[j] = ((ticks%TIMER_SPEED)*1000) /TIMER_SPEED;
                j++;
                

            }

        }
        else if(down & KEY_B)
        {
            timerStop(0);
            ticks = 0;
            state = timerState_Stop;
            j = 0;
        }

        iprintf("(A)     : Start & Pause Timer.\n(B)     : Clean Timer.\n(Start) : Exit Timer.\n\n");
        iprintf("Time\n");
        iprintf("ticks:  %u\n", ticks);
        iprintf("second: %u:%u\n", ticks/TIMER_SPEED, ((ticks%TIMER_SPEED)*1000) /TIMER_SPEED);
        iprintf("LIST\n");
        for(i = 0; i < j; i++) {
        iprintf(" [%d] :  %u : %u \n",i+1, sc[i],scp[i]);
        }
   	        
	

    }

    if(state != timerState_Stop)
    {
        timerStop(0);
    }
    vTaskDelay(portMAX_DELAY);
}

static
portTASK_FUNCTION(Touch_Press, pvParameters)
{


}

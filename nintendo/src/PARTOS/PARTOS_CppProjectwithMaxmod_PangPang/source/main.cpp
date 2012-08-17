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

// PAGfxConverter Include
#include "includefile.h"

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

	//사운드 관련
	mmLoad(MOD_GUNSHOT);
	mmLoad(MOD_KOONG);
	mmLoad(MOD_YEAH);

	while(1){
		Screen scr;

		scr.Ready_Screen();		// 스타트를 누를때까지 대기상태.

		mmStart(MOD_YEAH, MM_PLAY_LOOP); // 배경음악 사운드

		while(1){
			scr.SCreen_Change();

			Stage Game;		// game 객체 생성.

			while(1){
				Game.Location_Update();		// 모든 객체의 좌표값과 상태값 변경.
				Game.Image_Update();		// 모든 객체 이미지 뿌려줌.

				if(Pad.Newpress.Start && Game.isGameOver == 1) break;	// 게임도중 사망했을시.

				PA_WaitForVBL();
			}
			if(Game.isGameOver == 1){		// 사망했을시 초기상태로 돌아감.
				break;
			}
			Game.~Stage();
			PA_WaitForVBL();
		}
		scr.~Screen();
		PA_WaitForVBL();
	}

	//return 0;
}


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

#include "cs_api.h"
// Converted using PAGfx
#include "all_gfx.h"
#include "game.h"
#include "menu.h"

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

	// PAlib 초기화
	PA_Init();
	// PAlib VBL Reference 초기화
	PA_InitVBL();

	while(true)
	{
		// 메인 배경화면 출력
		PA_LoadBackground(UP_SCREEN, BG2, &bg_main_up);
		PA_LoadBackground(DOWN_SCREEN, BG2, &bg_main_down);

		int selnum=0;
		ret_next(); // 터치로 아무데나 누르면 다음 상태로의 전이
		PA_WaitForVBL();
		PA_LoadBackground(DOWN_SCREEN, BG2, &bg_menu); // 메뉴화면 출력

		selnum = sel_menu();

		if(selnum == 1){
			// 메뉴1 - 게임소개 (미구현)
		}
		else if(selnum == 2){
			// 메뉴2 - Single Player
			init_singleplay(); // 1player 초기화
			singleplay_start(); // 1player 게임시작
		}
		else if(selnum == 3){
			// 메뉴3 - Multi Player
		}
		else
			;
		PA_WaitForVBL();
	}
}


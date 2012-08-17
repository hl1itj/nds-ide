/*
 * Screen.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Screen.h"
#include "includefile.h"


#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define BACKGROUND_UP 1
#define BACKGROUND_DOWN 0

#define START_NUM 0
#define STAGE 1
#define GO 2

Screen::Screen() {
	PA_Init();
	PA_LoadBackground(UP_SCREEN, BACKGROUND_UP, &UP_Main);
	PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &DOWN_MAIN);

	PA_LoadSpritePal(DOWN_SCREEN, START_NUM, (void*)start_Pal);
	PA_CreateSprite(DOWN_SCREEN, START_NUM,(void*)start_Sprite, OBJ_SIZE_64X32,1, START_NUM, 96, 50);

	PA_LoadDefaultText(1, 0);

	stage = 1;
}

Screen::~Screen() {}


void Screen::Ready_Screen(){			// 시작 버튼을 누를때까지 대기.
	while(1){
		if(Stylus.Newpress){
			if(PA_SpriteTouched(START_NUM)){
				PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
				PA_DeleteSprite(DOWN_SCREEN, START_NUM);
				PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &BG2);
				break;
			}
		}
		if(Pad.Newpress.Start){
			PA_DeleteBg(DOWN_SCREEN, BACKGROUND_DOWN);
			PA_DeleteSprite(DOWN_SCREEN, START_NUM);
			PA_LoadBackground(DOWN_SCREEN, BACKGROUND_DOWN, &BG2);
			break;
		}
		PA_WaitForVBL();
	}
}


void Screen::SCreen_Change(){
	switch(stage){
	case 1:
		PA_LoadSpritePal(DOWN_SCREEN, STAGE, (void*)Stage1_Pal);
		PA_CreateSprite(DOWN_SCREEN, STAGE,(void*)Stage1_Sprite, OBJ_SIZE_64X32,1, STAGE, 96, 50);
		stage++;
		break;
	case 2:
		PA_LoadSpritePal(DOWN_SCREEN, STAGE, (void*)Stage2_Pal);
		PA_CreateSprite(DOWN_SCREEN, STAGE,(void*)Stage2_Sprite, OBJ_SIZE_64X32,1, STAGE, 96, 50);
		break;
	default:
		break;
	}

	int count1=0, count2=0;		// 딜레이를 주기 위해서 선언.
	while(1){
		if(count1 < 70) count1++;
		if(count1 == 70 && count2 == 0){
				PA_DeleteSprite(DOWN_SCREEN, STAGE);
				PA_LoadSpritePal(DOWN_SCREEN, GO, (void*)go_Pal);
				PA_CreateSprite(DOWN_SCREEN, GO,(void*)go_Sprite, OBJ_SIZE_64X32,1, GO, 96, 50);
		}
		if(count1 == 70 && count2 < 50){
			count2++;
		}
		if(count2 == 50){
			PA_DeleteSprite(DOWN_SCREEN, GO);
			break;
		}
		PA_WaitForVBL();
	}
}


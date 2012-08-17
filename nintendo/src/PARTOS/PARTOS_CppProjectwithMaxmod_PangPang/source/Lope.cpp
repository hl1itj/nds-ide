/*
 * Lope.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Lope.h"
#include "includefile.h"

#define DOWN_SCREEN 0
#define UP_SCREEN 1


#define LOPE_NUM 1

Lope::Lope() {
	x= -200;
	y= -200;
	isShot = 0;
	isSelected = 1;
	//PA_LoadSpritePal(스크린번호, 파렛트 번호, 파렛트)
	PA_LoadSpritePal(DOWN_SCREEN, LOPE_NUM, (void*)suri_Pal);	// 로프...

	//PA_CreateSprite(생성할 위치 스크린 번호, 할당시킬 스프라이트 번호, 스프라이트 그림 변수, 크기, 256색이면 1, 팔렛번호, x좌표,y좌표)
	PA_CreateSprite(DOWN_SCREEN, LOPE_NUM,(void*)suri_Sprite, OBJ_SIZE_8X8,1, LOPE_NUM, x, y); // 로프 생성
}

Lope::~Lope() {}

void Lope::change(){

}

void Lope::move(int ch_x, int ch_y){

	if(Pad.Newpress.A && isShot == 0){
		mmStart(MOD_GUNSHOT, MM_PLAY_ONCE);//총 발사시 사운드

		isShot = 1;
		x = ch_x;
		y = ch_y;
	}

	if(isShot == 1){
		if(y <= 0){
			isShot = 0;
			x = -200, y = -200;
		}
		y = y - 5;
	}
}

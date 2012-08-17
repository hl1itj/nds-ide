/*
 * Character.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Character.h"
#include "includefile.h"

#define DOWN_SCREEN 0
#define UP_SCREEN 1
#define CHARACTER_NUM 0

Character::Character() {
	x = 120;		// 초기 케릭터 위치 값.
	y = 160;
	isShild = 0;

	//PA_LoadSpritePal(스크린번호, 파렛트 번호, 파렛트)
	PA_LoadSpritePal(DOWN_SCREEN, CHARACTER_NUM, (void*)sprite0_Pal);	// 케릭터....
	//PA_CreateSprite(생성할 위치 스크린 번호, 할당시킬 스프라이트 번호, 스프라이트 그림 변수, 크기, 256색이면 1, 팔렛번호, x좌표,y좌표)
	PA_CreateSprite(DOWN_SCREEN, CHARACTER_NUM,(void*)som_Sprite, OBJ_SIZE_16X32,1, CHARACTER_NUM, x, y); // 케릭터 생성
}

Character::~Character() {
}

void Character::move() {
	////////////////////////// 케릭터 움직임 //////////////////////////
	//PA_StartSpriteAnim(스크린 번호, 스프라이트 번호, 처음 프레임, 마지막 프레임,  속도)
		if(Pad.Newpress.Up) PA_StartSpriteAnim(0, 0, 0, 3, 4);
		if(Pad.Newpress.Down) PA_StartSpriteAnim(0, 0, 8, 11, 4);

		if(Pad.Newpress.Right) {
			PA_StartSpriteAnim(0, 0, 4, 7, 4);
			PA_SetSpriteHflip(0, 0, 0);
		}
		if(Pad.Newpress.Left) {
			PA_StartSpriteAnim(0, 0, 4, 7, 4);
			PA_SetSpriteHflip(0, 0, 1);
		}
		if(!((Pad.Held.Left)||(Pad.Held.Up)||(Pad.Held.Down)||(Pad.Held.Right)))
			PA_SpriteAnimPause(0, 0, 1);

		if (Pad.Held.Up){}  // screen, sprite, frame
		if (Pad.Held.Down){}  // screen, sprite, frame
		if (Pad.Held.Left && x > 5){
			x-=2;
		}
		if (Pad.Held.Right && x < 236){
			x+=2;
		}
	//
}

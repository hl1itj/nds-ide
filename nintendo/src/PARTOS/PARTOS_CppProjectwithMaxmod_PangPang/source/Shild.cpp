/*
 * Shild.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Shild.h"
#include "includefile.h"

#define DOWN_SCREEN 0
#define UP_SCREEN 1

#define SHILED_ITEM 11

Shild::Shild() {
	x = -200;
	y = -200;
	PA_LoadSpritePal(DOWN_SCREEN, SHILED_ITEM, (void*)Ch_Shild_Pal);
	PA_CreateSprite(DOWN_SCREEN, SHILED_ITEM,(void*)Ch_Shild_Sprite, OBJ_SIZE_16X16,1, SHILED_ITEM, x, y);
}

Shild::~Shild() {}


void Shild::SetLocation(int ch_x, int ch_y){
	x = ch_x;
	y = ch_y;

}

void Shild::move() {
//	if(y < 176 && move_flag == 1){
//		y = y+1;
//	}
}

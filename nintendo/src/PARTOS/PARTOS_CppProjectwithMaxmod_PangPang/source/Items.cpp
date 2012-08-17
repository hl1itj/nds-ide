/*
 * Items.cpp
 *
 *  Created on: 2010. 11. 20.
 *      Author: 최진영, 박경호, 신동우
 */

#include "Items.h"
#include "includefile.h"

#define DOWN_SCREEN 0
#define UP_SCREEN 1

#define ITEM 	10


Items::Items() {
	x = -200;
	y = -200;
	move_flag = 0;
	size = 16;
	PA_LoadSpritePal(DOWN_SCREEN, ITEM, (void*)Shild_Pal);
	PA_CreateSprite(DOWN_SCREEN, ITEM,(void*)Shild_Sprite, OBJ_SIZE_16X16,1, ITEM, x, y);
}

Items::~Items() {
}

void Items::change(){
	srand(time(NULL));
	int num;
	num = rand() % 4 + 1;
	num = 1;		//아직 아이템을 다 구현 못해서. 1로 셋팅.... ㅠㅠ
	switch(num){
		case 1:
			PA_LoadSpritePal(DOWN_SCREEN, ITEM, (void*)Shild_Pal);
			PA_CreateSprite(DOWN_SCREEN, ITEM,(void*)Shild_Sprite, OBJ_SIZE_16X16,1, ITEM, x, y);
			move_flag =  1;
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
		}
}

void Items::SetLocation(int ch_x, int ch_y){
	x = ch_x;
	y = ch_y;

}

void Items::move() {
	if(y < 176 && move_flag == 1){
		y = y+1;
	}
}

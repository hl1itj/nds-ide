/*
 * crystal.cpp
 *
 *  Created on: 2010. 12. 16.
 *      Author: ywyw
 */

#include "crystal.h"
#include <PA9.h>
#include "all_gfx.h"
#include "cs_api.h"


crystal::crystal() {
	x = 10;
	y = 10;
}

crystal::~crystal() {

}

void crystal::move(int pal_num, int obj_num){

	sprite_print_dual((void*)water_Pal, (void*)water_Sprite, pal_num, obj_num, x, y); // 캐릭터 이미지 출력

	if(y != 0)
			y = y - 3;
	else{
		y = 190;
		wait(10);
	}
}

void crystal::destory()
{

}

/*
 * water.cpp
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#include "water.h"
#include <PA9.h>
#include "all_gfx.h"
#include "cs_api.h"

water::water() {
	 x = 10;
	 y = 10;
}

water::~water() {

}

void water::water_setting(int pal_num, int obj_num)
{
	 sprite_print_dual((void*)water_Pal, (void*)water_Sprite, pal_num, obj_num, x, y); // 물방울 이미지 출력
}

void water::move(int move_cnt)
{
	if(y < 390)
		y = y + move_cnt;
	else{
		x = PA_Rand()%250;
		y = 10;
	}
}

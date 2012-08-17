/*
 * monster.cpp
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#include "monster.h"
#include <PA9.h>
#include "all_gfx.h"
#include "cs_api.h"

monster::monster() {
	 x = 10;
	 y = 10;
	 hp = 100;
}

monster::~monster() {

}

void monster::monster_setting(int pal_num, int obj_num, int mon_X)
{
	sprite_print_dual2((void*)monster_Pal, (void*)monster_Sprite, pal_num, obj_num, mon_X, y); //몬스터 이미지 출력
}

void monster::move(int move_cnt)
{
	if(y < 390)
		y = y + move_cnt;
	else{
		x = PA_Rand()%250;
		y = 10;
	}
}

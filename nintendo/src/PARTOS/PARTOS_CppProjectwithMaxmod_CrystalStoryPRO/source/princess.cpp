/*
 * princess.cpp
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#include "princess.h"
#include <PA9.h>
#include "all_gfx.h"
#include "cs_api.h"

princess::princess() {
	 x = 230;
	 y = 10;
	 sprite_print2((void*)princess_Pal, (void*)princess_Sprite, x, y); // 캐릭터 이미지 출력
}

princess::~princess() {

}

void princess::move(int move_cnt)
{
	int cnt=0;
	if(cnt%50==0)
	{
		if(y==124)
			;
		else
			y = y + move_cnt;
	}

	if(Stylus.X | Stylus.Y)
	{
		if(Stylus.X >230 && Stylus.X < 250 && Stylus.Y >150 && Stylus.Y < 190)
			y = y - 5;
	}
	cnt++;

}

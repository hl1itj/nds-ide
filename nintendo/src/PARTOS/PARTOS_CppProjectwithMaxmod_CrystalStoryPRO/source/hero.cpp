/*
 * hero.cpp
 *
 *  Created on: 2010. 12. 17.
 *      Author: ywyw
 */

#include "hero.h"
#include "cs_api.h"
#include "all_gfx.h"

hero::hero() {
	x = 120;
	y = 130;
	sprite_print((void*)som_Pal, (void*)som_Sprite, x, y); // 캐릭터 이미지 출력
}

hero::~hero() {}

void hero::move() {
	if(Pad.Held.Right | Pad.Held.Left | Pad.Held.Down | Pad.Held.Up )
		{
			x += (Pad.Held.Right - Pad.Held.Left)*2;
		}
		if(!((Pad.Held.Left)||(Pad.Held.Right))) {
			PA_StartSpriteAnim(0, 0, 0, 0, 4);
		}
		if(Pad.Newpress.Right) {
			PA_StartSpriteAnim(0, 0, 4, 7, 6);
			PA_SetSpriteHflip(0, 0, 0);
		}
		if(Pad.Newpress.Left) {
			PA_StartSpriteAnim(0, 0, 4, 7, 6);
			PA_SetSpriteHflip(0, 0, 1);
		}
}

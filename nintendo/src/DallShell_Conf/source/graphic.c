/*
 * graphic.c
 *
 *  Created on: 2010. 7. 6.
 *      Author: 컴퓨터공학과
 */

#include "dall_conf.h"
#include "input.h"
#include "graphic.h"

struct ui_info ui[SPRITE_NUM] = {
    { // USB TAB 0
        0x0000000F,  // 이 버튼이 0,1,2,3번 화면에서 enable되는 버튼이면
        0x0000000F,  // 이 버튼이 눌리면, 4,5,6번 버튼이 있는 화면으로 전환
        1,            // 위 flag가 바뀌었다면, 새로 그림을 그려야 하므로 1,
        1,5,64,32,  // 이 버튼의 x, y, width, height
        &back_main,
        1,
        img64x32_Pal,
        img64x32_Sprite,     // Button 0의 이미지가 들어있는 배열
        NULL,   // Button 0에 focus가 있을 때 이미지, 없다면 NULL
        NULL,   // Button 0가 선택되면 실행 함수 어딘가에 있는 a_function();
        NULL   // 또다른 함수 b_function(인자);
    },
    // Serial TAB 1
    // {74,5,64,32}, // Serial TAB
    {
        0x0000000F, 0x0000000F, 1, 74,5,64,32, &back_main, 1,
        img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
	},
    //{148,5,64,32}, // Wifi TAB 2
    {
		0x0000000F, 0x000000FF, 1, 148,5,64,32, &back_main, 1,
		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    //{24,74,64,64}, // FIND AP 3
    {
		0x00000010, 0x0000F002, 1, 24,74,64,64, &back_sub, 1,
		img64x64_Pal, img64x64_Sprite, NULL, wifi_searchAP, NULL
    },
    //{99,74,64,64}, // Manual Setting 4
    {
		0x00000020, 0x00000F0F, 1, 99,74,64,64, &back_sub2, 1,
		img64x64_Pal, img64x64_Sprite, NULL, NULL, wifi_Manualconf,
    },
    //{168,74,64,64}, // Cancel 5
    {
		0x00000040, 0x0000000F, 1, 168,74,64,64, &back_main, 1,
		img64x64_Pal, img64x64_Sprite, NULL, NULL, NULL
    },
    //{5,162,64,32}, // Cancel(Manual Config section) 6
    {
 		0x00000100, 0x000000FF, 1, 5,162,64,32, &back_main, 1,
 		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    //{71,162,64,32}, // Wep(Manual Config section) 7
    {
  		0x00000200, NULL, 1, 71,162,64,32, &back_main, 0,
  		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    //{135,162,64,32}, // Find_AP(Manual Config section) 8
    {
		0x00000400, 0x0000F002, 1, 135,162,64,32, &back_sub, 1,
		img64x32_Pal, img64x32_Sprite, NULL, wifi_searchAP, NULL
    },
    // {199,162,64,32}, // Connect(Manual Config section) 9
    {
 	    0x00000700, 0x000F000F, 1, 199,162,64,32, &back_conn, 1,
 	    img64x32_Pal, img64x32_Sprite, NULL, NULL, wifi_APconn
    },
    // Cancel (FindAP Section) 10
    {
  		0x00001000, 0x000000FF, 1, 50,161,64,32, &back_main, 1,
  		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    // OK (FindAP Section) 11
    {
   		0x00004000, 0x000000FF, 1, 130,162,64,32, &back_main, 1,
   		img64x32_Pal,img64x32_Sprite, NULL, NULL, NULL
    },
    // Cancel (Connect Section) 12
    {
   		0x00010000, 0x00000F0F, 1, 60,134,64,32, &back_sub2, 1,
   		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    // OK (Connect Section) 13
    {
  		0x00020000, 0x00F0000F, 1, 135,135,64,32, &back_main, 1,
  		img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    // DNS Test (Test Section) 14
    {
        0x00100000, 0x0200000F, 1, 24,74,64,64, &back_main, 1,
        img64x64_Pal, img64x64_Sprite, NULL, NULL, wifi_Test_Dns
    },
    // Save Setting (Test Section) 15
    {
        0x00200000, 0x0100000F, 1, 99,74,64,64, &back_complete, 1,
        img64x64_Pal, img64x64_Sprite, NULL, wifi_SaveConf, NULL
    },
    // Cancel (Test Section) 16
    {
        0x00700000, 0x00000F04, 1, 168,74,64,64, &back_sub2, 1,
        img64x64_Pal, img64x64_Sprite, NULL, NULL, NULL
    },
    // OK (Save Setting Section) 17
    {
    	0x01000000, 0x0000000F, 1, 100,135,64,32, &back_main, 1,
    	img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    // Cancel (DNS Test Section) 18
    {
    	0x02000000, 0x00F0000F, 1, 100,150,64,32, &back_main, 1,
    	img64x32_Pal, img64x32_Sprite, NULL, NULL, NULL
    },
    // EDIT (Manul config SSID Edit) 19
    {
    	0x00000800, 0x00000001, 1, 207,44,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config IP Edit) 20
    {
    	0x00000900, 0x00000002, 1, 207,60,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config Gate Edit) 21
    {
    	0x00000A00, 0x00000003, 1, 207,76,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config Subnet Edit) 22
    {
    	0x00000B00, 0x00000004, 1, 207,92,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config DNS1 Edit) 23
    {
    	0x00000C00, 0x00000005, 1, 207,108,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config Dns2 Edit) 24
    {
    	0x00000D00, 0x00000006, 1, 207,124,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    },
    // EDIT (Manul config Channel Edit) 25
    {
    	0x00000E00, 0x00000007, 1, 207,140,32,16, &back_sub2, 1,
    	btn_edit32x16_Pal, btn_edit32x16_Sprite, NULL, NULL, key_inputString
    }
}; // 로 초기화를 합니다.

u16 gfx[SPRITE_NUM];

int cnt=0;
void load_sprite(int sprite_num, const unsigned short file_name_Pal[],const unsigned char file_name_Sprite[])
{
	if(sprite_num == 3 || sprite_num == 4 || sprite_num == 5 || sprite_num == 14  || sprite_num == 15 || sprite_num == 16)
	{PA_LoadSpritePal(DOWN_SCREEN, 3, (void *)file_name_Pal);
		if(sprite_num == 3) {
			PA_CreateSprite(DOWN_SCREEN, 3,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 3, 0);
			}
		else if(sprite_num == 4) {
			PA_CreateSprite(DOWN_SCREEN, 4,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 4, 1);
		}
		else if(sprite_num == 5) {
			PA_CreateSprite(DOWN_SCREEN, 5,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 5, 2);
		}
		else if(sprite_num == 14) {
			PA_CreateSprite(DOWN_SCREEN, 14,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 14, 3);
		}
		else if(sprite_num == 15) {
			PA_CreateSprite(DOWN_SCREEN, 15,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 15, 4);
		}
		else if(sprite_num == 16) {
			PA_CreateSprite(DOWN_SCREEN, 16,
			(void *)file_name_Sprite, OBJ_SIZE_64X64,
			TRUE, 3, ui[sprite_num].x, ui[sprite_num].y);
			PA_SetSpriteAnim(DOWN_SCREEN, 16, 5);
		}
	}
	else if(sprite_num == 0 || sprite_num == 1 || sprite_num == 2 || sprite_num == 6 || sprite_num == 7 || sprite_num == 8 || sprite_num == 9 || sprite_num == 10 || sprite_num == 11 || sprite_num == 12 || sprite_num == 13 || sprite_num == 17 || sprite_num == 18)
	{PA_LoadSpritePal(DOWN_SCREEN, 0, (void *)file_name_Pal);
		if(sprite_num == 0) {
					PA_CreateSprite(DOWN_SCREEN, 0,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 0, 0);
					}
		else if(sprite_num == 1) {
					PA_CreateSprite(DOWN_SCREEN, 1,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 1, 1);
		}
		else if(sprite_num == 2) {
					PA_CreateSprite(DOWN_SCREEN, 2,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 2, 2);
		}
		else if(sprite_num == 6) {
					PA_CreateSprite(DOWN_SCREEN, 6,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 6, 3);
		}

		else if(sprite_num == 7) {
					PA_CreateSprite(DOWN_SCREEN, 7,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 7, 4);
		}
		else if(sprite_num == 8) {
					PA_CreateSprite(DOWN_SCREEN, 8,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 8, 5);
		}
		else if(sprite_num == 9) {
					PA_CreateSprite(DOWN_SCREEN, 9,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 9, 6);
		}
		else if(sprite_num == 10) {
					PA_CreateSprite(DOWN_SCREEN, 10,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 10, 7);
		}
		else if(sprite_num == 11) {
					PA_CreateSprite(DOWN_SCREEN, 11,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 11, 8);
		}
		else if(sprite_num == 12) {
					PA_CreateSprite(DOWN_SCREEN, 12,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 12, 9);
		}
		else if(sprite_num == 13) {
					PA_CreateSprite(DOWN_SCREEN, 13,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 13, 10);
		}
		else if(sprite_num == 17) {
					PA_CreateSprite(DOWN_SCREEN, 17,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 17, 11);
		}
		else if(sprite_num == 18) {
					PA_CreateSprite(DOWN_SCREEN, 18,
					(void *)file_name_Sprite, OBJ_SIZE_64X32,
					TRUE, 0, ui[sprite_num].x, ui[sprite_num].y);
					PA_SetSpriteAnim(DOWN_SCREEN, 18, 12);
		}
	}

	else {
		PA_LoadSpritePal(DOWN_SCREEN, 5, (void *)file_name_Pal);
		if(sprite_num == 19) {
			PA_LoadSpritePal(DOWN_SCREEN, 5, (void *)file_name_Pal);
				PA_CreateSprite(DOWN_SCREEN, 19,
				(void *)file_name_Sprite, OBJ_SIZE_32X16,
				TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
				PA_SetSpriteAnim(DOWN_SCREEN, 19, 0);
			}
			else if(sprite_num == 20) {
						PA_CreateSprite(DOWN_SCREEN, 20,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 20, 1);
			}
			else if(sprite_num == 21) {
						PA_CreateSprite(DOWN_SCREEN, 21,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 21, 2);
			}
			else if(sprite_num == 22) {
						PA_CreateSprite(DOWN_SCREEN, 22,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 22, 3);
			}
			else if(sprite_num == 23) {
						PA_CreateSprite(DOWN_SCREEN, 23,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 23, 4);
			}
			else if(sprite_num == 24) {
						PA_CreateSprite(DOWN_SCREEN, 24,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 24, 0);
			}
			else if(sprite_num == 25) {
						PA_CreateSprite(DOWN_SCREEN, 25,
						(void *)file_name_Sprite, OBJ_SIZE_32X16,
						TRUE, 5, ui[sprite_num].x, ui[sprite_num].y);
						PA_SetSpriteAnim(DOWN_SCREEN, 25, 0);
			}
		}

}

void del_sprite(int sprite_num)
{
	PA_DeleteSprite(0,sprite_num); // Screen is always down-screen.
}

void UnLoad_Screen()
{
	PA_ResetBgSysScreen(0);
	PA_ResetSpriteSysScreen(0);
}

void load_background(PA_BgStruct* bg)
{
    PA_LoadBackground(DOWN_SCREEN, 3, bg);
}


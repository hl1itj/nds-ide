/*
 * graphic.h
 *
 *  Created on: 2010. 7. 6.
 *      Author: 컴퓨터공학과
 */

#ifndef GRAPHIC_H_
#define GRAPHIC_H_

#define TRUE 1
#define FALSE 0

#define UP_SCREEN 1
#define DOWN_SCREEN 0

#define SPRITE_NUM 30 // 스프라이트 개수

#include "all_gfx.h"

/* Image Processing Function */
void load_background(PA_BgStruct* bg);
void setsize_sprite();
void load_sprite(int sprite_num, const unsigned short file_name_Pal[],const unsigned char file_name_Sprite[]);
void del_sprite(int sprite_num);
void UnLoad_Screen();

struct ui_info {
    u64 flag;      // 이 버튼이 활성화된 (즉 화면에 표시될) 상황을 검사하기 위한 flag
    u64 next_flag; // 이 버튼이 눌린 뒤, 화면이 바뀐다면,..
                   // 그 화면에 보이는 버튼들이 enable된 flag
    int redraw;    // 이 버튼이 눌린 후 화면이 새로 그려져야 하면
                   // 즉, 새로운 메뉴 셋으로 바뀌면, 1, 아니면 0
                   // next_flag != current_flag이면 아마도 1
    unsigned int x;
    unsigned int y;
    unsigned int img_widht;
    unsigned int img_height;

    PA_BgStruct* bg;
    int bgflag; // background flag

    const unsigned short* file_name_Pal;
    const unsigned char* file_name_Sprite;
    //char *image;           // 실제 이 버튼 (tab) 스프라이트 이미지에 대한 pointer
    char *image_focused; // 이 버튼에 focus가 있을 때 이미지,

    int  (*action)(void);  // int형 함수 포인터
    void (*another_action)(void); // action이 다른 종류로 여러개 필요하면
};

extern struct ui_info ui[SPRITE_NUM];

#endif /* GRAPHIC_H_ */

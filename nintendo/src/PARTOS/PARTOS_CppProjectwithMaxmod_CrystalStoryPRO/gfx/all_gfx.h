// Graphics converted using PAGfx by Mollusk.

#pragma once

#include <PA_BgStruct.h>

#ifdef __cplusplus
extern "C"{
#endif

// Sprites:
extern const unsigned char hpminus_Sprite[256] _GFX_ALIGN; // Palette: hpminus_Pal
extern const unsigned char monster_Sprite[1024] _GFX_ALIGN; // Palette: monster_Pal
extern const unsigned char princess_Sprite[512] _GFX_ALIGN; // Palette: princess_Pal
extern const unsigned char som_Sprite[6144] _GFX_ALIGN; // Palette: som_Pal
extern const unsigned char water_Sprite[256] _GFX_ALIGN; // Palette: water_Pal

// Backgrounds:
extern const PA_BgStruct stage2;
extern const PA_BgStruct bg_main_down;
extern const PA_BgStruct bg_main_up;
extern const PA_BgStruct bg_menu;
extern const PA_BgStruct bg_stage1_init;
extern const PA_BgStruct bg_stage2_init;
extern const PA_BgStruct bg_stage3_init;
extern const PA_BgStruct gameover;
extern const PA_BgStruct stage1;

// Palettes:
extern const unsigned short hpminus_Pal[256] _GFX_ALIGN;
extern const unsigned short monster_Pal[256] _GFX_ALIGN;
extern const unsigned short princess_Pal[256] _GFX_ALIGN;
extern const unsigned short som_Pal[256] _GFX_ALIGN;
extern const unsigned short water_Pal[256] _GFX_ALIGN;

#ifdef __cplusplus
}
#endif

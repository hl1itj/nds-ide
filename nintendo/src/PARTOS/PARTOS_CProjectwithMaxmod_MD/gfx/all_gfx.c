//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "number.c"
#include "item.c"

// Background files : 
#include "Gameover.c"
#include "Pause_Down.c"
#include "Play_Down.c"
#include "Season.c"
#include "Shop.c"
#include "Title.c"
#include "Title_Down.c"

// Palette files : 
#include "number.pal.c"
#include "item.pal.c"
#include "Gameover.pal.c"
#include "Pause_Down.pal.c"
#include "Play_Down.pal.c"
#include "Season.pal.c"
#include "Shop.pal.c"
#include "Title.pal.c"
#include "Title_Down.pal.c"

// Background Pointers :
PAGfx_struct Gameover = {(void*)Gameover_Map, 768, (void*)Gameover_Tiles, 48896, (void*)Gameover_Pal, (int*)Gameover_Info };
PAGfx_struct Pause_Down = {(void*)Pause_Down_Map, 768, (void*)Pause_Down_Tiles, 48640, (void*)Pause_Down_Pal, (int*)Pause_Down_Info };
PAGfx_struct Play_Down = {(void*)Play_Down_Map, 768, (void*)Play_Down_Tiles, 48320, (void*)Play_Down_Pal, (int*)Play_Down_Info };
PAGfx_struct Season = {(void*)Season_Map, 768, (void*)Season_Tiles, 48192, (void*)Season_Pal, (int*)Season_Info };
PAGfx_struct Shop = {(void*)Shop_Map, 768, (void*)Shop_Tiles, 48384, (void*)Shop_Pal, (int*)Shop_Info };
PAGfx_struct Title = {(void*)Title_Map, 768, (void*)Title_Tiles, 29248, (void*)Title_Pal, (int*)Title_Info };
PAGfx_struct Title_Down = {(void*)Title_Down_Map, 768, (void*)Title_Down_Tiles, 35840, (void*)Title_Down_Pal, (int*)Title_Down_Info };


#ifdef __cplusplus
}
#endif


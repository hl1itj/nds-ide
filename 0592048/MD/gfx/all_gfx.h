//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char number_Sprite[17408] __attribute__ ((aligned (4))) ;  // Pal : number_Pal
extern const unsigned char item_Sprite[13312] __attribute__ ((aligned (4))) ;  // Pal : item_Pal

// Background files : 
extern const int Gameover_Info[3]; // BgMode, Width, Height
extern const unsigned short Gameover_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Gameover_Pal
extern const unsigned char Gameover_Tiles[48896] __attribute__ ((aligned (4))) ;  // Pal : Gameover_Pal
extern PAGfx_struct Gameover; // background pointer

extern const int Pause_Down_Info[3]; // BgMode, Width, Height
extern const unsigned short Pause_Down_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Pause_Down_Pal
extern const unsigned char Pause_Down_Tiles[48640] __attribute__ ((aligned (4))) ;  // Pal : Pause_Down_Pal
extern PAGfx_struct Pause_Down; // background pointer

extern const int Play_Down_Info[3]; // BgMode, Width, Height
extern const unsigned short Play_Down_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Play_Down_Pal
extern const unsigned char Play_Down_Tiles[48320] __attribute__ ((aligned (4))) ;  // Pal : Play_Down_Pal
extern PAGfx_struct Play_Down; // background pointer

extern const int Season_Info[3]; // BgMode, Width, Height
extern const unsigned short Season_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Season_Pal
extern const unsigned char Season_Tiles[48192] __attribute__ ((aligned (4))) ;  // Pal : Season_Pal
extern PAGfx_struct Season; // background pointer

extern const int Shop_Info[3]; // BgMode, Width, Height
extern const unsigned short Shop_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Shop_Pal
extern const unsigned char Shop_Tiles[48384] __attribute__ ((aligned (4))) ;  // Pal : Shop_Pal
extern PAGfx_struct Shop; // background pointer

extern const int Title_Info[3]; // BgMode, Width, Height
extern const unsigned short Title_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Title_Pal
extern const unsigned char Title_Tiles[29248] __attribute__ ((aligned (4))) ;  // Pal : Title_Pal
extern PAGfx_struct Title; // background pointer

extern const int Title_Down_Info[3]; // BgMode, Width, Height
extern const unsigned short Title_Down_Map[768] __attribute__ ((aligned (4))) ;  // Pal : Title_Down_Pal
extern const unsigned char Title_Down_Tiles[35840] __attribute__ ((aligned (4))) ;  // Pal : Title_Down_Pal
extern PAGfx_struct Title_Down; // background pointer


// Palette files : 
extern const unsigned short number_Pal[125] __attribute__ ((aligned (4))) ;
extern const unsigned short item_Pal[93] __attribute__ ((aligned (4))) ;
extern const unsigned short Gameover_Pal[128] __attribute__ ((aligned (4))) ;
extern const unsigned short Pause_Down_Pal[129] __attribute__ ((aligned (4))) ;
extern const unsigned short Play_Down_Pal[128] __attribute__ ((aligned (4))) ;
extern const unsigned short Season_Pal[120] __attribute__ ((aligned (4))) ;
extern const unsigned short Shop_Pal[125] __attribute__ ((aligned (4))) ;
extern const unsigned short Title_Pal[82] __attribute__ ((aligned (4))) ;
extern const unsigned short Title_Down_Pal[129] __attribute__ ((aligned (4))) ;


#endif


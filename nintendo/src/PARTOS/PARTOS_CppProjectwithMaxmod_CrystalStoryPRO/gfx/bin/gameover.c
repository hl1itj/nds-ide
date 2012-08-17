#include <PA_BgStruct.h>

extern const char gameover_Tiles[];
extern const char gameover_Map[];
extern const char gameover_Pal[];

const PA_BgStruct gameover = {
	PA_BgNormal,
	256, 192,

	gameover_Tiles,
	gameover_Map,
	{gameover_Pal},

	47616,
	{1536}
};

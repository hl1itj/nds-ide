#include <PA_BgStruct.h>

extern const char bg_down_Tiles[];
extern const char bg_down_Map[];
extern const char bg_down_Pal[];

const PA_BgStruct bg_down = {
	PA_BgNormal,
	256, 192,

	bg_down_Tiles,
	bg_down_Map,
	{bg_down_Pal},

	5888,
	{1536}
};

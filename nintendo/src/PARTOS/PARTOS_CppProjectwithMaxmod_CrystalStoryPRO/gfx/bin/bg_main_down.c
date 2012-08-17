#include <PA_BgStruct.h>

extern const char bg_main_down_Tiles[];
extern const char bg_main_down_Map[];
extern const char bg_main_down_Pal[];

const PA_BgStruct bg_main_down = {
	PA_BgNormal,
	256, 192,

	bg_main_down_Tiles,
	bg_main_down_Map,
	{bg_main_down_Pal},

	9600,
	{1536}
};

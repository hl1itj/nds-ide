#include <PA_BgStruct.h>

extern const char bg_main_up_Tiles[];
extern const char bg_main_up_Map[];
extern const char bg_main_up_Pal[];

const PA_BgStruct bg_main_up = {
	PA_BgNormal,
	256, 192,

	bg_main_up_Tiles,
	bg_main_up_Map,
	{bg_main_up_Pal},

	14016,
	{1536}
};

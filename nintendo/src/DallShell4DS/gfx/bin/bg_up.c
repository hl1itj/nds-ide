#include <PA_BgStruct.h>

extern const char bg_up_Tiles[];
extern const char bg_up_Map[];
extern const char bg_up_Pal[];

const PA_BgStruct bg_up = {
	PA_BgNormal,
	256, 192,

	bg_up_Tiles,
	bg_up_Map,
	{bg_up_Pal},

	4032,
	{1536}
};

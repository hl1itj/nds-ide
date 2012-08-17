#include <PA_BgStruct.h>

extern const char bg_create_Tiles[];
extern const char bg_create_Map[];
extern const char bg_create_Pal[];

const PA_BgStruct bg_create = {
	PA_BgNormal,
	256, 192,

	bg_create_Tiles,
	bg_create_Map,
	{bg_create_Pal},

	4032,
	{1536}
};

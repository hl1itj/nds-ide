#include <PA_BgStruct.h>

extern const char DOWN_MAIN_Tiles[];
extern const char DOWN_MAIN_Map[];
extern const char DOWN_MAIN_Pal[];

const PA_BgStruct DOWN_MAIN = {
	PA_BgNormal,
	256, 192,

	DOWN_MAIN_Tiles,
	DOWN_MAIN_Map,
	{DOWN_MAIN_Pal},

	39808,
	{1536}
};

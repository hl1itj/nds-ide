#include <PA_BgStruct.h>

extern const char BG2_Tiles[];
extern const char BG2_Map[];
extern const char BG2_Pal[];

const PA_BgStruct BG2 = {
	PA_BgNormal,
	256, 192,

	BG2_Tiles,
	BG2_Map,
	{BG2_Pal},

	45120,
	{1536}
};

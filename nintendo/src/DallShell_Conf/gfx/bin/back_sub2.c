#include <PA_BgStruct.h>

extern const char back_sub2_Tiles[];
extern const char back_sub2_Map[];
extern const char back_sub2_Pal[];

const PA_BgStruct back_sub2 = {
	PA_BgNormal,
	256, 192,

	back_sub2_Tiles,
	back_sub2_Map,
	{back_sub2_Pal},

	9536,
	{1536}
};

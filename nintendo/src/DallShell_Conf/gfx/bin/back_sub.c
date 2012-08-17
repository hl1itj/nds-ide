#include <PA_BgStruct.h>

extern const char back_sub_Tiles[];
extern const char back_sub_Map[];
extern const char back_sub_Pal[];

const PA_BgStruct back_sub = {
	PA_BgNormal,
	256, 192,

	back_sub_Tiles,
	back_sub_Map,
	{back_sub_Pal},

	33088,
	{1536}
};

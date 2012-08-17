#include <PA_BgStruct.h>

extern const char back_main_Tiles[];
extern const char back_main_Map[];
extern const char back_main_Pal[];

const PA_BgStruct back_main = {
	PA_BgNormal,
	256, 192,

	back_main_Tiles,
	back_main_Map,
	{back_main_Pal},

	4224,
	{1536}
};

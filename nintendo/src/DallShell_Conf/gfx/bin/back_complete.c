#include <PA_BgStruct.h>

extern const char back_complete_Tiles[];
extern const char back_complete_Map[];
extern const char back_complete_Pal[];

const PA_BgStruct back_complete = {
	PA_BgNormal,
	256, 192,

	back_complete_Tiles,
	back_complete_Map,
	{back_complete_Pal},

	3904,
	{1536}
};

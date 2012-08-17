#include <PA_BgStruct.h>

extern const char stage2_Tiles[];
extern const char stage2_Map[];
extern const char stage2_Pal[];

const PA_BgStruct stage2 = {
	PA_BgUnlimited,
	256, 400,

	stage2_Tiles,
	stage2_Map,
	{stage2_Pal},

	100096,
	{1600}
};

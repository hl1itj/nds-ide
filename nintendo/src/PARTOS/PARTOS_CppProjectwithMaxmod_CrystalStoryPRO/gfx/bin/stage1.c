#include <PA_BgStruct.h>

extern const char stage1_Tiles[];
extern const char stage1_Map[];
extern const char stage1_Pal[];

const PA_BgStruct stage1 = {
	PA_BgUnlimited,
	256, 400,

	stage1_Tiles,
	stage1_Map,
	{stage1_Pal},

	102400,
	{1600}
};

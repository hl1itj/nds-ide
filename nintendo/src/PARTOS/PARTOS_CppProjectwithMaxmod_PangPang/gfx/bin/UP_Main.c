#include <PA_BgStruct.h>

extern const char UP_Main_Tiles[];
extern const char UP_Main_Map[];
extern const char UP_Main_Pal[];

const PA_BgStruct UP_Main = {
	PA_BgNormal,
	256, 192,

	UP_Main_Tiles,
	UP_Main_Map,
	{UP_Main_Pal},

	14400,
	{1536}
};

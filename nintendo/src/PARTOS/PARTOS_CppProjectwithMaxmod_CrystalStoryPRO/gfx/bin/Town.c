#include <PA_BgStruct.h>

extern const char Town_Tiles[];
extern const char Town_Map[];
extern const char Town_Pal[];

const PA_BgStruct Town = {
	PA_BgUnlimited,
	1024, 1024,

	Town_Tiles,
	Town_Map,
	{Town_Pal},

	114560,
	{16384}
};

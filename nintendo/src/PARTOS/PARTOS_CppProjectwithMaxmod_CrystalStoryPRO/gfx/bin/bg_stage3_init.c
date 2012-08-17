#include <PA_BgStruct.h>

extern const char bg_stage3_init_Tiles[];
extern const char bg_stage3_init_Map[];
extern const char bg_stage3_init_Pal[];

const PA_BgStruct bg_stage3_init = {
	PA_BgNormal,
	256, 192,

	bg_stage3_init_Tiles,
	bg_stage3_init_Map,
	{bg_stage3_init_Pal},

	5952,
	{1536}
};

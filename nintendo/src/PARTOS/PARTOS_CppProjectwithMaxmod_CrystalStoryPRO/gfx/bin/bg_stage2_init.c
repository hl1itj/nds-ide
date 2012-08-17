#include <PA_BgStruct.h>

extern const char bg_stage2_init_Tiles[];
extern const char bg_stage2_init_Map[];
extern const char bg_stage2_init_Pal[];

const PA_BgStruct bg_stage2_init = {
	PA_BgNormal,
	256, 192,

	bg_stage2_init_Tiles,
	bg_stage2_init_Map,
	{bg_stage2_init_Pal},

	6144,
	{1536}
};

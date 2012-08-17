#include <PA_BgStruct.h>

extern const char bg_stage1_init_Tiles[];
extern const char bg_stage1_init_Map[];
extern const char bg_stage1_init_Pal[];

const PA_BgStruct bg_stage1_init = {
	PA_BgNormal,
	256, 192,

	bg_stage1_init_Tiles,
	bg_stage1_init_Map,
	{bg_stage1_init_Pal},

	6592,
	{1536}
};

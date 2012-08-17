#include <PA_BgStruct.h>

extern const char bg_dialog_Tiles[];
extern const char bg_dialog_Map[];
extern const char bg_dialog_Pal[];

const PA_BgStruct bg_dialog = {
	PA_BgNormal,
	256, 192,

	bg_dialog_Tiles,
	bg_dialog_Map,
	{bg_dialog_Pal},

	3136,
	{1536}
};

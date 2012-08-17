#include <PA_BgStruct.h>

extern const char bg_menu_Tiles[];
extern const char bg_menu_Map[];
extern const char bg_menu_Pal[];

const PA_BgStruct bg_menu = {
	PA_BgNormal,
	256, 192,

	bg_menu_Tiles,
	bg_menu_Map,
	{bg_menu_Pal},

	12544,
	{1536}
};

#include <PA_BgStruct.h>

extern const char bg_edit_dialog_Tiles[];
extern const char bg_edit_dialog_Map[];
extern const char bg_edit_dialog_Pal[];

const PA_BgStruct bg_edit_dialog = {
	PA_BgNormal,
	256, 192,

	bg_edit_dialog_Tiles,
	bg_edit_dialog_Map,
	{bg_edit_dialog_Pal},

	2048,
	{1536}
};

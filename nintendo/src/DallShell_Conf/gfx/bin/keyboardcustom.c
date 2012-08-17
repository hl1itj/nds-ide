#include <PA_BgStruct.h>

extern const char keyboardcustom_Tiles[];
extern const char keyboardcustom_Map[];
extern const char keyboardcustom_Pal[];

const PA_BgStruct keyboardcustom = {
	PA_BgLarge,
	256, 1024,

	keyboardcustom_Tiles,
	keyboardcustom_Map,
	{keyboardcustom_Pal},

	30016,
	{8192}
};

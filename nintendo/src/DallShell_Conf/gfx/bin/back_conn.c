#include <PA_BgStruct.h>

extern const char back_conn_Tiles[];
extern const char back_conn_Map[];
extern const char back_conn_Pal[];

const PA_BgStruct back_conn = {
	PA_BgNormal,
	256, 192,

	back_conn_Tiles,
	back_conn_Map,
	{back_conn_Pal},

	5376,
	{1536}
};

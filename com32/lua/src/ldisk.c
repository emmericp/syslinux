/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2014 Paul Emmerich, First Colo GmbH - All Rights Reserved
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------- */

#include <stdlib.h>
#include <unistd.h>

#include <disk/geom.h>
#include <disk/bootloaders.h>
#include <disk/errno_disk.h>
#include <disk/error.h>
#include <disk/mbrs.h>
#include <disk/msdos.h>
#include <disk/partition.h>
#include <disk/swsusp.h>
#include <disk/read.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define SET_TABLE_STRING_INT(state, key, value) do {\
		lua_pushstring((state), (key));\
		lua_pushinteger((state), (value));\
		lua_settable((state), -3);\
} while(0);

#define SET_TABLE_STRING_STRING(state, key, value) do {\
		lua_pushstring((state), (key));\
		lua_pushstring((state), (const char*) (value));\
		lua_settable((state), -3);\
} while(0);

static int get_disks(lua_State* L) {
	lua_newtable(L);
	int i = 1;
	for (int id = 0x80; id < 0xff; id++) {
		struct driveinfo info = {
			.disk = id,
		};
		if (get_drive_parameters(&info) == -1 // disk does not exist
		|| !info.cbios) { // is not an hdd (-> a cdrom or whatever)
		    continue;
		}
		lua_pushinteger(L, i);
		lua_newtable(L);
		SET_TABLE_STRING_INT(L, "id", id);
		// legacy info
		SET_TABLE_STRING_INT(L, "legacy_heads", info.legacy_max_head + 1);
		SET_TABLE_STRING_INT(L, "legacy_cylinders", info.legacy_max_cylinder + 1);
		SET_TABLE_STRING_INT(L, "legacy_sectors_per_track",
			info.legacy_sectors_per_track + 1);
		// edd
		SET_TABLE_STRING_INT(L, "info", info.edd_params.info);
		SET_TABLE_STRING_INT(L, "cylinders", info.edd_params.cylinders);
		SET_TABLE_STRING_INT(L, "heads", info.edd_params.heads);
		SET_TABLE_STRING_INT(L, "sectors", info.edd_params.sectors);
		// size in mb as lua number type is uint32_t
		SET_TABLE_STRING_INT(L, "size",
			(uint64_t) info.edd_params.sectors * info.edd_params.bytes_per_sector
			/ (1024 * 1024));
		SET_TABLE_STRING_STRING(L, "host_bus", info.edd_params.host_bus_type);
		SET_TABLE_STRING_STRING(L, "interface", info.edd_params.interface_type);
		lua_settable(L, -3);
		i++;
    	}
	return 1;
}

static const luaL_Reg disklib[] = {
    {"get_disks", get_disks},
    {NULL, NULL}
};


LUALIB_API int luaopen_ldisk(lua_State* L) {
    luaL_newlib(L, disklib);
    return 1;
}


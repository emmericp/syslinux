/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2006 Erwan Velu - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
 *   Boston MA 02111-1307, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

#ifndef DMI_BASE_BOARD_H
#define DMI_BASE_BOARD_H

#include "stdbool.h"
#define BASE_BOARD_MANUFACTURER_SIZE       	32
#define BASE_BOARD_PRODUCT_NAME_SIZE		32	
#define BASE_BOARD_VERSION_SIZE   		16
#define BASE_BOARD_SERIAL_SIZE  		32
#define BASE_BOARD_ASSET_TAG_SIZE  		32
#define BASE_BOARD_LOCATION_SIZE		32
#define BASE_BOARD_FEATURES_SIZE		32
#define BASE_BOARD_TYPE_SIZE			32

#define BASE_BOARD_NB_ELEMENTS			5

static const char *base_board_features_strings[]={
                "Board is a hosting board", /* 0 */
                "Board requires at least one daughter board",
                "Board is removable",
                "Board is replaceable",
                "Board is hot swappable" /* 4 */
};

/* this struct have BASE_BOARD_NB_ELEMENTS */
/* each bool is associated to the relevant message above */
typedef struct {
bool hosting;
bool board_needs_daughter;
bool removable;
bool replaceable;
bool hot_swappable;
} __attribute__((__packed__)) s_base_board_features;

typedef struct {
char manufacturer[BASE_BOARD_MANUFACTURER_SIZE];
char product_name[BASE_BOARD_PRODUCT_NAME_SIZE];
char version[BASE_BOARD_VERSION_SIZE];
char serial[BASE_BOARD_SERIAL_SIZE];
char asset_tag[BASE_BOARD_ASSET_TAG_SIZE];
char location[BASE_BOARD_LOCATION_SIZE];
char type[BASE_BOARD_TYPE_SIZE];
s_base_board_features features;
} s_base_board;

#endif

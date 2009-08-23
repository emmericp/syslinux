/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2009 Pierre-Alexandre Meyer
 *
 *   This file is part of Syslinux, and is made available under
 *   the terms of the GNU General Public License version 2.
 *
 * ----------------------------------------------------------------------- */

#include <disk/common.h>
#include <disk/geom.h>
#include <disk/read.h>
#include <stdlib.h>
#include <string.h>

/**
 * get_bootloader_string - return a string describing the boot code
 * @label:		first two bytes of the MBR
 * @buffer:		pre-allocated buffer
 * @buffer_size:	@buffer size
 **/
void get_bootloader_string(const int label, char* buffer, const int buffer_size)
{
	switch (label) {
	case 0x0000:
	case 0xfa33:
	case 0xfab8:
	case 0xfabe:
		strncpy(buffer, "No bootloader", buffer_size - 1); break;
	case 0x0ebe: strncpy(buffer, "ThinkPad", buffer_size - 1); break;
	case 0x31c0: strncpy(buffer, "Acer 3", buffer_size - 1); break;
	case 0x33c0: strncpy(buffer, "Windows", buffer_size - 1); break;
	case 0x33ff: strncpy(buffer, "HP/Gateway", buffer_size - 1); break;
	case 0xb800: strncpy(buffer, "Plop", buffer_size - 1); break;
	case 0xea1e: strncpy(buffer, "Truecrypt Boot Loader", buffer_size - 1); break;
	case 0xeb04: strncpy(buffer, "Solaris", buffer_size - 1); break;
	case 0xeb48: strncpy(buffer, "Grub", buffer_size - 1); break;
	case 0xeb4c: strncpy(buffer, "Grub2", buffer_size - 1); break;
	case 0xeb5e: strncpy(buffer, "Grub4Dos", buffer_size - 1); break;
	case 0xfa31: strncpy(buffer, "Syslinux or Master Boot LoaDeR", buffer_size - 1); break;
	case 0xfaeb: strncpy(buffer, "Lilo", buffer_size - 1); break;
	case 0xfc31: strncpy(buffer, "Testdisk", buffer_size - 1); break;
	case 0xfc33: strncpy(buffer, "Gag", buffer_size - 1); break;
	case 0xfceb: strncpy(buffer, "BootIT NG", buffer_size - 1); break;
	default: strncpy(buffer, "Unknown bootloader", buffer_size - 1); break;
	}

	buffer[buffer_size - 1] = '\0';
}

/**
 * get_bootloader_id - return the first two bytes of the MBR
 * @d:		driveinfo struct describing the drive
 **/
int get_bootloader_id(const struct driveinfo *d)
{
	char mbr[SECTOR * sizeof(char)];

	if (read_mbr(d->disk, &mbr) == -1)
		return -1;
	else {
		uint16_t bootloader_id;
		/* Reverse the opcodes */
		bootloader_id = *(uint8_t *) (mbr + 1);
		bootloader_id += (*(uint8_t *) mbr << 8);
		return bootloader_id;
	}
}

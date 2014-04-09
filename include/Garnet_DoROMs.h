/******************************************************************************
 *
 * Copyright (c) 2010 Aceeca Ltd
 *
 * File: Garnet_DoROMs.h
 *
 * Release: 
 *
 * Description:
 *		
 *
 *****************************************************************************/

#ifndef __Garnet_DoROMs_H
#define __Garnet_DoROMs_H

#define GARNET_ROM		1
#define FACTORY_ROM		2
#define UBOOT_ROM			3

void WriteROM(int rom_id);
void LoadROM(int rom_id);

unsigned int CheckROM(int rom_id, int verbose);
unsigned int DoROMcsum (int rom_id);

#endif	// __Garnet_DoROMs_H

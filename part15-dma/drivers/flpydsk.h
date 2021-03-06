
#ifndef _FLPYDSK_DRIVER_H
#define _FLPYDSK_DRIVER_H

#include <stdint.h>

//! install floppy driver
void flpydsk_install ();

//! set current working drive
void flpydsk_set_working_drive (uint8_t drive);

//! get current working drive
uint8_t flpydsk_get_working_drive ();

//! read a sector
uint8_t* flpydsk_read_sector (int sectorLBA);

//! converts an LBA address to CHS
void flpydsk_lba_to_chs (int lba,int *head,int *track,int *sector);

void flpydsk_set_dma (int addr);
#endif

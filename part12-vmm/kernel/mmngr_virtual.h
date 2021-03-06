#ifndef _MMNGR_VIRT_H
#define _MMNGR_VIRT_H
//****************************************************************************
//**
//**    mmngr_virtual.h
//**		-Virtual Memory Manager
//**
//****************************************************************************
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================

#include "common.h"
#include "vmmngr_pte.h"
#include "vmmngr_pde.h"

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

//! virtual address
typedef uint32_t virtual_addr;

//! i86 architecture defines 1024 entries per table--do not change
#define PAGES_PER_TABLE 1024
#define PAGES_PER_DIR	1024

#define PAGE_DIRECTORY_INDEX(x) (((x) >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(x) (((x) >> 12) & 0x3ff)
#define PAGE_GET_PHYSICAL_ADDRESS(x) (*x & ~0xfff)

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================

//! page table
struct ptable {

	pt_entry m_entries[PAGES_PER_TABLE];
};

//! page directory
struct pdirectory {

	pd_entry m_entries[PAGES_PER_DIR];
};


//! maps phys to virtual address
extern void MmMapPage (void* phys, void* virt);

//! initialize the memory manager
extern void vmmngr_initialize ();

//! allocates a page in physical memory
extern int vmmngr_alloc_page (pt_entry*);

//! frees a page in physical memory
extern void vmmngr_free_page (pt_entry* e);

//! switch to a new page directory
extern int vmmngr_switch_pdirectory(struct pdirectory* dir);

//! get current page directory
extern struct pdirectory* vmmngr_get_directory ();

//! flushes a cached translation lookaside buffer (TLB) entry
extern void vmmngr_flush_tlb_entry (virtual_addr addr);

//! clears a page table
extern void vmmngr_ptable_clear (struct ptable* p);

//! convert virtual address to page table index
extern uint32_t vmmngr_ptable_virt_to_index (virtual_addr addr);

//! get page entry from page table
extern pt_entry* vmmngr_ptable_lookup_entry (struct ptable* p,virtual_addr addr);

//! convert virtual address to page directory index
extern uint32_t vmmngr_pdirectory_virt_to_index (virtual_addr addr);

//! clears a page directory table
extern void vmmngr_pdirectory_clear (struct pdirectory* dir);

//! get directory entry from directory table
extern pd_entry* vmmngr_pdirectory_lookup_entry (struct pdirectory* p, virtual_addr addr);

#endif

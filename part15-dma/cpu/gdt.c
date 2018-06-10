
//****************************************************************************
//**
//**    gdt.cpp
//**
//**	global descriptor table (gdt) for i86 processors. This handles
//**	the basic memory map for the system and permission levels
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include "gdt.h"
#include "../libc/string.h"
#include "../drivers/printk.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================


//! processor gdtr register points to base of gdt. This helps
//! us set up the pointer

//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================




typedef struct
{
  uint16_t limit_low;     // The lower 16 bits of the limit.
  uint16_t base_low;      // The lower 16 bits of the base.
  uint8_t base_middle;    // The next 8 bits of the base.
  uint8_t access;         // Access flags, determines what ring this segment can be used in.
  uint8_t granularity;
  uint8_t base_high;      // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_t;

// This struct describes a GDT pointer. It points to the start of
// our array of GDT entries, and is in the format required by the
// lgdt instruction.
typedef struct
{
    uint16_t limit;       // The Global Descriptor Table limit.
    uint32_t base;        // The address of the first gdt_entry_t struct.
} __attribute__((packed)) gdt_ptr_t;

static gdt_entry_t gdt_entries [3];
static gdt_ptr_t gdt_ptr;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

//! install gdtr
static void gdt_install ();

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

//! install gdtr
static void gdt_install () {
	__asm__ __volatile__("lgdtl (%0)" : : "r" (&gdt_ptr));
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================


//! Setup a descriptor in the Global Descriptor Table
void gdt_set_descriptor(uint32_t i, uint64_t base, uint64_t limit, uint8_t access, uint8_t grand)
{
	// if (i > MAX_DESCRIPTORS)
	// 	return;

	// //! null out the descriptor
	// memset ((void*)&_gdt[i], 0, sizeof (gdt_descriptor));

	// //! set limit and base addresses
	// _gdt[i].baseLo	= (uint16_t)(base & 0xffff);
	// _gdt[i].baseMid	= (uint8_t)((base >> 16) & 0xff);
	// _gdt[i].baseHi	= (uint8_t)((base >> 24) & 0xff);
	// _gdt[i].limit	= (uint16_t)(limit & 0xffff);

	// //! set flags and grandularity bytes
	// _gdt[i].flags = access;
	// _gdt[i].grand = (uint8_t)((limit >> 16) & 0x0f);
	// _gdt[i].grand |= grand & 0xf0;
}


//! returns descriptor in gdt
// struct gdt_descriptor* i86_gdt_get_descriptor (int i) {

// 	if (i > MAX_DESCRIPTORS)
// 		return 0;

// 	return &_gdt[i];
// }


//! initialize gdt
int i86_gdt_initialize () {

	//! set up gdtr
	gdt_ptr.limit = sizeof (gdt_entry_t) * 3 - 1;
	gdt_ptr.base = (uint32_t) &gdt_entries;

	printk("uint16_t: %d", gdt_ptr.limit);
	printk("uint16_t: %x", gdt_ptr.base);

	gdt_set_gate (0, 0, 0, 0, 0);             // Null segment.
  	gdt_set_gate (1, 0, 0xFFFFF, 0x9A, 0xCF); // Code segment.
  	gdt_set_gate (2, 0, 0xFFFFF, 0x92, 0xCF); // Data segment.

	// //! install gdtr
	gdt_install ();
	printk("Installed ");
	return 0;
}

void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low    = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high   = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low   = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access      = access;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[gdt.cpp]
//**
//****************************************************************************

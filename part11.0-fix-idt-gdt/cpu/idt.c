#include "idt.h"
#include "../libc/string.h"
#include "hal.h"


//! describes the structure for the processors idtr register
struct idtr {
	//! size of the interrupt descriptor table (idt)
	uint16_t		limit;
	//! base address of idt
	uint32_t		base;
};

#ifdef _MSC_VER
#pragma pack (pop, 1)
#endif

//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

//! interrupt descriptor table
static struct idt_descriptor	_idt [I86_MAX_INTERRUPTS];

//! idtr structure used to help define the cpu's idtr register
static struct idtr				_idtr;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

//! installs idtr into processors idtr register
static void idt_install ();

//! default int handler used to catch unregistered interrupts
static void i86_default_handler ();

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

//! installs idtr into processors idtr register
static void idt_install () {
  __asm__ __volatile__("lidt (%0)" : : "r" (&_idtr));
}


//! default handler to catch unhandled system interrupts.
static void  i86_default_handler () {
	for(;;);
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

//! returns interrupt descriptor
struct idt_descriptor* i86_get_ir (uint32_t i) {

	if (i>I86_MAX_INTERRUPTS)
		return 0;

	return &_idt[i];
}


//! installs a new interrupt handler
int i86_install_ir (uint32_t i, uint16_t flags, uint16_t sel, I86_IRQ_HANDLER irq) {

	if (i>I86_MAX_INTERRUPTS)
		return 0;

	if (!irq)
		return 0;

	//! get base address of interrupt handler
	uint64_t		uiBase = (uint64_t)&(*irq);

	//! store base address into idt
	_idt[i].baseLo		=	(uint16_t)(uiBase & 0xffff);
	_idt[i].baseHi		=	(uint16_t)((uiBase >> 16) & 0xffff);
	_idt[i].reserved	=	0;
	_idt[i].flags		=	(uint8_t)(flags);
	_idt[i].sel			=	sel;

	return	0;
}


//! initialize idt
int i86_idt_initialize (uint16_t codeSel) {
    int i=0;
	//! set up idtr for processor
	_idtr.limit = sizeof (struct idt_descriptor) * I86_MAX_INTERRUPTS -1;
	_idtr.base	= (uint32_t)&_idt[0];

	//! null out the idt
	memset ((void*)&_idt[0], 0, sizeof (struct idt_descriptor) * I86_MAX_INTERRUPTS-1);

	//! register default handlers
	for ( i=0; i<I86_MAX_INTERRUPTS; i++)
		i86_install_ir (i, I86_IDT_DESC_PRESENT | I86_IDT_DESC_BIT32,
			codeSel, (I86_IRQ_HANDLER)i86_default_handler);

	//! install our idt
	idt_install ();

	return 0;
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[idt.cpp]
//**
//****************************************************************************

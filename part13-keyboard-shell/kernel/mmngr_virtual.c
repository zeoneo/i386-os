#include "common.h"
#include "mmngr_virtual.h"
#include "mmngr_phys.h"
#include "../drivers/printk.h"
#include "../cpu/idt.h"
#include "../cpu/panic.h"


//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================

//! page table represents 4mb address space
#define PTABLE_ADDR_SPACE_SIZE 0x400000

//! directory table represents 4gb address space
#define DTABLE_ADDR_SPACE_SIZE 0x100000000

//! page sizes are 4k
#define PAGE_SIZE 4096


typedef struct
{
  uint32_t ds;             // Data segment selector.
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
  uint32_t int_no, err_code; // Interrupt number and error code (if applicable).
  uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

//! current directory table
struct pdirectory*		_cur_directory=0;

//! current page directory base register
physical_addr	_cur_pdbr=0;


inline pt_entry* vmmngr_ptable_lookup_entry (struct ptable* p,virtual_addr addr) {

	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}

inline pd_entry* vmmngr_pdirectory_lookup_entry (struct pdirectory* p, virtual_addr addr) {

	if (p)
		return &p->m_entries[ PAGE_TABLE_INDEX (addr) ];
	return 0;
}

inline void paging_enable () {
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0" : "=r" (cr0));
  	cr0 |= 0x80000000;
  	asm volatile ("mov %0, %%cr0" : : "r" (cr0));
}

inline int pmmngr_is_paging () {
	uint32_t res=0;
	asm volatile ("mov %%cr0, %0" : "=r" (res));
	return (res & 0x80000000) ? 0 : 1;
}

physical_addr pmmngr_get_PDBR () {
    uint32_t res=0;
	asm volatile ("mov %%cr3, %0" : "=r" (res));
    return res;
}

inline int vmmngr_switch_pdirectory (struct pdirectory* dir) {

	if (!dir)
		return 0;

	_cur_directory = dir;
    asm volatile ("mov %0, %%cr3" : : "r" (dir));

	return 1;
}

void vmmngr_flush_tlb_entry (virtual_addr addr) {
  // Inform the CPU that we have invalidated a page mapping.
  asm volatile("cli");
  asm volatile ("invlpg (%0)" : : "a" (addr));
  asm volatile("sti");
}

struct pdirectory* vmmngr_get_directory () {

	return _cur_directory;
}

int vmmngr_alloc_page (pt_entry* e) {

	//! allocate a free physical frame
	void* p = pmmngr_alloc_block ();
	if (!p)
		return 0;

	//! map it to the page
	pt_entry_set_frame (e, (physical_addr)p);
	pt_entry_add_attrib (e, I86_PTE_PRESENT);
	//doesent set WRITE flag...

	return 1;
}

void vmmngr_free_page (pt_entry* e) {

	void* p = (void*)pt_entry_pfn (*e);
	if (p)
		pmmngr_free_block (p);

	pt_entry_del_attrib (e, I86_PTE_PRESENT);
}

void vmmngr_map_page (void* phys, void* virt) {

   //! get page directory
   struct pdirectory* pageDirectory = vmmngr_get_directory ();

   //! get page table
   pd_entry* e = &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ((uint32_t) virt) ];
   if ( (*e & I86_PTE_PRESENT) != I86_PTE_PRESENT) {

      //! page table not present, allocate it
      struct ptable* table = (struct ptable*) pmmngr_alloc_block ();
      if (!table)
         return;

      //! clear page table
      memset (table, 0, sizeof(struct ptable));

      //! create a new entry
      pd_entry* entry =
         &pageDirectory->m_entries [PAGE_DIRECTORY_INDEX ( (uint32_t) virt) ];

      //! map in the table (Can also just do *entry |= 3) to enable these bits
      pd_entry_add_attrib (entry, I86_PDE_PRESENT);
      pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
      pd_entry_set_frame (entry, (physical_addr)table);
   }

   //! get table
   struct ptable* table = (struct ptable*) PAGE_GET_PHYSICAL_ADDRESS ( e );

   //! get page
   pt_entry* page = &table->m_entries [ PAGE_TABLE_INDEX ( (uint32_t) virt) ];

   //! map it in (Can also do (*page |= 3 to enable..)
   pt_entry_set_frame ( page, (physical_addr) phys);
   pt_entry_add_attrib ( page, I86_PTE_PRESENT);
}

void page_fault (registers_t regs)
{
  uint32_t cr2;
  asm volatile ("mov %%cr2, %0" : "=r" (cr2));

   int present   = !(regs.err_code & 0x1); // Page not present
   int rw = regs.err_code & 0x2;           // Write operation?
   int us = regs.err_code & 0x4;           // Processor was in user-mode?
   int reserved = regs.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
   int id = regs.err_code & 0x10;  

    printk ("Page fault at 0x%x, faulting address 0x%x\n", regs.eip, cr2);
    printk ("Error code: %x", regs.err_code);
  
    if (present) {printk(" present ");}
    if (rw) {printk(" read-only ");}
    if (us) {printk(" user-mode ");}
    if (reserved) {printk(" reserved ");}

    printk("\n");
    panic ("");
  for (;;) ;
}


void vmmngr_initialize () {
    int i;
    int frame;
    int virt;
   //! allocate default page table
   struct ptable* table = (struct ptable*) pmmngr_alloc_block ();
   if (!table)
      return;

   //! allocates 3gb page table
   struct ptable* table2 = (struct ptable*) pmmngr_alloc_block ();
   if (!table2)
      return;

   //! clear page table
   memset (table, 0, sizeof (struct ptable));

   //! 1st 4mb are idenitity mapped
   for (i=0, frame=0x0, virt=0x00000000; i<1024; i++, frame+=4096, virt+=4096) {

      //! create a new page
      pt_entry page=0;
      pt_entry_add_attrib (&page, I86_PTE_PRESENT);
      pt_entry_set_frame (&page, frame);

      //! ...and add it to the page table
      table2->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
   }

   //! map 1mb to 3gb (where we are at)
   for (i=0, frame=0x100000, virt=0xc0000000; i<1024; i++, frame+=4096, virt+=4096) {

      //! create a new page
      pt_entry page=0;
      pt_entry_add_attrib (&page, I86_PTE_PRESENT);
      pt_entry_set_frame (&page, frame);

      //! ...and add it to the page table
      table->m_entries [PAGE_TABLE_INDEX (virt) ] = page;
   }

   //! create default directory table
   struct pdirectory*   dir = (struct pdirectory*) pmmngr_alloc_blocks (3);
   if (!dir)
      return;

  //! clear directory table and set it as current
  memset (dir, 0, sizeof (struct pdirectory));

   //! get first entry in dir table and set it up to point to our table
   pd_entry* entry = &dir->m_entries [PAGE_DIRECTORY_INDEX (0xc0000000) ];
   pd_entry_add_attrib (entry, I86_PDE_PRESENT);
   pd_entry_add_attrib (entry, I86_PDE_WRITABLE);
   pd_entry_set_frame (entry, (physical_addr)table);

   pd_entry* entry2 = &dir->m_entries [PAGE_DIRECTORY_INDEX (0x00000000) ];
   pd_entry_add_attrib (entry2, I86_PDE_PRESENT);
   pd_entry_add_attrib (entry2, I86_PDE_WRITABLE);
   pd_entry_set_frame (entry2, (physical_addr)table2);

   //! store current PDBR
   _cur_pdbr = (physical_addr) &dir->m_entries;

    //Register Page Fault Handler
    set_idt_gate(14, &page_fault);

   //! switch to our page directory
   vmmngr_switch_pdirectory (dir);

   //! enable paging
   paging_enable();
}


//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[mmngr_virtual.cpp]
//**
//****************************************************************************

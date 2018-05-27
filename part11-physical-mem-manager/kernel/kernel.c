
#include "../drivers/vga.h"
#include "../drivers/screen.h"
#include "bootinfo.h"
#include "debug_display.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

//! format of a memory region
struct memory_region {
	uint32_t	startLo;
	uint32_t	startHi;
	uint32_t	sizeLo;
	uint32_t	sizeHi;
	uint32_t	type;
	uint32_t	acpi_3_0;
};

//! different memory regions (in memory_region.type)
char* strMemoryTypes[] = {
	"Available",			//memory_region.type==0
	"Reserved",			//memory_region.type==1
	"ACPI Reclaim",		//memory_region.type==2
	"ACPI NVS Memory"		//memory_region.type==3
};

void dummy_test_entrypoint()
{
}

void main(struct multiboot_info * bootinfo)
{
    uint32_t kernelSize=0;
    uint32_t memSize = 1024 + bootinfo->m_memoryLo + bootinfo->m_memoryHi*64;
    int i;

    DebugClrScr (0x13);
	DebugGotoXY (0,0);
	DebugSetColor (0x3F);
	DebugPrintf ("                    ~[ Physical Memory Manager Demo ]~                          ");
	DebugGotoXY (0,24);
	DebugSetColor (0x3F);
	DebugPrintf ("                                                                                ");
	DebugGotoXY (0,2);
	DebugSetColor (0x17);

    DebugPrintf("boo_info memSize:%i mem_lo:%i mem_hi:%i ",memSize, bootinfo->m_memoryLo, bootinfo->m_memoryHi);

    //DebugPrintf("pmm initialized with KB physical memory; memLo: %i memHi: %i\n\n",
		// bootinfo->m_memoryLo,bootinfo->m_memoryHi);

    while(1);

    isr_install();

    asm volatile("sti");
    init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    init_keyboard();

    kprint("-------Physical Memory Manager Demo-------");
    
     __asm__("mov %%cx, %%dx" : : "c" (kernelSize));


    	//! get memory size in KB


	//! initialize the physical memory manager
	//! we place the memory bit map used by the PMM at the end of the kernel in memory
	pmmngr_init (memSize, 0x100000 + kernelSize*512);

  
    kprint("pmm initialized with %i KB physical memory; memLo: %i memHi: %i\n\n");
	kprint ("Physical Memory Map:\n");

	struct memory_region *region = (struct memory_region*)0x1000;

	for (i=0; i<15; ++i) {

		//! sanity check; if type is > 4 mark it reserved
		if (region[i].type>4)
			region[i].type=1;

		//! if start address is 0, there is no more entries, break out
		if (i>0 && region[i].startLo==0)
			break;

		//! display entry
		kprint ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %i (%s)\n");
        // , i, 
		// 	region[i].startHi, region[i].startLo,
		// 	region[i].sizeHi,region[i].sizeLo,
		// 	region[i].type, strMemoryTypes[region[i].type-1]);

		//! if region is avilable memory, initialize the region for use
		if (region[i].type==1)
			pmmngr_init_region (region[i].startLo, region[i].sizeLo);
	}

	//! deinit the region the kernel is in as its in use
	pmmngr_deinit_region (0x100000, kernelSize*512);

	// DebugSetColor (0x17);

	kprint("\npmm regions initialized: %i allocation blocks; used or reserved blocks: %i\nfree blocks: %i\n");
    // ,
		// pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );

	//! allocating and deallocating memory examples...

	// DebugSetColor (0x12);

	uint32_t* p = (uint32_t*)pmmngr_alloc_block ();
	kprint("\np allocated at 0x%x");

	uint32_t* p2 = (uint32_t*)pmmngr_alloc_blocks (2);
	kprint("\nallocated 2 blocks for p2 at 0x%x");

	pmmngr_free_block (p);
	p = (uint32_t*)pmmngr_alloc_block ();
	kprint("\nUnallocated p to free block 1. p is reallocated to 0x%x");

	pmmngr_free_block (p);
	pmmngr_free_blocks (p2, 2);


    while (1); // never execute code below. Just for linking large kernel


    vgaGraphicsModeInit();
    fillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8); //fill Screen with blue pixels
}

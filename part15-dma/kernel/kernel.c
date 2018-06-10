#include "bootinfo.h"
#include "../cpu/hal.h"
#include "../drivers/kybrd.h"
#include "../drivers/flpydsk.h"
#include "../drivers/printk.h"
#include "../drivers/screen.h"
#include "shell.h"


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

void dummy_test_entrypoint() {}

extern void* kernel_end;

void init(struct multiboot_info * bootinfo)
{
    uint32_t kernelSize=0;
    asm volatile (
        ""
        : "=d" (kernelSize)
        // no inputs, no clobbers
    );
    uint32_t memSize = 1024 + bootinfo->m_memoryLo + bootinfo->m_memoryHi*64;
    int i;


	
    clear_screen_with_color (0x13);
	goto_xy (0,0);
	printk ("Zeus is loading...\n");
	goto_xy (0,24);
	screen_set_color (0x3F);
	printk ("                                                                                ");
	goto_xy (0,2);
	screen_set_color (0x17);

    printk("pmm initialized with %d KB physical memory; memLo: %d memHi: %d\n\n", memSize, bootinfo->m_memoryLo,bootinfo->m_memoryHi);
	
	disable_interrupts();
	vmmngr_initialize ();
	hal_initialize();
	enable_interrupts();

    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */

    printk("kernel size %d kernel_end:%x", kernelSize, &kernel_end);
    	// ! get memory size in KB
  

	//! initialize the physical memory manager
	//! we place the memory bit map used by the PMM at the end of the kernel in memory
	pmmngr_init (memSize, &kernel_end);

	screen_set_color (0x19);
	printk ("Physical Memory Map:\n");

	struct memory_region*	region = (struct memory_region*)0x1000;

	for (i=0; i<15; ++i) {

		//! sanity check; if type is > 4 mark it reserved
		if (region[i].type>4)
			region[i].type=1;

		//! if start address is 0, there is no more entries, break out
		if (i>0 && region[i].startLo==0)
			break;

		// ! display entry
		printk ("region %i: start: 0x%x%x length (bytes): 0x%x%x type: %d Name:%s \n", i, 
			region[i].startHi, region[i].startLo,
			region[i].sizeHi,region[i].sizeLo,
			region[i].type, strMemoryTypes[region[i].type-1]);

		//! if region is avilable memory, initialize the region for use
		if (region[i].type==1)
			pmmngr_init_region (region[i].startLo, region[i].sizeLo);
	}

	//! deinit the region the kernel is in as its in use
	pmmngr_deinit_region (0x100000, kernelSize*512);

	screen_set_color (0x17);

	printk("\npmm regions initialized: %d allocation blocks; used or reserved blocks: %d \nfree blocks: %d\n",
		pmmngr_get_block_count (),  pmmngr_get_use_block_count (), pmmngr_get_free_block_count () );

	//! allocating and deallocating memory examples...

	screen_set_color (0x12);

	//! initialize our vmm


	kkybrd_install(33);

	flpydsk_set_working_drive (0);

	printk("Chose floppy drive");
	//! install floppy disk to IR 38, uses IRQ 6
	flpydsk_install ();

	// set dma buffer to 64k
	flpydsk_set_dma (0x8000);
	// printk("Install flpy irq");
}

void readSector() {
	uint8_t* sector = flpydsk_read_sector (0);

	if (sector!=0) {

		int i = 0;
		int c, j;
		for ( c = 0; c < 4; c++ ) {
			for (j = 0; j < 128; j++)
				printk ("0x%x ", sector[ i + j ]);
			i += 128;
		}
	} else {
		printk("Failed to read sector");
	}
}

void main(struct multiboot_info * bootinfo) {
	init (bootinfo);

	clear_screen_with_color (0x13);
	goto_xy (0,0);
	printk ("\nType \"exit\" to quit, \"help\" for a list of commands\n");
	printk ("+-------------------------------------------------------+\n");
	// run ();
	// printk ("\nExit command recieved; demo halted");
	readSector();
	
	for (;;);
	return 0;
}
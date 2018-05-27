#include "bootinfo.h"
#include "debug_display.h"
#include "../cpu/hal.h"

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
    DebugClrScr (0x18);
	DebugGotoXY (0,0);
	DebugSetColor (0x70);
	DebugPrintf (" Microcomputer Operating System (MOS) Preparing to load...                       ");
	DebugGotoXY (0,1);
	DebugSetColor (0x19);
	DebugPrintf (" MOS Starting Up...\n");

	DebugSetColor (0x70);
	DebugGotoXY (0,24);
	DebugPrintf (" Initializing Hardware Abstraction Layer (HAL.lib)...                           ");

	DebugSetColor (0x19);
	DebugGotoXY (0,2);

	hal_initialize ();

//! uncomment to generate interrupt 0x15. This will call the default exception handler
	geninterrupt (0x15);

	return 0;
}

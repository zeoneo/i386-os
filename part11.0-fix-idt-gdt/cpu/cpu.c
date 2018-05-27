
#include "cpu.h"
#include "gdt.h"
#include "idt.h"


//! Initializes cpu resources
int i86_cpu_initialize () {

	//! initialize processor tables
	i86_gdt_initialize ();
	i86_idt_initialize (0x8);

	return 0;
}

//! shuts down cpu resources...Nothing to do yet
void i86_cpu_shutdown () {


}

#include "cpu.h"
#include "gdt.h"

int i86_cpu_initialize () {

	//! initialize processor tables
	i86_gdt_initialize();

	return 0;
}
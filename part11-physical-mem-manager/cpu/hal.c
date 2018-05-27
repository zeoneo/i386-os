#include "hal.h"

void hal_initialize() {
	//! initialize motherboard controllers and system timer
	i86_cpu_initialize ();
}

void disable() {
    asm volatile("cli");
}

void enable() {
    asm volatile("sti");
}
#include "hal.h"
#include "pic.h"

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

inline void interruptdone (unsigned int intno) {

	//! insure its a valid hardware irq
	if (intno > 16)
		return;

	//! test if we need to send end-of-interrupt to second pic
	if (intno >= 8)
		i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 1);

	//! always send end-of-interrupt to primary pic
	i86_pic_send_command (I86_PIC_OCW2_MASK_EOI, 0);
}
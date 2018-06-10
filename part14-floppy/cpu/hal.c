#include "hal.h"
#include "gdt.h"
#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "pic.h"

void hal_initialize() {
	//! initialize motherboard controllers and system timer
	disable_interrupts();
	i86_gdt_initialize();
	i86_idt_initialize(0x8);
	i86_pic_initialize(0x20,0x28);
	init_timer(100);
	isr_install();
	enable_interrupts();
}

void disable_interrupts() {
    asm volatile("cli");
}

void enable_interrupts() {
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
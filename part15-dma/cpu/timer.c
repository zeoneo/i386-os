#include "./isr.h"
#include "./timer.h"
#include "hal.h"
#include "idt.h"
#include "../drivers/ports.h"
#include "../drivers/printk.h"
#include "../drivers/screen.h"
#include "../libc/function.h"

volatile u32 tick = 0;
static unsigned char y, x;
static void timer_callback(registers_t regs)
{
    tick++;
    // if(tick < 5) 
    
	get_cursor_xy(&x, &y);
    set_cursor_xy(0, 0);
    printk("Timer ticks : %d \r", tick);
    set_cursor_xy(x, y);
    UNUSED(regs);

    interruptdone(0); // its interrupt request 0
}

void init_timer(u32 freq)
{
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low = (u8)(divisor & 0xFF);
    u8 high = (u8)((divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}


unsigned int get_timer_ticks() {
    return tick;
}

void sleep (unsigned int ms) {
	unsigned int ticks = ms + get_timer_ticks();
     printk("\n Entering sleep at: %d.", get_timer_ticks());
	while (ticks > get_timer_ticks ());
    printk("\n Exiting sleep at: %d.", get_timer_ticks());
}
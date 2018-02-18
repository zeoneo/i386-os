
#include "../drivers/vga.h"
#include "../drivers/screen.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/paging.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void dummy_test_entrypoint()
{
}

void main()
{
    clear_screen();
    kprint("Hello ZERO ...!");
    isr_install();
    asm volatile("sti");
    init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
    init_keyboard();
    paging_init();
    kprint("Welcome to paging ...!");
    while (1)
        ; // never execute code below. Just for linking large kernel
    vgaGraphicsModeInit();
    fillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8); //fill Screen with blue pixels
}

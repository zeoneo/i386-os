
#include "../drivers/vga.h"
#include "../drivers/screen.h"

void dummy_test_entrypoint()
{
}

void main()
{
    clear_screen();
    kprint("Hello World ...!");
    kprint("This should print in new line ...!");

    kprint_at("X", 1, 6);
    kprint_at("This text spans multiple lines", 75, 10);
    kprint_at("There is a line\nbreak", 0, 20);
    kprint("There is a line\nbreak");
    kprint_at("What happens when we run out of space?", 45, 24);

    while (1)
        ;
    vgaGraphicsModeInit();
    fillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8); //fill Screen with blue pixels
}

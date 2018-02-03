
#include "../drivers/vga.h"

void dummy_test_entrypoint()
{
}

void main()
{
    vgaGraphicsModeInit();
    fillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8); //fill Screen with blue pixels
}

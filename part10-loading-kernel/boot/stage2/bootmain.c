#include "../../drivers/screen.h"

void bootmain()
{
    clear_screen();
    kprint("Hello World I am in boot main");
    while (1)
        ;
}
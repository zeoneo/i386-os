#include "../drivers/screen.h"

void dummy_test_entrypoint()
{
}

void main()
{
    // clear_screen();
    kprint("This is kernel .Hello World ...!");
    kprint("This should print in new line ...!");
}

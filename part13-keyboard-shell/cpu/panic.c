#include "panic.h"
#include "../drivers/printk.h"

void panic (const char *msg)
{
  printk ("*** System panic: %s\n", msg);
  printk ("***\n");
  for (;;) ;
}
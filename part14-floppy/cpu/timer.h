#ifndef TIMER_H
#define TIMER_H

#include "../libc/string.h"

void init_timer(unsigned int freq);
unsigned int get_timer_ticks();
#endif

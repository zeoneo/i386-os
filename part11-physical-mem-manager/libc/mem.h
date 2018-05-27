#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memory_copy(u8 *source, u8 *dest, int nbytes);
void memory_set(u8 *dest, u8 val, u32 len);
void* memcpy(void *dest, const void *src, size_t count);
void* memset(void *dest, char val, size_t count);

#endif

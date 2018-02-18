#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

static uint32_t* page_directory = 0;
static uint32_t page_dir_loc = 0;
static uint32_t* last_page = 0;

void paging_init();
void paging_enable();
void paging_map_virtual_to_phys(uint32_t virt, uint32_t phys);
void * get_physaddr(void * virtualaddr);
static inline void __native_flush_tlb_single(unsigned long addr);

#endif
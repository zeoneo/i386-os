Paging

Changes.: stage2.s calls enable paging.s before copying kernel to Protected mode address

In paging.s
    Store Page directory at some address < 4MB which we are going to initialize identity mapping
    PAGE_DIR equ 0x9C000

    ; 0th page table. Address must be 4KB aligned. address should be < 4MB
    PAGE_TABLE_0 equ 0x9D000

    ; 768th page table. Address must be 4KB aligned. address should be < 4MB
    PAGE_TABLE_768  equ 0x9E000

    Initialize Page Table :
            1. It stores 1024 entries in Page Table 0.
            Eg for entry 0: physical frame address = 0x0
                for entry 1: physical frame address = 0x1000 (4k aligned)
            2. Initialize Page 768. Why 768 ?? because it's a first page starting from 3GB i.e 0xC0000000
                Start entry 0 at physical address = 0x100000 i.e. 1 MB and virtual address 3GB i.e. 0xC0000000
                So strategy is why linking your kernel link it at 0xC0000000 and while loading it load it at 0x100000 i.e. 1MB
                Now load these page table 0 and page table 768 in page directory. load page directory using asm commands. Enable paging.
                When we start copy our loaded kernel from 0x7E00 to 0xC0000000 paging kicks in as follows.
                    1. 0x7E00 is treated as 0x7E00 because we initialized page table 0 like that . (identity mapping)
                    2. 0xC0000000 is treated as 0x100000 as we already initialized it using page table 768

                hence when we do jump from stage2.s to virtual address 0xC0000000 (kernel instructions start from 1MB refer link)
                as soon as we jump to virtual address 0xC0000000 memory it jumps to Physical address at 0x100000.

            Things to note we do not need to change the video buffer address because we have identity mapped whole area upto 4MB
            as VIDEO MEMORY ADDR 0xB8000 < 4MB

            3. After kernel start executing we allocated new blocks for page table 0 and page table 768 and mark those frames as used so nobody touches them
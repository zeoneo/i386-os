This is first step towards OS development.

I am using macOS X High Seirra as Host system.

I have downloaded pre-built binaries of cross-compiler required for development
Compiler Name: i386-elf-gcc

All the files contain in 
```
i386-elf-gcc/bin
i386-elf-c++            i386-elf-g++            i386-elf-gcc-5.2.0      i386-elf-gcc-nm         i386-elf-gcov
i386-elf-cpp            i386-elf-gcc            i386-elf-gcc-ar         i386-elf-gcc-ranlib     i386-elf-gcov-tool
```

Also we will require NASM for assembling bootloader code which is written in assembly.

Theory Related to following point is covered in os-dev.pdf in this direcotry which is downloaded from  3rd link provided below.
1. BIOS
2. Boot Sector
3. Boot Sequence
4. X86 Assembly Coding
5. VGA Text Mode
6. Processor Real Mode and Protected Mode.
7. GDT



I have been reading alot on os development and I am providing link of all the websites here.

X86 Related
1. http://wiki.osdev.org/Main_Page
2. http://www.brokenthorn.com/Resources/OSDevIndex.html
3. https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf

Raspberry Pi Related.

4. http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1/
5. https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/
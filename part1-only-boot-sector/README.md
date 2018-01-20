This is simple boot sector code. First of all for valid boot sector must end with boot sector signature 0xAA55 (Hex)

When X86 processor boots it boots into 16 bit Real Mode.
Why?
Because nowdays X86 family has advanced processors 32bit or 64 bit etc just to maintain backward compatibility all the X86 family processors boot in 16 bit mode which is also called as REAL Mode.

Now if you are writing OS for processor which supports 32bit or 64 bits then you need to switch over to PROTECTED MODE which supports 32bit or 64 bit operations.

So in REAL Mode (16 bit) you have BIOS routine available
int 0x10 invokes bios routing which is related screen.
ah = 0x0e select print routine . It prints character present in
al register.

BIOS knows how to print character, it provides fonts etc.
If you give character 'A' then it knows how to draw pixels on screen so that it looks like A to user.


To compile code use 
```
make all
``` 

To test this code on qemu use
```
make run
```
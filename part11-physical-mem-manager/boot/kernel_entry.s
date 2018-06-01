[bits 32]
[extern main];
[GLOBAL kernel_start]
kernel_start:
call main;
jmp $
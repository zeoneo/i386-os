[bits 32]
[extern main];
call main; 
TIMES 1024 * 1024 DB 0
jmp $
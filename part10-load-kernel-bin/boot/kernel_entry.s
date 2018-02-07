[bits 32]
[extern main];
call main;
TIMES 1024 DB 12
TIMES 1024 DB 12
TIMES 1024 DB 12
TIMES 1024 DB 12
DB 13

jmp $
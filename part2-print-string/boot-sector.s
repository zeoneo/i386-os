[org 0x7c00]
; A simple boot sector that prints a message to the screen using a BIOS routine. ;
; Boot sector code should start here. include other asm files here will execute that code first
mov bp, 0x8000;
mov sp, bp;
mov bx, my_string ;
call print_string;
jmp $ ;Jump to the current address (i.e. forever).
;

; Include external files here ; It simply copies asse,bly code here
%include "print_string.s" 

; Data declaration here
my_string:
    db 'Hello world booting in 16 bit mode', 0 ; Null terminated string

; Padding and magic BIOS number. ;
times 510-($-$$) db 0 ; Pad the boot sector out with zeros
dw 0xaa55 
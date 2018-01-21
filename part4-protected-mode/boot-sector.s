[org 0x7c00]
; A simple boot sector that prints a message to the screen using a BIOS routine. ;
; Boot sector code should start here. include other asm files here will execute that code first
mov bp, 0x8000; Initialize Stack Base Pointer to 0x8000
mov sp, bp; ; Point current stack pointer to top. Stack pointer will decrease from BP to downwords

mov bx, REAL_MODE_MSG ;
call print_string;

call switch_to_pm

jmp $ ;Jump to the current address (i.e. forever).
;

; Include external files here ; It simply copies asse,bly code here
%include "print_string.s"
%include "print_hex.s"
%include "disk_load.s"
%include "gdt.s"
%include "pm_switch.s"
%include "print_string32.s"

[bits 32]

BEGIN_PM:

    mov ebx, PROTECTED_MODE_MSG;
    call print_string32;
    jmp $

; Global Variable
BOOT_DRIVE:
    db 0

; Data declaration here
REAL_MODE_MSG:
    db 'Hello world booting in 16 bit mode', 0 ; Null terminated string

PROTECTED_MODE_MSG:
    db 'Successfully Landed in 32 Bit mode', 0 ; Null terminated string

my_space:
    db ' ', 0 ; Null terminated string

; Padding and magic BIOS number. ;
times 510-($-$$) db 0 ; Pad the boot sector out with zeros
dw 0xaa55

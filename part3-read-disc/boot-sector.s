[org 0x7c00]
; A simple boot sector that prints a message to the screen using a BIOS routine. ;
; Boot sector code should start here. include other asm files here will execute that code first
mov bp, 0x8000; Initialize Stack Base Pointer to 0x8000
mov sp, bp; ; Point current stack pointer to top. Stack pointer will decrease from BP to downwords

mov bx, my_string ;
call print_string;
call print_nl;

mov dx, 0x1234;
call print_hex;
call print_nl;

mov dx, 0x6789;
call print_hex;
call print_nl;

mov dx, 0xabcd;
call print_hex;
call print_nl;

mov dx, 0x00ef;
call print_hex;
call print_nl;

mov bx, 0x9000;
mov dh, 5;
mov dl, [BOOT_DRIVE];

call disk_load
mov dx, [0x9000]
call print_hex;
call print_nl;

mov dx, [0x9000 + 512]
call print_hex
jmp $ ;Jump to the current address (i.e. forever).
;

; Include external files here ; It simply copies asse,bly code here
%include "print_string.s"
%include "print_hex.s"
%include "disk_load.s"
%include "print_nl.s"

; Global Variable
BOOT_DRIVE:
    db 0

; Data declaration here
my_string:
    db 'Hello world booting in 16 bit mode', 0 ; Null terminated string
temp_msg:
    db 'This should not be printed', 0 ; Null terminated string

my_space:
    db ' ', 0 ; Null terminated string

; Padding and magic BIOS number. ;
times 510-($-$$) db 0 ; Pad the boot sector out with zeros
dw 0xaa55

times 256 dw 0xdada;
times 256 dw 0xface;
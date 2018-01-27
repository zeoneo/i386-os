[org 0x7c00]

KERNEL_OFFSET equ 0x1000

start:          jmp loader

; A simple boot sector that prints a message to the screen using a BIOS routine. ;
; Boot sector code should start here. include other asm files here will execute that code first

bpbOEM			db "My OS   "				; This member must be exactally 8 bytes. It is just
								; the name of your OS :) Everything else remains the same.

bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xF0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "

loader: mov [BOOT_DRIVE], dl;

mov bp, 0x9000; Initialize Stack Base Pointer to 0x8000
mov sp, bp; ; Point current stack pointer to top. Stack pointer will decrease from BP to downwords

mov bx, REAL_MODE_MSG ;
call print_string;
call print_nl;
call load_kernel;
jmp $
call switch_to_pm

jmp $ ;Jump to the current address (i.e. forever).
;

; Include external files here ; It simply copies asse,bly code here
%include "boot/print_string.s"
%include "boot/print_nl.s"
%include "boot/print_hex.s"
%include "boot/disk_load.s"
%include "boot/gdt.s"
%include "boot/pm_switch.s"
%include "boot/print_string32.s"

[bits 16]
load_kernel:
    mov bx, KERNEL_LOAD_MSG;
    call print_string;
    call print_nl;
    mov bx, KERNEL_OFFSET;

    mov dh, 15;
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret


[bits 32]
BEGIN_PM:

    ; mov ebx, PROTECTED_MODE_MSG;
    ; call print_string32;
    call KERNEL_OFFSET;
    jmp $

; Global Variable
BOOT_DRIVE: db 0
REAL_MODE_MSG: db 'Hello world booting in 16 bit mode', 0 ; Null terminated string
PROTECTED_MODE_MSG: db 'Successfully Landed in 32 Bit mode', 0 ; Null terminated string
KERNEL_LOAD_MSG: db 'Loading Kernel in memory', 0;
my_space: db ' ', 0 ; Null terminated string

; Padding and magic BIOS number. ;
times 510-($-$$) db 0 ; Pad the boot sector out with zeros
dw 0xaa55


; *********************************************
; Boot1.asm
; - A Simple Bootloader

; Operating Systems Development Tutorial
; *********************************************

bits	16                                                   ; we are in 16 bit real mode

org		0                                                    ; we will set regisers later

start:
    jmp	main                                              ; jump to start of bootloader
    nop

; *********************************************
; BIOS Parameter Block
; *********************************************

; BPB Begins 3 bytes from start. We do a far jump, which is 3 bytes in size.
; If you use a short jump, add a "nop" after it to offset the 3rd byte.

bpbOEM			db "My OS   "
bpbBytesPerSector:  	DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	DB 2
bpbRootEntries: 	DW 224
bpbTotalSectors: 	DW 2880
bpbMedia: 		DB 0xf0                                       ; ; 0xF1
bpbSectorsPerFAT: 	DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 		DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:	        DD 0xa0a1a2a3
bsVolumeLabel: 	        DB "MOS FLOPPY "
bsFileSystem: 	        DB "FAT12   "



main:
    ; ----------------------------------------------------
    ; code located at 0000:7C00, adjust segment registers
    ; ----------------------------------------------------
    cli                                             ; disable interrupts
    mov     ax, 0x07C0                              ; setup registers to point to our segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    ; ----------------------------------------------------
    ; create stack
    ; ----------------------------------------------------
    mov     ax, 0x0000                              ; set the stack
    mov     ss, ax
    mov     sp, 0xFFFF
    sti                                             ; restore interrupts
    mov  [BOOT_DRIVE], dl                           ; Save drive number from which booted from before
                                                    ; anything changes number in dl
    ; ----------------------------------------------------
    ; Printing Loading Message
    ; ----------------------------------------------------
    mov si, REAL_MODE_MSG
    call Print
    mov     si, msgCRLF
    call    Print
    ; ----------------------------------------------------
    ; Try to load STAGE 2 BOOTLOADER
    ; ----------------------------------------------------

    call loadRootDirectoryTable;

    jmp $

%include "boot/stage1/print16.s"
%include "boot/stage1/fat12.s"

; VARIABLES DECLARATION
absoluteSector db 0x00
absoluteHead   db 0x00
absoluteTrack  db 0x00
datasector  dw 0x0000
cluster     dw 0x0000
ImageName   db "STAGE2  BIN"
BOOT_DRIVE db 0
REAL_MODE_MSG db 'Hello world booting in 16 bit mode', 0 ; Null terminated string           ; Null terminated string
msgCRLF     db 0x0D, 0x0A, 0x00
msgProgress db ".", 0x00
msgFailure  db 0x0D, 0x0A, "STAGE 2 Not found . Please Reboot", 0x0D, 0x0A, 0x00
     

TIMES 510-($-$$) DB 0
DW 0xAA55

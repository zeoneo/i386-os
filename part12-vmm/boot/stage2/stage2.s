
;*******************************************************
;
;	Stage2.asm
;		Stage2 Bootloader
;
;	OS Development Series
;*******************************************************

[bits 16]
[org 0x500]

jmp	main				; go to start
;*******************************************************
;	Preprocessor directives
;*******************************************************

%include "boot/stage2/print16.s"
%include "boot/stage2/print32.s"
%include "boot/stage2/floppy16_driver.s"
%include "boot/stage2/fat12.s"
%include "boot/stage2/gdt.s"
%include "boot/stage2/a20.s"
%include "boot/stage2/bootinfo.s"
%include "boot/stage2/mmap.s"

;*******************************************************
;	Data Section
;*******************************************************

msgFailure db 0x0D, 0x0A, "Failed", 0x00
welcomeMessage db 0x0D, 0x0A, "Landed in STAGE TWO...", 0x00
enableA20Msg db 0x0D, 0x0A, "Enable A20 Installed GDT", 0x00
ImageName     db "KRNL32  BIN"
ImageSize     db 0

IMAGE_RMODE_BASE equ 0x7E00

boot_info:
istruc multiboot_info
	at multiboot_info.flags,			dd 0
	at multiboot_info.memoryLo,			dd 0
	at multiboot_info.memoryHi,			dd 0
	at multiboot_info.bootDevice,		dd 0
	at multiboot_info.cmdLine,			dd 0
	at multiboot_info.mods_count,		dd 0
	at multiboot_info.mods_addr,		dd 0
	at multiboot_info.syms0,			dd 0
	at multiboot_info.syms1,			dd 0
	at multiboot_info.syms2,			dd 0
	at multiboot_info.mmap_length,		dd 0
	at multiboot_info.mmap_addr,		dd 0
	at multiboot_info.drives_length,	dd 0
	at multiboot_info.drives_addr,		dd 0
	at multiboot_info.config_table,		dd 0
	at multiboot_info.bootloader_name,	dd 0
	at multiboot_info.apm_table,		dd 0
	at multiboot_info.vbe_control_info,	dd 0
	at multiboot_info.vbe_mode_info,	dw 0
	at multiboot_info.vbe_interface_seg,dw 0
	at multiboot_info.vbe_interface_off,dw 0
	at multiboot_info.vbe_interface_len,dw 0
iend

main:
	;-------------------------------;
	;   Setup segments and stack	;
	;-------------------------------;

	cli	                   ; clear interrupts
	xor		ax, ax             ; null segments
	mov		ds, ax
	mov		es, ax
	mov		ax, 0x0000         ; stack begins at 0x9000-0xffff
	mov		ss, ax
	mov		sp, 0xFFFF
	sti	                   ; enable interrupts

	mov si, welcomeMessage
    call Print16

	mov     [boot_info+multiboot_info.bootDevice], dl ; save boot device in multibootInfo

	call	_EnableA20
	call	InstallGDT
	sti
	mov si, enableA20Msg
    call Print16

	xor		eax, eax
	xor		ebx, ebx
	call	BiosGetMemorySize64MB
	mov		word [boot_info+multiboot_info.memoryHi], bx
	mov		word [boot_info+multiboot_info.memoryLo], ax

	mov		eax, 0x0
	mov		ds, ax
	mov		di, 0x1000
	call	BiosGetMemoryMap

	call	LoadRoot
   	mov    	ebx, 0
   	mov		ebp, IMAGE_RMODE_BASE
   	mov 	esi, ImageName
	call	LoadFile		; load our file
   	mov   	dword [ImageSize], ecx
	cmp		ax, 0
	je		EnterStage3
	mov		si, msgFailure
	call   	Print16
	mov		ah, 0
	int     0x16                    ; await keypress
	int     0x19                    ; warm boot computer

	jmp $;

EnterStage3:

	cli	                           ; clear interrupts
	mov	eax, cr0                   ; set bit 0 in cr0--enter pmode
	or	eax, 1
	mov	cr0, eax

	jmp	CODE_DESC:Stage3                ; far jump to fix CS. Remember that the code selector is 0x8!

	; Note: Do NOT re-enable interrupts! Doing so will triple fault!
	; We will fix this in Stage 3.

;******************************************************
;	ENTRY POINT FOR STAGE 3
;******************************************************

[bits 32]
%include "boot/stage2/paging.s"

BadImage db "*** FATAL: Invalid or corrupt kernel image. Halting system.", 0

Stage3:

	;-------------------------------;
	;   Set registers				;
	;-------------------------------;

	mov	ax, DATA_DESC		; set data segments to data selector (0x10)
	mov	ds, ax
	mov	ss, ax
	mov	es, ax
	mov	esp, 90000h		; stack begins from 90000h

	; call	ClrScr32
	call	EnablePaging

CopyImage:
  	 mov	eax, dword [ImageSize]
  	 movzx	ebx, word [bpbBytesPerSector]
  	 mul	ebx
  	 mov	ebx, 4
	 mov 	edx, 0x0000
  	 div	ebx
   	 cld
   	 mov    esi, IMAGE_RMODE_BASE
   	 mov	edi, IMAGE_PMODE_BASE
   	 mov	ecx, eax
   	 rep	movsd                   ; copy image to its protected mode address
	 mov	eax, 0x2BADB002		; multiboot specs say eax should be this
	 mov	ebx, 0
	 mov edx, [ImageSize]
	 push	dword boot_info

	 mov eax, 0x10  ; Load data segment selector for KM Data Segment descriptor
  	mov ds, eax     ; Load data segment selector registers
 	mov  es, eax
  	mov fs, eax
  	mov gs, eax
  	mov ss, eax
	jmp IMAGE_PMODE_BASE

    cli
	hlt
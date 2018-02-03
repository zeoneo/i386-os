
;*******************************************************
;
;	Stage2.asm
;		Stage2 Bootloader
;
;	OS Development Series
;*******************************************************

[bits 16]
[extern bootmain]
start:jmp	main				; go to start
;*******************************************************
;	Preprocessor directives
;*******************************************************

%include "boot/stage2/print16.s"
%include "boot/stage2/gdt.s"
%include "boot/stage2/a20.s"

;*******************************************************
;	Data Section
;*******************************************************

msgFailure db 0x0D, 0x0A, "Failed", 0x00
welcomeMessage db 0x0D, 0x0A, "Landed in STAGE TWO...", 0x00
enableA20Msg db 0x0D, 0x0A, "Enable A20 Installed GDT", 0x00
ImageName     db "KRNL32  BIN"
ImageSize     db 0



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

	call	_EnableA20
	call	InstallGDT
	sti
	mov si, enableA20Msg
    call Print16

	je		EnterStage3

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

	call bootmain


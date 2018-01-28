
;*******************************************************
;
;	Stage2.asm
;		Stage2 Bootloader
;
;	OS Development Series
;*******************************************************

bits	16

org 0x500

jmp	main				; go to start

;*******************************************************
;	Preprocessor directives
;*******************************************************

%include "boot/stage1/print16.s"

;*******************************************************
;	Data Section
;*******************************************************

LoadingMsg db 0x0D, 0x0A, "Landed in STAGE TWO...", 0x00

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

	mov si, LoadingMsg
    call Print
	jmp $;

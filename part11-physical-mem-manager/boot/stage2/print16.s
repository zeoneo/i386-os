[bits 16]

Print16:
    pusha; save registers
.NEXT_CHAR:
    lodsb				; load next byte from string from SI to AL
    or	al, al			; Does AL=0?
    jz	PRINT16_DONE		; Yep, null terminator found-bail out
    mov	ah, 0eh			; Nope-Print the character
    int	10h			; invoke BIOS
    jmp	.NEXT_CHAR			; Repeat until null terminator found
PRINT16_DONE:
    popa				; restore registers
    ret				; we are done, so return
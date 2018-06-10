Print:
	lodsb						; load next byte from string from SI to AL
	or	al, al					; Does AL=0?
	jz	PrintDone				; Yep, null terminator found-bail out
	mov	ah, 0eh					; Nope-Print the character
	int	10h
	jmp	Print					; Repeat until null terminator found
PrintDone:
	ret						; we are done, so return
print_string:
    pusha ; push all cpu registers on stacke
print_char:
    mov ah, 0x0e ; Selecte screen print BIOS ISR
    mov al, [bx] ;
    int 0x10; ; print the character on screen
    add bx, 0x01 ; increament bp register to point next char
    mov ax, [bx] ;
    cmp ax, 0x00 ;
    jne print_char
    popa ; pop all cpu register
    ret ; return from function


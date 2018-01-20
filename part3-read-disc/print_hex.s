print_hex:
    pusha ; push all cpu registers on stack
    mov al, '0';
    mov ah, 0x0e ; Selecte screen print BIOS ISR
    int 0x10;

    mov al, 'x';
    mov ah, 0x0e ; Selecte screen print BIOS ISR
    int 0x10;
    mov cl, 0x0c;
    mov bl, 0x04;
print_next:
    mov ax, dx;
    shr ax, cl;
    and al, 0x0f;
    cmp al, 0x09;
    jg greater_than_9
less_than_9:
    add al, 0x30;
    jmp print_digit
greater_than_9:
    add al, 0x37;
print_digit:
    mov ah, 0x0e ; Selecte screen print BIOS ISR
    int 0x10; ; print the character on screen
    dec bl;
    sub cl, 0x04;
    cmp bl, 0x00;
    jne print_next
    popa ; pop all cpu register
    ret ; return from function



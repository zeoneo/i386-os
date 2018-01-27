print_string:
    pusha
print_char:
    mov al, [bx] ; 'bx' is the base address for the string
    cmp al, 0 
    je done
print:
    mov ah, 0x0e
    int 0x10 ; 'al' already contains the char
    ; increment pointer and do next loop
    add bx, 1
    jmp print_char
done:
    popa
    ret

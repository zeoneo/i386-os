disk_load:
    push dx;

    mov ah, 0x02;
    mov al, dh;
    mov ch, 0x00;
    mov dh, 0x00;
    mov cl, 0x02;
    int 0x13;

    jc disk_error;

    pop dx;
    cmp dh, al;
    jne umatched_error;
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $

umatched_error:
    mov bx, SECTOR_READ_ERROR_MSG
    call print_string
    jmp $

; Variables
DISK_ERROR_MSG:
    db "DISK Read ERROR !", 0

SECTOR_READ_ERROR_MSG:
    db "Unmatched number of sector Read!", 0

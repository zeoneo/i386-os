[bits 32]

VIDEO_MEMORY equ 0xb8000;
WHITE_ON_BLACK equ 0x0f;

print_string32:
    pusha;
    mov edx, VIDEO_MEMORY;

print_string32_loop:
    mov al, [ebx];
    mov ah, WHITE_ON_BLACK;

    cmp al, 0x00;
    je done_print_string32;

    mov [edx], ax;

    add ebx, 1;
    add edx, 2;

    jmp print_string32_loop;

done_print_string32:
    popa;
    ret;
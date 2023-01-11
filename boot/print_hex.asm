print_hex:
    pusha
    mov si, HEX_OUT + 2 ; keep track of the current char in our template string
    mov cx, 0 ; counter how many nibbles processed, stop at 4

next_character:
    inc cx 

    mov bx, dx
    and bx, 0xf000 ; and bx, 1111000000000000 
    shr bx, 4
    add bh, 0x30

    ; if hex digit was > 9, it will be > 0x39, so add 7 to get ascii
    cmp bh, 0x39
    jg add_7

add_character_hex:
    mov [si], bh
    inc si
    shl dx, 4
    cmp cx, 4
    jnz next_character
    jmp _done

_done:
    mov bx, HEX_OUT
    call print_string
    popa
    ret

add_7:
    add bh, 0x7
    jmp add_character_hex

%include "print_string.asm" ; this will simply get replaced by the contents of the file

HEX_OUT:
    db '0x0000', 0

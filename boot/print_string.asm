; prints string of address in bx
print_string:
    pusha
    mov ah, 0x0e
    print_string_start:
        mov al, [bx]
        int 0x10
        add bx, 1
        cmp al, 0
        jne print_string_start
    popa
    ret

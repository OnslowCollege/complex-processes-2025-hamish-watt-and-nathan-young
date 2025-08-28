global _fillgradient

section .text

_fillgradient:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ebx

    sub esp, 52

    mov edi, [ebp+8] ; location in memory of the beginning of the colour array
    mov eax, [ebp+12] ; starting colour
    mov ebx, [ebp+16] ; ending colour
    mov ecx, [ebp+24] ; width of colour array in pixels

    ; store individual colours out of eax
    mov esi, eax
    and esi, 0xFF
    mov [esp], esi ; blue value

    mov esi, eax
    shr esi, 8
    and esi, 0xFF
    mov [esp+4], esi ; green value

    mov esi, eax
    shr esi, 16
    and esi, 0xFF
    mov [esp+8], esi ; red value

    ; get per loop increments for colours
    mov esi, ebx,
    and esi, 0xFF
    sub esi, [esp]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+12], eax ; blue increment
    mov [esp+24], edx ; blue remainder

    mov esi, ebx
    shr esi, 8
    and esi, 0xFF
    sub esi, [esp+4]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+16], eax ; green increment
    mov [esp+28], edx ; green remainder

    mov esi, ebx
    shr esi, 16
    and esi, 0xFF
    sub esi, [esp+8]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+20], eax ; red increment
    mov [esp+32], edx ; red remainder

    mov eax, [ebp+20]
    lea edx, [edi + eax]
    mov [esp+48], edx ; ending address of array

    ; accumulators
    mov DWORD [esp+36], 0
    mov DWORD [esp+40], 0
    mov DWORD [esp+44], 0

    mov eax, [ebp+12] ; starting colour

    shl eax, 8 ; put into little endian format (00BBGGRR)
    bswap eax

    mov ebx, [ebp+24]
    xor esi, esi
.fillgradient_loop:
    mov [edi], eax

    add edi, 4
    inc esi

    add al, BYTE [esp+12]
    add ah, BYTE [esp+16]
    mov ecx, eax
    shr ecx, 16
    add cl, BYTE [esp+20]
    shl ecx, 16
    mov cx, ax
    mov eax, ecx

    ; blue channel carry check
    mov ecx, [esp+36]
    add ecx, [esp+24]
    cmp ecx, [ebp+24]
    jl .no_blue_carry
    sub ecx, [ebp+24]
    add eax, 1
.no_blue_carry:
    mov [esp+36], ecx

    ; green channel carry check
    mov ecx, [esp+40]
    add ecx, [esp+28]
    cmp ecx, [ebp+24]
    jl .no_green_carry
    sub ecx, [ebp+24]
    add eax, 1 << 8
.no_green_carry:
    mov [esp+40], ecx

    ; red channel carry check
    mov ecx, [esp+44]
    add ecx, [esp+32]
    cmp ecx, [ebp+24]
    jl .no_red_carry
    sub ecx, [ebp+24]
    add eax, 1 << 16
.no_red_carry:
    mov [esp+44], ecx

    cmp esi, ebx
    jl .continue_loop

    mov eax, [ebp+12]
    xor esi, esi
.continue_loop:
    cmp edi, [esp+48]
    jb .fillgradient_loop
.finish:
    add esp, 52 

    pop ebx
    pop edi
    pop esi

    mov esp, ebp
    pop ebp

    ret

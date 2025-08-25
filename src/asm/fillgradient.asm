global _fillgradient

section .text

_fillgradient:
    push ebp
    mov ebp, esp

    push esi
    push edi
    push ebx

    sub esp, 0x18

    mov edi, [ebp+0x8] ; location in memory of the beginning of the colour array
    mov eax, [ebp+0xc] ; starting colour
    mov ebx, [ebp+0x10] ; ending colour
    mov ecx, [ebp+0x18] ; width of colour array in pixels

    mov esi, eax
    and esi, 0xFF
    mov [esp], esi ; blue value

    mov esi, eax
    shr esi, 0x8
    and esi, 0xFF
    mov [esp+0x4], esi ; green value

    mov esi, eax
    shr esi, 0x10
    and esi, 0xFF
    mov [esp+0x8], esi ; red value

    mov esi, ebx,
    and esi, 0xFF
    sub esi, [esp]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+0xc], eax

    mov esi, ebx
    shr esi, 0x8
    and esi, 0xFF
    sub esi, [esp+0x4]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+0x10], eax

    mov esi, ebx
    shr esi, 0x10
    and esi, 0xFF
    sub esi, [esp+0x8]
    xor edx, edx
    mov eax, esi
    cdq
    idiv ecx,
    mov [esp+0x14], eax

    shl eax, 0x8 ; put into little endian format (00BBGGRR)
    bswap eax

    mov edx, [esp+0x14]
    shl edx, 0x8
    or edx, [esp+0x10]
    shl edx, 0x8
    or edx, [esp+0xc]

    mov eax, [ebp+0xc] ; starting colour

    shl edx, 0x8 ; put increments into little endian format
    bswap edx

    mov ebx, [ebp+0x18]
    mov ecx, [ebp+0x14]
    xor esi, esi
fillgradient_loop:
    mov [edi], eax

    add edi, 4
    sub ecx, 4
    inc esi

    add eax, edx

    cmp esi, ebx
    jl continue_loop

    mov eax, [ebp+0xc]
    xor esi, esi
continue_loop:
    test ecx, ecx
    jnz fillgradient_loop

    add esp, 0x18

    pop ebx
    pop edi
    pop esi

    mov esp, ebp
    pop ebp

    ret

global _fillcolor

section .text

_fillcolor:
    push ebp
    mov ebp, esp

    push eax
    push ebx
    push ecx
    push edx

    mov ebx, [ebp+0x8] ; location in memory of the beginning of the colour array
    mov eax, [ebp+0xc] ; colour
    mov ecx, [ebp+0x10] ; size of colour array
loop:
    shl eax, 0x8
    mov edx, eax
    shr edx, 0x18
    or eax, edx
    mov edx, eax
    bswap edx

    mov [ebx], edx
    add ebx, 0x4
    sub ecx, 0x4
    cmp ecx, 1 
    jge loop

    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp

    ret

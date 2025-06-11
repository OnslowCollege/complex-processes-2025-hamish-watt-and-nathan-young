global _fillcolor
global _fillcolorvertical

section .text

_fillcolor:
    push ebp
    mov ebp, esp

    push ebx

    mov ebx, [ebp+0x8] ; location in memory of the beginning of the colour array
    mov eax, [ebp+0xc] ; colour
    mov ecx, [ebp+0x10] ; size of colour array

    shl eax, 0x8
    bswap eax
fillcolor_loop:
    mov [ebx], eax

    add ebx, 4
    sub ecx, 4
    test ecx, ecx
    jnz fillcolor_loop

    pop ebx
    mov esp, ebp
    pop ebp


    ret

_fillcolorvertical:
    push ebp
    mov ebp, esp

    push ebx

    mov ecx, [ebp+0x8] ; location in memory of the beginning of the colour array
    mov edx, [ebp+0xc] ; colour
    mov eax, [ebp+0x10] ; size of pixels to draw
    mov ebx, [ebp+0x14] ; rectangle width in bytes

    shl edx, 0x8
    bswap edx
fillcolorvertical_loop:
    mov [ecx], edx

    add ecx, ebx
    sub eax, 0x4
    test eax, eax
    jnz fillcolorvertical_loop

    pop ebx
    mov esp, ebp
    pop ebp

    ret

global _fillcolor
global _fillcolorvertical

section .text

fillpixel:
    push ebp,
    mov ebp, esp

    mov eax, edx
    shr eax, 0x10

    mov [ecx], al
    mov [ecx+0x1], dh
    mov [ecx+0x2], dl

    pop ebp
    ret

_fillcolor:
    push ebp
    mov ebp, esp

    push ebx

    mov ebx, [ebp+0x8] ; location in memory of the beginning of the colour array
    mov eax, [ebp+0xc] ; colour
    mov ecx, [ebp+0x10] ; size of colour array
fillcolor_loop:
    shl eax, 0x8
    mov edx, eax
    shr edx, 0x18
    or eax, edx
    mov edx, eax
    bswap edx

    mov [ebx], edx
    add ebx, 0x4
    sub ecx, 0x4
    cmp ecx, 3
    jg fillcolor_loop
    jne fillcolor_done

    mov ecx, ebx
    mov edx, [ebp+0xc]
    call fillpixel
fillcolor_done:
    pop ebx
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
fillcolorvertical_loop:
    push eax
    call fillpixel
    pop eax

    add ecx, ebx
    sub eax, 0x3
    test eax, eax
    jnz fillcolorvertical_loop

    pop ebx
    pop ebp

    ret

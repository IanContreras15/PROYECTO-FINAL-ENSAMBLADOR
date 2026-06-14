bits 64
default rel

global contar_caracteres
global validar_movimiento
global calcular_puntaje
global detectar_objeto
global contar_celdas_libres

section .text

contar_caracteres:
    xor  eax, eax
    xor  r9d, r9d
    cmp  edx, 0
    jle  .fin_contar
    .loop_contar:
        cmp  r9d, edx
        jge  .fin_contar
        mov  r10b, [rcx + r9]
        cmp  r10b, r8b
        jne  .siguiente_contar
        inc  eax
    .siguiente_contar:
        inc  r9d
        jmp  .loop_contar
    .fin_contar:
    ret

validar_movimiento:
    mov  eax, r8d
    imul eax, edx
    add  eax, r9d
    movsxd rax, eax
    mov  r10b, [rcx + rax]
    cmp  r10b, '#'
    je   .bloqueado
    mov  eax, 1
    ret
    .bloqueado:
    xor  eax, eax
    ret

calcular_puntaje:
    mov  eax, ecx
    imul eax, 100
    mov  r9d, r8d
    imul r9d, 500
    add  eax, r9d
    mov  r10d, edx
    imul r10d, 2
    sub  eax, r10d
    cmp  eax, 0
    jge  .fin_puntaje
    xor  eax, eax
    .fin_puntaje:
    ret

detectar_objeto:
    mov  eax, r8d
    imul eax, edx
    add  eax, r9d
    movsxd rax, eax
    mov  r10b, [rcx + rax]
    mov  r11b, [rsp + 40]
    cmp  r10b, r11b
    je   .encontrado
    xor  eax, eax
    ret
    .encontrado:
    mov  eax, 1
    ret

contar_celdas_libres:
    xor  eax, eax
    xor  r9d, r9d
    cmp  edx, 0
    jle  .fin_libres
    .loop_libres:
        cmp  r9d, edx
        jge  .fin_libres
        mov  r10b, [rcx + r9]
        cmp  r10b, '.'
        jne  .siguiente_libres
        inc  eax
    .siguiente_libres:
        inc  r9d
        jmp  .loop_libres
    .fin_libres:
    ret
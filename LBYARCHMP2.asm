section .text
global calculate_distances_asm

calculate_distances_asm:
    push rbp
    mov rbp, rsp
    push rbx
    push rsi
    push rdi

    mov r10, [rbp + 48]
    mov r11, [rbp + 56]

    xor rax, rax
    mov rbx, rcx

    test rcx, rcx
    jz done

main_loop:
    movsd xmm0, qword [rdx + rax*8]
    movsd xmm1, qword [r8 + rax*8]
    movsd xmm2, qword [r9 + rax*8]
    movsd xmm3, qword [r10 + rax*8]

    subsd xmm1, xmm0
    subsd xmm3, xmm2

    mulsd xmm1, xmm1
    mulsd xmm3, xmm3

    addsd xmm1, xmm3
    sqrtsd xmm1, xmm1

    movsd qword [r11 + rax*8], xmm1

    inc rax
    cmp rax, rbx
    jl main_loop

done:
    pop rdi
    pop rsi
    pop rbx
    pop rbp
    ret

%include "../include/io.mac"

; declare your structs here

section .text
    global check_passkeys
    extern printf

check_passkeys:
    ;; DO NOT MODIFY
    enter 0, 0
    pusha

    mov ebx, [ebp + 8]      ; requests
    mov ecx, [ebp + 12]     ; length
    mov eax, [ebp + 16]     ;
    ;; DO NOT MODIFY

    ;; Your code starts here
    
requests_loop:
    push    ecx
    push    eax
check_msb:
    xor     ecx, ecx
    mov     ecx, [ebx + 2] ; ecx stores the KEY
    and     ecx, 0xFFFF
    
    xor     eax, eax
    mov     eax, ecx
    shr     eax, 15
    cmp     eax, 1
    jl      legitimate_auth  ; First bit is not 1 
                        
    cmp     eax, 1      
    jz      check_lsb        ; Continue inspection

check_lsb:
    xor     eax, eax
    mov     eax, ecx
    and     eax, 1

    cmp     eax, 1
    jz      check_seven_lsb  ; Continue inspection

    cmp     eax, 1
    jl      legitimate_auth  ; Last bit is not 1

check_seven_lsb: ; Check the 7 lower bits that remain
    xor     eax, eax
    mov     eax, ecx
    and     eax, 0xFF
    shr     eax, 1
    ; Now eax, contains the 7 lower bits 
    push    ecx 
    push    ebx
    mov     ecx, 7
    xor     edi, edi

count_bits1:        
    ; This loop counts the bits set to 1
    ; and stores the number in edi
    mov     ebx, eax    
    and     ebx, 1
    add     edi, ebx
    shr     eax, 1
    loop count_bits1

    pop     ebx
    pop     ecx

    ; Test if the number is divisible by 2 
    ; (edi AND 1 will set the 0F if so)
    test    edi, 1
    jz check_seven_msb  ; Even number of bits set to 1

    test    edi, 1
    jnz legitimate_auth ; Odd number of bits set to 1

check_seven_msb:
    xor     eax, eax
    mov     eax, ecx
    shr     eax, 8  ; eax contains the upper 8 bits
    push    ecx     ; but we will only count up to 
    push    ebx     ; the seventh
    mov     ecx, 7
    xor     edi, edi

count_bits2:
    mov     ebx, eax
    and     ebx, 1
    add     edi, ebx
    shr     eax, 1
    loop    count_bits2

    pop ebx
    pop ecx

    test edi, 1
    jz legitimate_auth ; Even number of ones

    test edi, 1
    jnz hacker_detected ; Odd number of ones
    
hacker_detected:
    pop eax
    mov byte [eax], 1
    jmp next_request

legitimate_auth:
    pop eax
    mov byte [eax], 0

next_request:
    add ebx, 55
    inc eax
    pop ecx
    dec ecx
    cmp ecx, 0
    jnz requests_loop
    
    mov eax, [ebp + 16]
    mov ebx, [ebp + 8]

    ;PRINTF32 `\n\x0`
    ;; Your code ends here
    
    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
%include "../include/io.mac"

; declare your structs here

section .text
    global sort_requests
    extern printf

sort_requests:
    ;; DO NOT MODIFY
    enter 0,0
    pusha

    mov ebx, [ebp + 8]      ; requests
    mov ecx, [ebp + 12]     ; length
    ;; DO NOT MODIFY

    ;; Your code starts here
    mov edx, ecx 
    
outerloop:
    ; This loops based od edx from len - 1, to 0
    dec edx

    ; start innerloop
    mov ecx, [ebp + 12] 
    dec ecx


innerloop:
    ; This loops based on ecx from len - 1, to 0
    dec  ecx
    push edx
    ; We calculate the offset of the element we
    ; want to compare.
    xor eax, eax
    mov eax, 55
    mul ecx
    mov edx, eax
    add edx, ebx
    ; Now edx stores the offset
    ; We will compare this element to the one next to him
    
    ; Store the admin status of element in eax
    xor eax, eax
    mov al, byte [edx]

    push ebx        ; Save ebx
        
    ; Store the admin status of the next element in ebx
    xor ebx, ebx
    mov bl, byte [edx + 55]
    
    mov esi, edx    ; Store the address of first elem
    mov edi, edx 
    add edi, 55     ; Store the address of second elem

    cmp eax, ebx    ; The first element => admin = 0
    jl  exchange     ; The second elemnt => admin = 1

    
    cmp eax, ebx            ; First elemt => admin = 1
    jg  do_not_exchange      ; Second => admin = 0

    cmp eax, ebx            ; admin first = admin second
    jz  check_prio           ; Compare priority

check_prio:
    xor eax, eax
    xor ebx, ebx
    mov al, byte [edx + 1]  ; Store the priority of 1st
    mov bl, byte [edx + 56] ; Store the priority of 2nd

    cmp eax, ebx
    jl  do_not_exchange      ; Already sorted

    cmp eax, ebx            ; 1st prio > 2nd prio
    jg  exchange             ; We need to exchange

    cmp eax, ebx            ; 1st prio = 2nd prio
    jz  check_alp

check_alp:
    push eax
    push ebx
    push ecx
    push esi
    push edi        ; Save all the needed information

    mov ecx, 50     ; We will compare the first 50 characters 
    add esi, 4      ; esi -> address of 1st name
    add edi, 4      ; edi -> address of 2nd name

alp_loop:           ; This loops compares the strings
    xor eax, eax
    xor ebx, ebx
    mov al, [esi]   
    mov bl, [edi]
    
    cmp eax, ebx    ; The strings are already sorted
    jl  do_not_exchange_intermediate

    cmp eax, ebx    ; The strings are not sorted
    jg  exchange_intermediate
    

    inc  esi         ; Move to next character
    inc  edi         ; Move to next character
    loop alp_loop

do_not_exchange_intermediate:
    pop  edi
    pop  esi
    pop  ecx
    pop  ebx
    pop  eax
    jmp  do_not_exchange ; Restore data and dont exchange

exchange_intermediate:
    pop edi
    pop esi
    pop ecx
    pop ebx
    pop eax
    jmp exchange        ; Restore data and exchange

exchange:
    push ecx            ; Save ecx
    mov  ecx, 55
    
swap:                   ; Swap loop
    mov  al, [esi]
    xchg al, [edi]
    mov  [esi], al       ; xchg bytes
    inc  esi             ;
    inc  edi             ; Move to next byte
    loop swap
    pop  ecx             ; Restore ecx

do_not_exchange:
    
    pop  ebx
    pop  edx
    cmp  ecx, 0
    jnz  innerloop

    ; outerloop
    cmp  edx, 0
    jnz  outerloop

    
    ;; Your code ends here

    ;; DO NOT MODIFY
    popa
    leave
    ret
    ;; DO NOT MODIFY
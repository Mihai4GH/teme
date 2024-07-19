; subtask 2 - bsearch


section .text
    global binary_search
    ;; no extern functions allowed    
;int32_t __attribute__((fastcall)) binary_search(int32_t *buff, uint32_t needle, uint32_t start, uint32_t end);

binary_search:
    ; ecx -> *buff
    ; edx -> needle ( respectam conventia de fastcall)
    ;; create the new stack frame
    enter   0, 0
    push    ebx
    ;; save the preserved registers    
    ;; recursive bsearch implementation goes here
    mov     eax, dword [ebp+8]
    ; Calculate m
    add     eax, dword [ebp+12]
    ; devide by 2
    shr     eax, 1
    ; Daca start > end -> setam eax = -1
    mov     ebx, dword [ebp+8]
    ; cmp start, end
    cmp     ebx, dword [ebp+12]
    jg      .set_negative

    ; Daca v[eax] = needle -> am gasit
    cmp     [ecx+4*eax], edx
    je      .done

    ; Daca v[eax] > needle -> cautam pe start, eax - 1
    cmp     [ecx+4*eax], edx
    jg      .call_bs_low

    ; Daca v[eax] < needle -> cautam pe eax+1, start
    cmp     [ecx+4*eax], edx
    jl      .call_bs_high

.call_bs_low:
    dec     eax
    push    eax
    ; push old start
    push    dword [ebp+8]
    call    binary_search
    ; reset stack
    add     esp, 8
    jmp     .done

.call_bs_high:
    inc     eax
    ; push old end
    push    dword [ebp+12]
    push    eax
    call    binary_search
    ; reset stack
    add     esp, 8
    jmp     .done

.set_negative:
    ; return -1
    mov     eax, -1

.done:
    ;; restore the preserved registers
    pop ebx
    leave
    ret

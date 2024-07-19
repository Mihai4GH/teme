; subtask 1 - qsort

section .text
    global quick_sort
    ;; no extern functions allowed    
;void quick_sort(int32_t *buff, uint32_t start, uint32_t end);

quick_sort:
    ;; create the new stack frame
    enter 0, 0
    pusha
    ;; save the preserved registers    
    ;; recursive qsort implementation goes here
    ; start
    mov     eax, [ebp+12]
    ; end
    mov     ebx, [ebp+16]
    cmp     eax, ebx
    ; start > end
    jge     .base_case
    ; Determine m and echange buff[m] with buff[st]
    mov     eax, [ebp+12]
    ; eax <- start, ebx <- end
    mov     ebx, [ebp+16]
    add     ebx, eax
    ; devide by 2
    shr     ebx, 1
    ; ecx <- buff
    mov     ecx, [ebp+8]
    ; xcgh buff[start] with buff[m]
    push    dword [ecx+4*eax]
    ; xcgh buff[start] with buff[m]
    mov     edx, [ecx+4*ebx]
    ; xcgh buff[start] with buff[m]
    mov     dword [ecx+4*eax], edx
    pop     edx
    ; xcgh buff[start] with buff[m]
    mov     dword [ecx+4*ebx], edx
    ; end
    mov     eax, [ebp+12]
    ; start
    mov     ebx, [ebp+16] 
    ; buff
    mov     edx, [ebp+8]
    ; edi = d from pseudocode  
    mov     edi, 0

.while_sle:  ; Start Lower than End
    ; Compare buff[i] with buff[j]
    mov     ecx, [edx+4*eax]
    ; Compare buff[i] with buff[j]
    cmp     ecx, [edx+4*ebx]
    jle     .dont_xcgh
    ; Echange
    push    dword [edx+4*eax]
    ; Echange
    mov     ecx, [edx+4*ebx]
    ; Echange
    mov     dword [edx+4*eax], ecx
    pop     ecx
    ; Echange
    mov     dword [edx+4*ebx], ecx
    ; switch d
    xor     edi, 1

.dont_xcgh:
    add     eax, edi
    add     ebx, edi
    ; update ebx <=> j
    sub     ebx, 1
    cmp     eax, ebx
    jl      .while_sle
    ; Recursive calls
    sub     eax, 1
    push    eax
    ; push start
    push    dword [ebp+12]
    ; push buff
    push    dword [ebp+8]
    call    quick_sort
    ; reset stack
    add     esp, 12
    ; ebx = i+1        
    add     ebx, 1
    ; push end
    push    dword [ebp+16]
    push    ebx
    ; push buff
    push    dword [ebp+8]
    call    quick_sort
    ; reset stack
    add     esp, 12   
.base_case:

.done:
    ;; restore the preserved registers
    popa
    leave
    ret

%include "../include/io.mac"

extern ant_permissions

extern printf
global check_permission

section .text

check_permission:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     eax, [ebp + 8]  ; id and permission
    mov     ebx, [ebp + 12] ; address to return the result
    ;; DO NOT MODIFY
   
    ;; Your code starts here
    push    eax ; save eax
    shr     eax, 24 ; index of ant
    ; Move in edx the permisions of the and
    mov     edx, dword [ant_permissions + 4 * eax] 
    and     edx, 0xFFFFFF   ; Make sure to contain only the 3 lower bytes
    pop     eax
    and     eax, 0xFFFFFF   ; Make sure to get from eax only the 3 lower bytes
    and     edx, eax        
    ; If eax == edx => all the bits that are set in eax
    ; are also set in edx.
    cmp     eax, edx        
    je      set1            ; The ant can reserve the rooms
    mov     dword [ebx], 0
    jmp     end             ; The and can't reserve all the rooms 

set1:
    mov     dword [ebx], 1

end:

    ;; Your code ends here
    
    ;; DO NOT MODIFY

    popa
    leave
    ret
    
    ;; DO NOT MODIFY

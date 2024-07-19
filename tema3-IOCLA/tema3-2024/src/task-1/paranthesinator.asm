; Interpret as 32 bits code
[bits 32]

%include "../include/io.mac"

section .data
    ; stiva de 100 de bytes
    stack db 100, 0

section .text
    ; int check_parantheses(char *str)
    global check_parantheses
    global get_coresponding
    extern printf

get_coresponding:
    push    ebp
    mov     ebp, esp
    xor     eax, eax
    ; Move in eax ')', ']' or '}'
    mov     eax, dword [ebp + 8]
    ; '('
    mov     ebx, 40
    ; ')'     
    cmp     eax, 41     
    jz      found
    ; '['
    mov     ebx, 91
    ; ']'     
    cmp     eax, 93        
    jz      found
    ; '{'
    mov     ebx, 123
    ; '}'    
    cmp     eax, 125    
    jz      found

found:
    leave
    ret    

    ; Explicatii suplimentare in README
check_parantheses:
    push    ebp
    mov     ebp, esp
    push    ebx
    ; sa-nceapa concursul
    mov     edi, dword [ebp + 8]
    xor     ecx, ecx

ploop:      ; pharantheses loop
    xor     eax, eax
    mov     al, byte [edi]
    test    al, al
    jz      out_true
    ; The '\0' is encountered so we stop the search
    ; '('
    cmp     eax, 40
    jz      open
    ; '['
    cmp     eax, 91
    jz      open
    ; '{'
    cmp     eax, 123
    jz      open
    ; If a open pharantheses is detecter we add it to the stack
    cmp     ecx, 0
    jz      out_false
    ; If we want to close a parantheses without having others open left
    ; we jump to out_false
    push    eax
    call    get_coresponding
    ; La asa ceva NU trebuie eroare de codyng style!
    add     esp, 4
    ; We check wether the open and closed pharanthesez are of the same type
    cmp     byte [stack + ecx - 1], bl
    jnz     out_false
    dec     ecx
    jmp     next_iteration

open: 
    mov     byte [stack + ecx], al
    inc     ecx

next_iteration:
    xor     edx, edx
    mov     edx, [stack + ecx]
    inc     edi
    jmp     ploop

out_false:
    ; Send false signal
    mov     eax, 1
    ; Make sure the prgam wont cycle
    mov     ecx, 0

out_true:
    ; We check wether the stack isn't empty
    cmp     ecx, 0
    jnz     out_false

    pop     ebx
    leave
    ret

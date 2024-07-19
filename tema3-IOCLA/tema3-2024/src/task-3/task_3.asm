%include "../include/io.mac"

; The `expand` function returns an address to the following type of data
; structure.
struc neighbours_t
    .num_neighs resd 1  ; The number of neighbours returned.
    .neighs resd 1      ; Address of the vector containing the `num_neighs` neighbours.
                        ; A neighbour is represented by an unsigned int (dword).
endstruc

section .bss
; Vector for keeping track of visited nodes.
visited resd 10000
global visited

section .data
; Format string for printf.
fmt_str db "%u", 10, 0

section .text
global dfs
extern printf

; C function signiture:
;   void dfs(uint32_t node, neighbours_t *(*expand)(uint32_t node))
; where:
; - node -> the id of the source node for dfs.
; - expand -> pointer to a function that takes a node id and returns a structure
; populated with the neighbours of the node (see struc neighbours_t above).
; 
; note: uint32_t is an unsigned int, stored on 4 bytes (dword).
dfs:
    push    ebp
    mov     ebp, esp
    pusha
    ; TODO: Implement the depth first search algorith, using the `expand`
    ; function to get the neighbours. When a node is visited, print it by
    ; calling `printf` with the format string in section .data    
    mov     eax, [ebp+8]
    ; Daca e viziat return
    cmp     dword [visited+4*eax], 1
    je      end

    ; Marcam ca vizitat
    mov     dword [visited+4*eax], 1    
    pusha
    ; Print node
    push    dword [ebp+8]
    push    fmt_str
    call    printf
    ; Reset string
    add     esp, 8
    popa

    ; Acu' facem while pe rezultatul de la expand
    mov     ebx, dword [ebp+8]
    ; push node
    push    dword [ebp+8]
    ; call expand
    call    dword [ebp+12]
    ; reset stack
    add     esp, 4

    mov     ecx, [eax] 
    ; Daca numar de vecini = 0 => end 
    cmp     ecx, 0
    je      end
    ; Vector de vecini
    mov     edx, [eax+4]
    ; Contor 
    mov     edi, 0

while:    
    pusha
    ; Push expand
    mov     ebx, [ebp+12]
    push    ebx
    ; Push neigbour
    mov     ebx, [edx+4*edi]
    push    ebx
    call    dfs
    ; Reset stack
    add     esp, 8
    popa    
    inc     edi
    cmp     edi, ecx
    jl      while    
end:
    popa
    leave
    ret

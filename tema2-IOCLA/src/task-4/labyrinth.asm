%include "../include/io.mac"

extern printf
extern position
global solve_labyrinth

; you can declare any helper variables in .data or .bss

section .text

; void solve_labyrinth(int *out_line, int *out_col, int m, int n, char **labyrinth);
solve_labyrinth:
    ;; DO NOT MODIFY
    push    ebp
    mov     ebp, esp
    pusha

    mov     eax, [ebp + 8]  ; unsigned int *out_line, pointer to structure containing exit position
    mov     ebx, [ebp + 12] ; unsigned int *out_col, pointer to structure containing exit position
    mov     ecx, [ebp + 16] ; unsigned int m, number of lines in the labyrinth
    mov     edx, [ebp + 20] ; unsigned int n, number of colons in the labyrinth
    mov     esi, [ebp + 24] ; char **a, matrix represantation of the labyrinth
    ;; DO NOT MODIFY
   
    ;; Freestyle starts here
    mov eax, 0  ; eax will store number of current row
    mov ebx, 0  ; ebx -------------------- current collumn
    
    mov edx, dword [esi + 4]
    xor ecx, ecx
    mov cl, [edx]
    cmp ecx, 48 ; CODUL PT 0 E 48
    jz one_zero

    jmp     zero_one

    ; First check wheter we can go bellow or to the right (1,0) 
    ; or (0,1)

one_zero:
    mov     edi, dword [esi]
    mov     byte [edi], 49
    mov     eax, 1
     
    jmp     start_search

zero_one:
    mov     edi, dword [esi]
    mov     byte [edi], 49
    mov     ebx, 1
    
    jmp     start_search

start_search:

SEARCH_LOOP:
    mov     ecx, dword [ebp + 16]   ; lines
    mov     edx, dword [ebp + 20]   ; columns
    dec     ecx
    dec     edx
    cmp     ecx, eax
    jz      end_preparation
    cmp     edx, ebx
    jz      end_preparation
    ; The solution was not found
   
    mov     edi, [esi + 4 * eax]
    mov     byte [edi + ebx], 49    
    ; We mark the current position with 1 (ASCII = 49)
    ;and keep exploring
    cmp     ebx, 0
    je      COL_NULL ; (Left edge of matrix)

    cmp     eax, 0
    je      LINE_NUL ; (Top edge of matrix)
    ; No jump => the cursor is 'inside' the matrix
    
INSIDE:
    ; verificam mai intai m[eax-1][ebx], 
    mov edi, [esi + 4 * eax - 4]
    cmp byte [edi + ebx], 48
    je  ABOVE
    ; verificam m[eax][ebx - 1]
    mov edi, [esi + 4 * eax]
    cmp byte [edi + ebx - 1], 48
    je LEFT
    ; verificam m[eax][ebx + 1]
    mov edi, [esi + 4 * eax]
    cmp byte [edi + ebx + 1], 48
    je RIGHT
    ; verificam m[eax - 1][ebx]
    mov edi, [esi + 4 * eax + 4]
    cmp byte [edi + ebx], 48
    je BELLOW
    
COL_NULL:
    ; verificam m[eax - 1][ebx]
    mov edi, [esi + 4 * eax - 4]
    cmp byte [edi + ebx], 48
    je ABOVE
    ; verificam m[eax + 1][ebx]
    mov edi, [esi + 4 * eax + 4]
    cmp byte [edi + ebx], 48
    je BELLOW
    ; verificam m[eax][ebx + 1]
    mov edi, [esi + 4 * eax]
    cmp byte [edi + ebx + 1], 48
    je RIGHT
LINE_NUL:
    ; verificam m[eax][ebx -1]
    mov edi, [esi + 4 * eax]
    cmp byte [edi + ebx - 1], 48
    je LEFT
    ; verificam m[eax][ebx + 1]
    mov edi, [esi + 4 * eax]
    cmp byte [edi + ebx + 1], 48
    je RIGHT
    ; verificam m[eax+1][ebx]
    mov edi, [esi + 4 * eax + 4]
    cmp byte [edi + ebx], 48
    je BELLOW

ABOVE:
    dec eax
    jmp NEXT_ITERATION

LEFT:
    dec ebx
    jmp NEXT_ITERATION

RIGHT:
    inc ebx
    jmp NEXT_ITERATION

BELLOW:
    inc eax
    jmp NEXT_ITERATION

NEXT_ITERATION:
    jmp SEARCH_LOOP
    
end_preparation:
    ; Transmit the result over
    mov ecx, eax
    mov edx, ebx
    mov     eax, [ebp + 8]  ; unsigned int *out_line, pointer to structure containing exit position
    mov     ebx, [ebp + 12] ; unsigned int *out_col, pointer to structure containing exit position
    mov     dword [eax], ecx
    mov     dword [ebx], edx
    ;; Freestyle ends here
end:
    ;; DO NOT MODIFY

    popa
    leave
    ret
    
    ;; DO NOT MODIFY

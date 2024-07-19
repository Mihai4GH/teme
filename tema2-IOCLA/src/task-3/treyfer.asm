section .rodata
	global sbox
	global num_rounds
	sbox db 126, 3, 45, 32, 174, 104, 173, 250, 46, 141, 209, 96, 230, 155, 197, 56, 19, 88, 50, 137, 229, 38, 16, 76, 37, 89, 55, 51, 165, 213, 66, 225, 118, 58, 142, 184, 148, 102, 217, 119, 249, 133, 105, 99, 161, 160, 190, 208, 172, 131, 219, 181, 248, 242, 93, 18, 112, 150, 186, 90, 81, 82, 215, 83, 21, 162, 144, 24, 117, 17, 14, 10, 156, 63, 238, 54, 188, 77, 169, 49, 147, 218, 177, 239, 143, 92, 101, 187, 221, 247, 140, 108, 94, 211, 252, 36, 75, 103, 5, 65, 251, 115, 246, 200, 125, 13, 48, 62, 107, 171, 205, 124, 199, 214, 224, 22, 27, 210, 179, 132, 201, 28, 236, 41, 243, 233, 60, 39, 183, 127, 203, 153, 255, 222, 85, 35, 30, 151, 130, 78, 109, 253, 64, 34, 220, 240, 159, 170, 86, 91, 212, 52, 1, 180, 11, 228, 15, 157, 226, 84, 114, 2, 231, 106, 8, 43, 23, 68, 164, 12, 232, 204, 6, 198, 33, 152, 227, 136, 29, 4, 121, 139, 59, 31, 25, 53, 73, 175, 178, 110, 193, 216, 95, 245, 61, 97, 71, 158, 9, 72, 194, 196, 189, 195, 44, 129, 154, 168, 116, 135, 7, 69, 120, 166, 20, 244, 192, 235, 223, 128, 98, 146, 47, 134, 234, 100, 237, 74, 138, 206, 149, 26, 40, 113, 111, 79, 145, 42, 191, 87, 254, 163, 167, 207, 185, 67, 57, 202, 123, 182, 176, 70, 241, 80, 122, 0
	num_rounds dd 10

section .text
	global treyfer_crypt
	global treyfer_dcrypt

; void treyfer_crypt(char text[8], char key[8]);
treyfer_crypt:
	;; DO NOT MODIFY
	push ebp
	mov ebp, esp
	pusha

	mov esi, [ebp + 8] ; plaintext
	mov edi, [ebp + 12] ; key	
	;; DO NOT MODIFY
	;; FREESTYLE STARTS HERE
	;; TODO implement treyfer_crypt
	mov ecx, dword [num_rounds]

rounds: ; LOOPS for 10 times
	push ecx
	xor eax, eax
	mov al, byte [esi] ; First byte of text
	xor ecx, ecx

one_to_seven:	; encodes the bytest 1 to 7
	; The t variable will be stored in eax
	xor ebx, ebx
	mov bl, byte [edi]
	add al, bl
	and eax, 0xFF
	mov ebx, eax
	mov al, byte [sbox + ebx]
	xor ebx, ebx
	mov bl, byte [esi + 1]
	add al, bl
	and eax, 0xFF
	
	; Now we need to apply a rotation
	mov ebx, eax
	shr ebx, 7
	and ebx, 1
	shl eax, 1
	and eax, 0xFF
	add eax, ebx
	mov byte [esi + 1], al

	
	inc esi
	inc edi
	inc ecx
	cmp ecx, 7
	jl  one_to_seven
	; Loop ends here
	; Now we encode the byte 0
	; Keep in mind that edi, and esi
	; now point to the last byte of the 
	; text and secret key
	xor ebx, ebx
	mov bl, byte [edi]
	add al, bl
	and eax, 0xFF
	mov ebx, eax
	mov al, byte [sbox + ebx]
	xor ebx, ebx
	mov edx, [ebp + 8] 
	; Now edx points to the addres of the 
	; 1st byte of the text.
	; This is the byte that we need to add 
	; to eax.
	mov bl, byte [edx]
	add al, bl
	and eax, 0xFF

	; rotim

	mov ebx, eax
	shr ebx, 7
	and ebx, 1
	shl eax, 1
	and eax, 0xFF
	add eax, ebx
	mov byte [edx], al
	
	; Restore esi, edi
	mov esi, [ebp + 8] ; plaintext
	mov edi, [ebp + 12] ; keyv

	pop ecx
	dec ecx
	cmp ecx, 0
	; Loop for 10 rounds.
	jnz rounds 
    
	; This is just to make sure that i restored
	; esi and edi.
	mov esi, [ebp + 8] ; plaintext
	mov edi, [ebp + 12] ; keyv
	
	;; FREESTYLE ENDS HERE
	;; DO NOT MODIFY
	popa
	leave
	ret

; void treyfer_dcrypt(char text[8], char key[8]);
treyfer_dcrypt:
	;; DO NOT MODIFY
	push ebp
	mov ebp, esp
	pusha
	;; DO NOT MODIFY
	;; FREESTYLE STARTS HERE
	;; TODO implement treyfer_dcrypt
	mov ecx, dword [num_rounds]

	mov esi, [ebp + 8] ; plaintext
	mov edi, [ebp + 12] ; key

rounds_dec:
	push ecx
	; Firstly decode the byte 0
	xor  eax, eax
	mov  al, byte [esi + 7]
	xor  ebx, ebx
	mov  bl, byte [edi + 7]
	add  al, bl
	and  eax, 0xFF		; treat overflow
	mov  ebx, eax;
	mov  al, byte [sbox + ebx]
	push eax ; SAVE TOP
	mov  al, byte [esi]
	and  eax, 0xFF
	mov  ebx, eax
	and  ebx, 1
	shl  ebx, 7
	shr  eax, 1
	add  al, bl 		; bottom 
	pop  ebx			; top
	sub  al, bl
	and  eax, 0xFF		; treat underflow
	mov  byte [esi], al	; 

	mov  ecx, 7

	; Now decode the remaining bytes in 
	; the following loop
seven_to_one:
	push ecx

	dec  ecx
	xor  eax, eax
	xor  ebx, ebx
	mov  al, byte [esi + ecx]
	mov  bl, byte [edi + ecx]
	add  al, bl
	and  eax, 0xFF
	mov  ebx, eax
	mov  al, byte [sbox + ebx]
	push eax ; save top
	xor  eax, eax
	mov  al, byte [esi + ecx + 1]
	and  eax, 0xFF
	mov  ebx, eax
	and  ebx, 1
	shl  ebx, 7
	shr  eax, 1
	add  al, bl ; bottom
	pop  ebx 	; top
	sub  al, bl
	and  eax, 0xFF
	mov  byte [esi + ecx + 1], al


	pop  ecx
	dec  ecx
	cmp  ecx, 0
	jnz  seven_to_one

	pop  ecx
	dec  ecx
	cmp  ecx, 0
	jnz  rounds_dec

	;; FREESTYLE ENDS HERE
	;; DO NOT MODIFY
	popa
	leave
	ret


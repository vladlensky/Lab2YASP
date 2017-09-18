global _start
section .text

global string_length
string_length:
	xor rax, rax
	.loop:
	cmp  byte[rdi + rax], 0
	je   .ret
	inc  rax
	jmp  .loop	  
	.ret:
    ret

global print_string
print_string:
	call string_length
	mov  rdx, rax
	mov  rsi, rdi
	mov  rax, 1
	mov  rdi, 1
	syscall
    ret

global print_char
print_char:
	push rdi              
	mov  rax, 1         
	mov  rsi, rsp       
	mov  rdi, 1           
	mov  rdx, 1          
	syscall
	pop  rdi
    ret

global print_newline
print_newline:
	mov rdi, 0xA
	jmp print_char

global print_uint
print_uint:
	mov rax, rdi    
	mov r10, 10    	  	
	mov r9,  rsp     
	dec rsp              
	mov byte[rsp], 0 
   .loop:
   	xor  rdx, rdx     	
   	div  r10          
  	dec  rsp    
   	add  dl, '0'
  	mov  [rsp], dl
   	test rax, rax  
    jnz  .loop
	mov  rdi, rsp   
  	call print_string
  	mov  rsp, r9      
    ret

global print_int
print_int:
	mov r11 , rdi
	test r11, r11
	jns print_uint
	mov rdi, '-'
	push r11
	call print_char
	pop rdi
	neg rdi
	jmp print_uint

global string_equals
string_equals:
	xor r9, r9
	call string_length
	mov r8, rax
	xchg rdi, rsi
	call string_length
	cmp rax, r8
	jne .exit
	.loop:
		mov r11b, byte[rdi+r9]
		mov r10b, byte[rsi+r9]
		cmp r11b, r10b
		jne .exit
		inc r9
		dec rax
		jnz .loop
	.ok:
	mov rax, 1
	ret
	.exit:
	xor rax, rax
   ret

global read_char
read_char:
	push 0
   	mov  rdx, 1         
   	xor rdi, rdi          
   	mov  rsi, rsp        
   	xor rax, rax         
   	syscall
   	pop  rax
   ret 

section .data
word_buffer times 256 db 0

section .text 

global read_word
read_word:
	mov r8, 0
   	.loop:
		call read_char
		test rax, rax
		jz .exit
		cmp rax, 0x21
		jb .loop	
	mov rsi, word_buffer
	mov byte[word_buffer], al
	mov  rdx, 1
	xor rdi, rdi 
    	.loop1:
		inc r8  
		xor rax, rax
		lea rsi, [word_buffer + r8]
		syscall
		cmp byte [word_buffer + r8], 0x21
		jb .exit
		jmp .loop1  
    	.exit:
		mov byte[word_buffer + r8], 0
		mov rdx, r8
		mov rax, word_buffer
    ret
global read_string
read_string:
	mov r8, 0
	call read_char
	test rax, rax
	jz .exit
	mov rsi, word_buffer
	mov byte[word_buffer], al
	mov  rdx, 1
	xor rdi, rdi 
    	.loop1:
		inc r8  
		xor rax, rax
		lea rsi, [word_buffer + r8]
		syscall
		cmp byte [word_buffer + r8], 0xA
		je .exit
		jmp .loop1  
    	.exit:
		mov byte[word_buffer + r8], 0
		mov rdx, r8
		mov rax, word_buffer
    ret

; rdi points to a string
; returns rax: number, rdx : length
global parse_uint
parse_uint:
	mov r9, 10
	xor rax, rax
	xor r10, r10
	xor rcx,rcx
	.loop:
		cmp byte[rdi+rcx], 0x30
		jb .exit
		cmp byte[rdi+rcx], 0x39
		ja .exit
		mul r9
		mov r10b, byte[rdi+rcx]
		sub r10, 0x30
		add rax, r10
		inc rcx
		jmp .loop
	.exit: 
	mov rdx, rcx
    ret

; rdi points to a string
; returns rax: number, rdx : length
global parse_int
parse_int:
	cmp byte[rdi], 0x2D
	je .neg
	jmp parse_uint
	.neg:
	inc rdi
	call parse_uint
	neg rax
	inc rdx
    ret 

global string_copy
string_copy:
	call string_length	; Считаем длину строки и записываем в rcx
        mov  rcx, rax
        inc  rcx			; Чтобы учесть нулевой символ добавляем к длине строки 1 
        push rdi 			; Меняем местами приемник и источник
  	mov rdi, rsi
  	pop rsi
        xor r8,r8
	.loop:		; Копируем строки пока rcx != 0
	cmp byte[rsi+r8],0
	je .end    	
	mov r9b,byte[rsi+r8]
	mov byte[rdi+r8], r9b
	inc r8
	jmp .loop
	.end:mov byte[rdi+r8],0
ret	

extern string_equals
%include "words.inc"
section .text

; adress word ex
global find_word
find_word:
	mov rsi, list
	
	.loop:
	xor rax, rax
	push rsi
	push rdi
	add rsi, 8
	call string_equals
	pop  rdi
	pop  rsi	  
	test rax, rax
	jnz  .finally
	mov rsi, [rsi]
	test rsi, rsi
	jnz  .loop
	ret
  	
	.finally:
	lea rax, [rsi+8] 
 	ret
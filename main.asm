global _start
%include "words.inc" 
extern string_length
extern read_string
extern find_word
extern print_string

section .data
err: db 'Error! Word is not found!', 0
section .text
_start:
	call read_string
	mov rdi, rax
	call find_word
	test rax, rax
	jz .error
	mov rdi, rax
	call string_length
	inc rax 
	lea rdi, [rdi + rax]
	call print_string
	mov rax, 60
	xor rdi, rdi
	syscall
	.error:
	mov rsi, err
	mov rdi, rsi
	call string_length
	mov  rdx, rax
	mov rax, 1
	mov rdi, 2
	syscall
	mov rax, 60
	xor rdi, rdi
	syscall
program: lib.o colon.o words.o dict.o main.o
	ld -o program lib.o colon.o words.o dict.o main.o
lib.o: lib.inc
	nasm -f elf64 -o lib.o lib.inc

colon.o: lib.inc
	nasm -f elf64 -o colon.o colon.inc

words.o: lib.inc
	nasm -f elf64 -o words.o words.inc

dict.o: dict.asm
	nasm -f elf64 -o dict.o dict.asm

main.o: main.asm
	nasm -f elf64 -o main.o main.asm

clean:
	rm -f lib.o colon.o words.o dict.o main.o program

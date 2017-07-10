all: makeos

makeos:
	nasm -f bin main.asm -o main.bin
	qemu-system-i386 main.bin
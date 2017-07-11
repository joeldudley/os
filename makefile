all: makeos

makeos:
	i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o
	nasm enter_kernel.asm -f elf -o enter_kernel.o
	i386-elf-ld -o kernel.bin -Ttext 0x1000 enter_kernel.o kernel.o --oformat binary
	nasm bootsect.asm -f bin -o bootsect.bin
	cat bootsect.bin kernel.bin > os-image.bin
	qemu-system-i386 -fda os-image.bin
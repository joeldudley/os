all: makeos

makeos:
	# Create a temporary build directory.
	mkdir -p build

	# Create the OS image.
	i386-elf-gcc -ffreestanding -c kernel/kernel.c -o build/kernel.o
	nasm bootsect/enter_kernel.asm -f elf -o build/enter_kernel.o
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/enter_kernel.o build/kernel.o --oformat binary
	nasm bootsect/bootsect.asm -f bin -o build/bootsect.bin
	cat build/bootsect.bin build/kernel.bin > build/os-image.bin

	# Run the image.
	qemu-system-i386 -fda build/os-image.bin
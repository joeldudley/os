all: makeos

makeos:
	# Create a temporary build directory.
	mkdir build

	# Create the OS image.
	i386-elf-gcc -ffreestanding -c kernel/kernel.c -o build/kernel.o
	nasm bootsect/enter_kernel.asm -f elf -o build/enter_kernel.o
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/enter_kernel.o build/kernel.o --oformat binary
	nasm bootsect/bootsect.asm -f bin -o build/bootsect.bin
	cat build/bootsect.bin build/kernel.bin > os-image.bin

	# Delete the temporary build directory and its files.
	rm build/*
	rmdir build

	# Run the image.
	qemu-system-i386 -fda os-image.bin

	# Delete the OS image.
	rm os-image.bin
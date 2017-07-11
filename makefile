# On OSX, this makefile requires a GCC cross-compiler - see http://wiki.osdev.org/GCC_Cross-Compiler.

run: build/os-image.bin
	# Run the image.
	qemu-system-i386 -fda build/os-image.bin

debug: build/os-image.bin build/kernel.elf
	# Run the image with a debugger.
	qemu-system-i386 -s -fda build/os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# The targets below should not be invoked directly.

build/os-image.bin: build/kernel.bin build/bootsect.bin
	# Create a temporary build directory.
	mkdir -p build

	cat build/bootsect.bin build/kernel.bin > build/os-image.bin

build/kernel.bin: build/entry_point.o build/kernel.o
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/entry_point.o build/kernel.o --oformat binary

build/bootsect.bin:
	nasm bootsect/main.asm -f bin -o build/bootsect.bin

build/entry_point.o:
	nasm kernel/entry_point.asm -f elf -o build/entry_point.o

build/kernel.o:
	i386-elf-gcc -ffreestanding -c kernel/main.c -o build/kernel.o

# For debugging.
build/kernel.elf: build/entry_point.o
	i386-elf-ld -o build/kernel.elf -Ttext 0x1000 build/entry_point.o build/kernel.o
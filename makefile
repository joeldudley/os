# On OSX, this makefile requires a GCC cross-compiler - see http://wiki.osdev.org/GCC_Cross-Compiler.

run: clean build/os-image.bin
	# Run the image.
	qemu-system-i386 -fda build/os-image.bin

debug: clean build/os-image.bin build/kernel.elf
	# Run the image with a debugger using the `-s` flag.
	qemu-system-i386 -s -fda build/os-image.bin &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"

clean:
	# Create a new build directory.
	mkdir -p build

	# Delete existing build files.
	rm -rf build/*

# The targets below should not be invoked directly.

build/os-image.bin: build/kernel.bin build/bootsect.bin
	cat build/bootsect.bin build/kernel.bin > build/os-image.bin

build/kernel.bin: build/entry_point.o build/kernel.o build/ports.o
	i386-elf-ld -o build/kernel.bin -Ttext 0x1000 build/entry_point.o build/kernel.o build/ports.o --oformat binary

build/bootsect.bin:
	nasm bootsect/main.asm -f bin -o build/bootsect.bin

build/entry_point.o:
	nasm kernel/entry_point.asm -f elf -o build/entry_point.o

build/kernel.o:
	# Compile with debug information using the `-g` flag.
	i386-elf-gcc -g -ffreestanding -c kernel/main.c -o build/kernel.o

build/ports.o:
	i386-elf-gcc -g -ffreestanding -c kernel/ports.c -o build/ports.o

# For debugging.
build/kernel.elf: build/entry_point.o build/kernel.o build/ports.o
	i386-elf-ld -o build/kernel.elf -Ttext 0x1000 build/entry_point.o build/kernel.o build/ports.o
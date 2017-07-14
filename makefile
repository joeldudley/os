# On OSX, this makefile requires a GCC cross-compiler - see http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file
# `$^` refers to the input file(s)

run: clean build/os-image.bin
	# Run the image as a floppy using the `-fda` flag.
	qemu-system-i386 -fda build/os-image.bin

debug: clean os-image.bin kernel.elf
	# Run the image with a debugger using the `-s` flag.
	qemu-system-i386 -s -fda os-image.bin &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	mkdir -p build

	# Delete existing build files.
	rm -f build/*.bin build/*.o build/*.elf

# The targets below should not be invoked directly.

build/os-image.bin: build/bootsect.bin build/kernel.bin
	cat $^ > $@

build/kernel.bin: build/call_kernel.o build/kernel.o build/ports.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

build/%.o: bootsect/%.asm
	nasm $< -f elf -o $@

build/%.bin: bootsect/%.asm
	nasm $< -f bin -o $@

build/%.o: kernel/%.c
	# Compile with debug information using the `-g` flag.
	i386-elf-gcc -g -ffreestanding -c $^ -o $@

# For debugging.
kernel.elf: call_kernel.o kernel.o ports.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^
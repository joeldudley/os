# On OSX, this makefile requires a GCC cross-compiler - see http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file
# `$^` refers to the input file(s)

run: clean build/os-image.bin
	qemu-system-i386 -drive format=raw,file=build/os-image.bin,index=0,if=floppy

# Run the image with a debugger using the `-s` flag.
debug: clean build/os-image.bin build/kernel.elf
	qemu-system-i386 -s -drive format=raw,file=build/os-image.bin,index=0,if=floppy &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"

clean:
	mkdir -p build

	# Delete existing build files.
	rm -f build/*.bin build/*.o build/*.elf

# The targets below should not be invoked directly.

build/os-image.bin: build/bootsect.bin build/kernel.bin
	cat $^ > $@

build/kernel.bin: build/call_kernel.o build/kernel.o build/ports.o build/screen.o build/util.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

build/%.o: bootsect/%.asm
	nasm $< -f elf -o $@

build/%.bin: bootsect/%.asm
	nasm $< -f bin -o $@

# Compile with debug information using the `-g` flag.
build/%.o: kernel/%.c
	i386-elf-gcc -g -ffreestanding -c $^ -o $@

# For debugging.
build/kernel.elf: build/call_kernel.o build/kernel.o build/ports.o build/screen.o build/util.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^
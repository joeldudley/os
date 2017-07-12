# On OSX, this makefile requires a GCC cross-compiler - see http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file
# `$^` refers to the input file(s)

run: clean os-image.bin
	# Run the image as a floppy using the `-fda` flag.
	qemu-system-i386 -fda os-image.bin

debug: clean os-image.bin kernel.elf
	# Run the image with a debugger using the `-s` flag.
	qemu-system-i386 -s -fda os-image.bin &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

clean:
	# Delete existing build files.
	rm -f *.bin *.o *.elf

# The targets below should not be invoked directly.

os-image.bin: bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: call_kernel.o kernel.o ports.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

%.o: bootsect/%.asm
	nasm $< -f elf -o $@

%.bin: bootsect/%.asm
	nasm $< -f bin -o $@

%.o: kernel/%.c
	# Compile with debug information using the `-g` flag.
	i386-elf-gcc -g -ffreestanding -c $^ -o $@

# For debugging.
kernel.elf: call_kernel.o kernel.o ports.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^
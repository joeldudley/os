# On OSX, this makefile requires a GCC cross-compiler - see 
# http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file.
# `$<` refers to the first input file.
# `$^` refers to all input files.

OBJECTS :=  build/call_kernel.o \
			build/kernel_main.o \
			build/ports.o \
			build/screen.o \
			build/util.o \
			build/idt.o \
			build/isr.o \
			build/interrupt.o

# Runs the operating system.
run: clean build/os-image.bin
	qemu-system-i386 -drive format=raw,file=build/os-image.bin,index=0,if=floppy

# Runs the operating system with a debugger.
debug: clean build/os-image.bin build/kernel_main.elf
	# The `-s` flag waits for a gdb connection on TCP port 1234.
	qemu-system-i386 -s -drive format=raw,file=build/os-image.bin,index=0,if=floppy &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel_main.elf"

# Deletes any existing build files.
clean:
	rm -rf build
	# The `-p` flag prevents an error if the `build` folder already exists.
	mkdir -p build

# The targets below should not be invoked directly.

# Concatenates the bootsector and kernal binaries into a single operating system binary.
build/os-image.bin: build/bootsector_main.bin build/kernel_main.bin
	cat $^ > $@

build/kernel_main.bin: $(OBJECTS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# For debugging.
build/kernel_main.elf: $(OBJECTS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^

# Assembles all .asm files in .bin format.
build/%.bin: */%.asm
	nasm $< -f bin -o $@

# Assembles all .asm files in .elf format.
build/%.o: */%.asm
	nasm $< -f elf -o $@

# Compiles all .c files in .o format.
build/%.o: */%.c
	# The `-g` flag compiles the files with debug information.
	i386-elf-gcc -g -ffreestanding -c $^ -o $@
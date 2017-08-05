# On OSX, this makefile requires a GCC cross-compiler - see 
# http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file.
# `$<` refers to the first input file.
# `$^` refers to all input files.

# Runs the operating system.
run: clean build/os-image.bin
	qemu-system-i386 -drive format=raw,file=build/os-image.bin,index=0,if=floppy

# Runs the operating system with a debugger.
debug: clean build/os-image.bin build/kernel.elf
	# The `-s` flag waits for a gdb connection on TCP port 1234.
	qemu-system-i386 -s -drive format=raw,file=build/os-image.bin,index=0,if=floppy &
	/usr/local/i386elfgcc/bin/i386-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file build/kernel.elf"

# Deletes any existing build files.
clean:
	# The `-p` flag prevents an error if the `build` folder already exists.
	mkdir -p build
	# Deletes the existing build files.
	rm -f build/*.bin build/*.o build/*.elf

# The targets below should not be invoked directly.

# Concatenates the bootsector and kernal binaries into a single operating system binary.
build/os-image.bin: build/bootsector_main.bin build/kernel_main.bin
	cat $^ > $@

build/kernel_main.bin: build/call_kernel.o build/kernel_main.o build/ports.o build/screen.o build/util.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# Assembles all bootsector files in bin format.
build/%.bin: bootsector/%.asm
	nasm $< -f bin -o $@

# Assembles `call_kernel.asm` in elf format.
build/call_kernel.o: bootsector/call_kernel.asm
	nasm $< -f elf -o $@

# Compiles all kernel files.
build/%.o: kernel/%.c
	# The `-g` flag compiles the files with debug information.
	i386-elf-gcc -g -ffreestanding -c $^ -o $@

# For debugging.
build/kernel.elf: build/call_kernel.o build/kernel_main.o build/ports.o build/screen.o build/util.o
	i386-elf-ld -o $@ -Ttext 0x1000 $^
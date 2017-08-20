# On OSX, this makefile requires a GCC cross-compiler - see 
# http://wiki.osdev.org/GCC_Cross-Compiler.

# `$@` refers to the output file.
# `$<` refers to the first input file.
# `$^` refers to all input files.

# We print a variable using `$(info [${VAR}])`.

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o} 

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb
# -g: Use debugging symbols in gcc
CFLAGS = -g

# Runs the operating system.
# The first rule is run by default.
run: clean os-image.bin
	qemu-system-i386 -fda os-image.bin

# Runs the operating system with a debugger.
debug: clean os-image.bin kernel.elf
	# The `-s` flag waits for a gdb connection on TCP port 1234.
	qemu-system-i386 -s -fda os-image.bin -d guest_errors,int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Deletes any existing build files.
clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o bootsector/*.bin bootsector/*.o cpu/*.o
	
# The targets below should not be invoked directly.

# Concatenates the bootsector and kernal binaries into a single operating system binary.
os-image.bin: bootsector/bootsector.bin kernel.bin
	cat $^ > os-image.bin

kernel.bin: bootsector/call_kernel.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# For debugging.
kernel.elf: bootsector/call_kernel.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

# Compiles all .c files in .o format.
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

# Assembles all .asm files in .o format.
%.o: %.asm
	nasm $< -f elf -o $@

# Assembles all .asm files in .bin format.
%.bin: %.asm
	nasm $< -f bin -o $@
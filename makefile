all: makeos

makeos:
	nasm -f bin os.asm -o os.bin
	qemu-system-i386 os.bin
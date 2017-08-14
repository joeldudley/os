# Table of Contents
* Running the OS
* Debugging the OS
* Inline assembly

# Running the OS

Run the operating system using `make`.

# Debugging the OS

* Run the debugger using `make debug`
* Set breakpoints using `b [LINE_NUMBER]`
* Continue to the next breakpoint using `continue`

# Assembly instructions

* `db` places the specified byte in the binary
* `dw` places the two specified bytes (i.e. a word) in the binary
* `equ` gives a symbolic name to a numeric constant
* `global` makes a symbol visible to the linker so that other object files can use it

# Inline assembly

In GCC extended inline assembly, the sections are separated by colons:
* The first section is the instruction
    * Registers are denoted by `%`, a character that we escape by writing "%%"
    * `out %%1 %%2` writes the contents of register 1 to port 2
    * `in %%1 %%2` stores the contents of port 1 in register 2
* The second section assigns the register's value to the C variable in brackets
* The third section assigns the value of the C variable in brackets to the register
* In the second and third sections:
    * `a` refers to registers %eax, %ax, and %al
    * `b` refers to registers %ebx, %bx, and %bl
    * `c` refers to registers %ecx, %cx, and %cl
    * `d` refers to registers %edx, %dx, and %dl
    * `S` refers to registers %esi and %si
    * `D` refers to registers %edi and %di
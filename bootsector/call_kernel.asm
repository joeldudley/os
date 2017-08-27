; This code is prepended to the kernel machine code, and causes us to jump into the kernel.

global _start
_start:
    ; Creates a reference to `main` in kernel/main.c.
    [extern kernel_main]
    ; Calls `main`.
    call kernel_main
    ; Loops forever.
    jmp $
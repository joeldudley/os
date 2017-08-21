; This code is prepended to the kernel machine code, and causes us to jump into the kernel.

[bits 32]
[extern main]   ; Creates a reference to `main` in kernel/main.c.
call main       ; Calls `main`.
jmp $           ; Loops forever.
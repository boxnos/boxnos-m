    .text

.globl io_out32
io_out32:
    mov %di, %dx
    mov %esi, %eax
    out %eax, %dx
    ret

.globl io_in32
io_in32:
    mov %di, %dx
    in %dx, %eax
    ret

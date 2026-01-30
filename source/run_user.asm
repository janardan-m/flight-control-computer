    .arm
    .global run_user
    .global g_run_user_lr
    .global g_user_sp_top

run_user:
    ; r0 = entry function pointer

    ; Save kernel return address
    ldr     r1, g_run_user_lr_addr
    str     lr, [r1]

    ; Load desired USER stack top from global variable
    ldr     r1, g_user_sp_top_addr
    ldr     r1, [r1]                ; r1 = g_user_sp_top

    ; Switch to USER mode
    cps     #16

    ; Initialize USER stack pointer immediately
    mov     sp, r1

    ; Now safe to branch to user code
    blx     r0

    ; Trap back to kernel (undefined instruction)
    .word   0xE7F000F0

g_run_user_lr_addr:
    .word   g_run_user_lr

g_user_sp_top_addr:
    .word   g_user_sp_top

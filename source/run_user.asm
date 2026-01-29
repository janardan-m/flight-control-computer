    .arm
    .global run_user
    .global g_run_user_lr     ; <-- REQUIRED for TI ASM

run_user:
    ; Load address of g_run_user_lr via literal pool
    ldr     r1, g_run_user_lr_addr

    ; Save kernel return address
    str     lr, [r1]

    ; Switch to USER mode
    cps     #16

    ; Call partition entry (r0 holds function pointer)
    blx     r0

    ; Undefined instruction to trap back
    .word   0xE7F000F0



; ---- literal pool ----
g_run_user_lr_addr:
    .word   g_run_user_lr

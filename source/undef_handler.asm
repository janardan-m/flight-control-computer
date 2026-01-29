    .arm
    .global _undefHandler
    .global g_run_user_lr     ; <-- REQUIRED for TI ASM

_undefHandler:
    ; Load address of g_run_user_lr
    ldr     r0, g_run_user_lr_addr

    ; Restore kernel LR
    ldr     lr, [r0]

    ; Read SPSR (saved USER CPSR)
    mrs     r0, SPSR

    ; Force SVC mode (0x13)
    bic     r0, r0, #0x1F
    orr     r0, r0, #0x13

    ; Enable IRQs
    bic     r0, r0, #0x80

    ; Write back SPSR
    msr     SPSR_cxsf, r0

    ; Proper exception return
    movs    pc, lr

; ---- literal pool ----
g_run_user_lr_addr:
    .word   g_run_user_lr

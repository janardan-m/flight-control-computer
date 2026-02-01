    .arm
    .global _pabort

    .ref g_run_user_lr
    .ref fault_report_pabort

_pabort:
    stmfd   sp!, {r0-r12, lr}

    ; Check origin mode
    mrs     r1, SPSR
    and     r1, r1, #0x1F
    cmp     r1, #0x10          ; USER mode?
    bne     pabort_privileged

    ; ---- USER prefetch abort ----
    mov     r0, lr             ; fault PC (approx)
    mrs     r1, SPSR
    bl      fault_report_pabort

    ldmfd   sp!, {r0-r12, lr}

    ; Return to kernel scheduler
    ldr     r2, g_run_user_lr_addr
    ldr     lr, [r2]
    movs    pc, lr

pabort_privileged:
    ; Kernel fault → fatal
pabort
	b      pabort

g_run_user_lr_addr:
    .word g_run_user_lr





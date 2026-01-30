    .arm
    .global svcEntry

    .ref vPortSWI                 ; FreeRTOS SVC handler (keep working)
    .ref kernel_svc_dispatch       ; our C dispatcher

; --------------------------------------------------------------------
; svcEntry
;  - Runs in SVC mode (privileged) automatically when USER executes SVC.
;  - Checks the SVC immediate value:
;       if SVC #0x7F  -> our syscall
;       else          -> chain to vPortSWI (FreeRTOS)
;
; ABI we implement for *our* syscall:
;   r12 = operation ID (what to do)
;   r0  = arg0
;   r1  = arg1
;   r2  = arg2
;   return value in r0
;
; We purposely keep only 3 args (r0-r2) to avoid 5th arg calling issues.
; If you need more, pass a pointer in arg0 to a struct in shared memory.
; --------------------------------------------------------------------

SVC_MAGIC_IMM     .set 0x7F

svcEntry:
    ; Save user registers we will touch + LR_svc
    ; Layout (offsets from SP after push):
    ;   [0]  r0
    ;   [4]  r1
    ;   [8]  r2
    ;   [12] r3
    ;   [16] r12
    ;   [20] lr
    stmdb   sp!, {r0-r3, r12, lr}

    ; Read the SVC instruction to get immediate:
    ; In ARM state, LR_svc points to next instruction (SVC + 4)
    ; So SVC instruction address = LR_svc - 4
    ldr     r0, [lr, #-4]
	bic     r0, r0, #0xFF000000

    ; Check if it’s our syscall SVC (#0x7F)
    cmp     r0, #SVC_MAGIC_IMM
    bne     chain_to_freertos

    ; --- Our syscall path ---
    ; Load saved values and call kernel_svc_dispatch(op, a0, a1, a2)
    ldr     r0, [sp, #16]             ; r0 = op (saved r12)
    ldr     r1, [sp, #0]              ; r1 = arg0 (saved r0)
    ldr     r2, [sp, #4]              ; r2 = arg1 (saved r1)
    ldr     r3, [sp, #8]              ; r3 = arg2 (saved r2)

    bl      kernel_svc_dispatch

    ; Return value is in r0. Put it into saved r0 slot so USER sees it.
    str     r0, [sp, #0]

    ; Restore user regs and return from exception to USER
    ldmia   sp!, {r0-r3, r12, lr}
    movs    pc, lr                    ; return to USER, restore CPSR from SPSR

chain_to_freertos:
    ; Restore registers exactly as they were
    ldmia   sp!, {r0-r3, r12, lr}
    ; Chain to FreeRTOS SVC handler
    b       vPortSWI






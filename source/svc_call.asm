    .arm
    .global svc_call

; uint32_t svc_call(uint32_t op,
;                   uint32_t a0,
;                   uint32_t a1,
;                   uint32_t a2)
;
; ABI:
;   r0 = op
;   r1 = a0
;   r2 = a1
;   r3 = a2
;
; On return:
;   r0 = return value

svc_call:
    ; Move op into r12 (our syscall ID register)
    mov     r12, r0

    ; Shift args to expected places
    mov     r0, r1
    mov     r1, r2
    mov     r2, r3

    ; Trigger our syscall
    svc     #0x7F

    ; Return to caller (r0 already has return value)
    bx      lr

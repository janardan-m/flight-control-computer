;-------------------------------------------------------------------------------
; dabort.asm
;
; Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
; 
; 
;  Redistribution and use in source and binary forms, with or without 
;  modification, are permitted provided that the following conditions 
;  are met:
;
;    Redistributions of source code must retain the above copyright 
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the 
;    documentation and/or other materials provided with the   
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;
;

    .text
    .arm


;-------------------------------------------------------------------------------
; Run Memory Test

	.ref g_run_user_lr
    .ref fault_report_dabort   ; optional C logger (you can stub)

    .ref	custom_dabort
    .def	_dabort
    .asmfunc

_dabort
        stmfd   r13!, {r0 - r12, lr}        ; save regs on ABORT stack

        ; ---- Check origin mode (SPSR_abt[4:0]) ----
        mrs     r1, SPSR                    ; r1 = SPSR_abt (CPSR of code that faulted)
        and     r1, r1, #0x1F               ; r1 = mode bits
        cmp     r1, #0x10                   ; 0x10 = USER mode
        bne     dabort_privileged           ; if not USER, keep TI behavior

        ; =========================================================
        ; USER-MODE DATA ABORT  => kill/suspend this partition
        ; and return to kernel scheduler (privileged).
        ; =========================================================

        ; OPTIONAL: log (safe minimal logger)
        ; r0 = lr_abt (return address near fault), r1 = SPSR_abt
        mov     r0, lr
        mrs     r1, SPSR
        bl      fault_report_dabort    ; if you don't want logging, delete these 3 lines

        ; Restore registers (so kernel doesn't see garbage in regs)
        ldmfd   r13!, {r0 - r12, lr}

        ; Switch to SVC mode (privileged). This changes SP to SP_svc (kernel stack).
        cps     #0x13

        ; Enable IRQs
        mrs     r0,CPSR
		bic     r0, r0, #0x80
		msr     CPSR_c, r0

        ; Jump back to kernel return point saved by run_user()
        ldr     r2, g_run_user_lr_addr
        ldr     lr, [r2]
        bx      lr                          ; continue kernel scheduler after run_user()

        ; ---- literal pool ----
g_run_user_lr_addr:
        .word   g_run_user_lr


dabort_privileged:
        ; =========================================================
        ; PRIVILEGED/KERNEL DATA ABORT => keep original TI logic
        ; =========================================================

        ; (fall-through into your existing TI code below)
        ldr     r12, esmsr3
        ldr     r0,  [r12]
        tst     r0,  #0x8
        bne     ramErrorFound
        tst     r0,  #0x20
        bne     ramErrorFound2


noRAMerror
		tst		r0, #0x80			; check if bit 7 is set, this indicates uncorrectable ECC error on ATCM
		bne		flashErrorFound

		bl		custom_dabort		; custom data abort handler required
									; If this custom handler is written in assembly, all registers used in the routine
									; and the link register must be saved on to the stack upon entry, and restored before
									; return from the routine.

		ldmfd	r13!, {r0 - r12, lr}; pop registers and link register from stack
		subs	pc, lr, #8			; restore state of CPU when abort occurred, and branch back to instruction that was aborted

ramErrorFound
		ldr		r1, ramctrl			; RAM control register for B0TCM TCRAMW
		ldr		r2, [r1]
		tst		r2, #0x100			; check if bit 8 is set in RAMCTRL, this indicates ECC memory write is enabled
		beq		ramErrorReal
		mov		r2, #0x20
		str		r2, [r1, #0x10]		; clear RAM error status register

		mov		r2, #0x08
		str		r2, [r12]			; clear ESM group3 channel3 flag for uncorrectable RAM ECC errors
		mov		r2, #5
		str		r2, [r12, #0x18]	; The nERROR pin will become inactive once the LTC counter expires

		ldmfd	r13!, {r0 - r12, lr}
		subs	pc, lr, #4			; branch to instruction after the one that caused the abort
									; this is the case because the data abort was caused intentionally
									; and we do not want to cause the same data abort again.

ramErrorFound2
		ldr		r1, ram2ctrl		; RAM control register for B1TCM TCRAMW
		ldr		r2, [r1]
		tst		r2, #0x100			; check if bit 8 is set in RAMCTRL, this indicates ECC memory write is enabled
		beq		ramErrorReal
		mov		r2, #0x20
		str		r2, [r1, #0x10]		; clear RAM error status register

		mov		r2, #0x20
		str		r2, [r12]			; clear ESM group3 flags channel5 flag for uncorrectable RAM ECC errors
		mov		r2, #5
		str		r2, [r12, #0x18]	; The nERROR pin will become inactive once the LTC counter expires

		ldmfd	r13!, {r0 - r12, lr}
		subs	pc, lr, #4			; branch to instruction after the one that caused the abort
									; this is the case because the data abort was caused intentionally
									; and we do not want to cause the same data abort again.


ramErrorReal
		b		ramErrorReal		; branch here forever as continuing operation is not recommended

flashErrorFound
		ldr		r1, flashbase
		ldr		r2, [r1, #0x6C]		; read FDIAGCTRL register

		mov     r2, r2, lsr #16
		tst		r2, #5				; check if bits 19:16 are 5, this indicates diagnostic mode is enabled
		beq		flashErrorReal
		mov		r2, #1
		mov     r2, r2, lsl #8		
		
		str		r2, [r1, #0x1C]		; clear FEDACSTATUS error flag

		mov		r2, #0x80
		str		r2, [r12]			; clear ESM group3 flag for uncorrectable flash ECC error
		mov		r2, #5
		str		r2, [r12, #0x18]	; The nERROR pin will become inactive once the LTC counter expires

		ldmfd	r13!, {r0 - r12, lr}
		subs	pc, lr, #4			; branch to instruction after the one that caused the abort
									; this is the case because the data abort was caused intentionally
									; and we do not want to cause the same data abort again.


flashErrorReal
		b		flashErrorReal		; branch here forever as continuing operation is not recommended
		
esmsr3		.word	0xFFFFF520
ramctrl		.word	0xFFFFF800
ram2ctrl	.word	0xFFFFF900
ram1errstat	.word	0xFFFFF810
ram2errstat	.word	0xFFFFF910
flashbase	.word	0xFFF87000

    .endasmfunc
	
	

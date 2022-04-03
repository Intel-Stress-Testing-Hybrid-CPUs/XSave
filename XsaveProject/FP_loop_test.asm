PUBLIC run_fpu
EXTERN puts:PROC

.data

    hello1 db "Hello from ASM.",0

.code

run_fpu PROC
    push rbp
    mov rbp, rsp
    sub rsp, 32                 ; Shadow Space
    and spl, -16                ; Align stack at 16

    lea rcx, hello1
    call puts

    leave                       ; Restore stack (rsp) & frame pointer (rbp)
    ret
run_fpu ENDP

END
PUBLIC run_fpu
EXTERN puts:PROC

.data

    hello1 db "Hello from ASM.",0
    const   dd 2000.0
    ThreeBytes db 10h,20h,30h
    a dd 1.234
   

.code
run_fpu PROC var1:DWORD

    

    push rbp
    mov rbp, rsp
    sub rsp, 32                 ; Shadow Space
    and spl, -16                ; Align stack at 16

    ;push ebp
    ;mov ebp, esp

    ;mov ebp, [ebp+8]

    lea rcx, hello1
    call puts

    leave                       ; Restore stack (rsp) & frame pointer (rbp)

    mov eax,edi

    ;movd xmm0, ebp
    ;movd xmm0, eax; load from memory
    movd xmm0, var1
    ; addss xmm0,xmm0 ; add to itself (double it)
    
    ;pop ebp
    ret

    
run_fpu ENDP

END
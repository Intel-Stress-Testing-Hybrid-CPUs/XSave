PUBLIC run_fpu
EXTERN puts:PROC

.data
    align 16
    hello1 db "Hello from ASM.",0
    const   dd 2000.0
    ThreeBytes db 10h,20h,30h
    aval dd 1.234, 2.3456, 3.4567, 4.5678 ; this is a one dimensional array
    value dd 1.0
    address dd 0
   

.code
run_fpu PROC

    ;mov ebp,esp
    ;mov eax,[ebp+8]          ;Put argument addresses to registers
    ;mov ebx,[ebp+12] 

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
    movd xmm0, eax; load from memory
    ; movdqa xmm0, xmmword ptr [ebx]
    ; movaps xmm0, [aval]
    addss xmm0,xmm0 ; add to itself (double it)
    
    ;pop ebp
    ret

    
run_fpu ENDP

END
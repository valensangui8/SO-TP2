
GLOBAL _cli
GLOBAL _sti
GLOBAL pic_master_mask
GLOBAL pic_slave_mask
GLOBAL halt_cpu
GLOBAL _hlt

GLOBAL _irq80Handler

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL save_state
GLOBAL print_reg_asm

GLOBAL _initialize_stack_frame

GLOBAL _yield

EXTERN idt_manager
EXTERN irq_dispatcher
EXTERN exception_dispatcher
EXTERN ret_userland
EXTERN get_stack_base
EXTERN print_registers
EXTERN scheduler

SECTION .text


%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro pushStateNoRax 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro dState 0
	mov [registers.drbp], rbp
	mov rbp, [rsp]
	mov [registers.dr15], rbp
	mov rbp, [rsp+8]
	mov [registers.dr14], rbp
	mov rbp, [rsp+16]
	mov [registers.dr13], rbp
	mov rbp, [rsp+24]
	mov [registers.dr12], rbp
	mov rbp, [rsp+32]
	mov [registers.dr11], rbp
	mov rbp, [rsp+40]
	mov [registers.dr10], rbp
	mov rbp, [rsp+48]
	mov [registers.dr9], rbp
	mov rbp, [rsp+56]
	mov [registers.dr8], rbp
	mov rbp, [rsp+64]
	mov [registers.drsi], rbp
	mov rbp, [rsp+72]
	mov [registers.drdi], rbp
	mov rbp, [rsp+88]
	mov [registers.drdx], rbp
	mov rbp, [rsp+96]
	mov [registers.drcx], rbp
	mov rbp, [rsp+104]
	mov [registers.drbx], rbp
	mov rbp, [rsp+112]
	mov [registers.drax], rbp
	mov rbp, [rsp+120]
	mov [registers.drip], rbp
	mov rbp, [rsp+128]
	mov [registers.dcs], rbp
	mov rbp, [rsp+136]
	mov [registers.drfl], rbp
	mov rbp, [rsp+144]
	mov [registers.drsp], rbp
	mov rbp, [rsp+152]
	mov [registers.dss], rbp
	mov rbp, [registers.drbp]
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro popStateNoRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

_yield:
	int 20h
	ret

save_state:
	pushState
	dState
	popState
	ret

print_reg_asm:
	mov qword rdi, registers
	call print_registers
	ret

%macro irqHandlerMaster 1
	pushState
	
	mov rdi, %1
	cmp rdi, 1
	jne .noSave
	dState
	
	.noSave:
	mov rsi, rsp
	mov rdi, %1 ; pasaje de parametro
	call irq_dispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
		
%endmacro



%macro exceptionHandler 1
	pushState
	dState

	mov rsi, registers
	;mov rsi, rsp
	mov rdi, %1 ; pasaje de parametro
	call exception_dispatcher

	popState

	call get_stack_base
	sub rax, 20h
	mov qword [rsp+8*3], rax
	call ret_userland
	mov qword [rsp], rax
	iretq
%endmacro

_initialize_stack_frame:
	mov r8, rsp 	; Preservar rsp
	mov r9, rbp		; Preservar rbp
	mov rsp, rdx 	; Carga sp del proceso
	mov rbp, rdx
	push 0x0
	push rdx
	push 0x202
	push 0x8
	push rdi
	mov rdi, rsi 		; Primer argumento de wrapper, RIP
	mov rsi, rcx		; Segundo argumento, args
	pushState
	mov rax, rsp
	mov rsp, r8
	mov rbp, r9
	ret


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

pic_master_mask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

pic_slave_mask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn



_irq80Handler:
	push rbp
    mov rbp, rsp
    pushStateNoRax

    ;parameters for systemcalls
    push r9
    push r8
    push rcx
    push rdx
    push rsi
    push rdi

    mov rdi, rax
    mov rsi, rsp   ; stack pointer is pointing towards first element in the stack
	call idt_manager
	add rsp, 8*6    ; Restablezco el stack

    popStateNoRax
    mov rsp, rbp
    pop rbp
    iretq

;8254 Timer (Timer Tick)
_irq00Handler:
	
	;irqHandlerMaster 0
	pushState

	mov rdi, 0 
	call irq_dispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

halt_cpu:
	cli
	hlt
	ret



SECTION .bss
	aux resq 1

	GLOBAL registers
	registers:
	.drax resq 1
	.drbx resq 1
	.drcx resq 1
	.drdx resq 1
	.drsi resq 1
	.drdi resq 1
	.drsp resq 1
	.drbp resq 1
	.dr8  resq 1
	.dr9  resq 1
	.dr10 resq 1
	.dr11 resq 1
	.dr12 resq 1
	.dr13 resq 1
	.dr14 resq 1
	.dr15 resq 1
	.dss  resq 1
	.dcs  resq 1
	.drfl resq 1
	.drip resq 1
	
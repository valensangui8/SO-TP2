GLOBAL cpuVendor
GLOBAL get_key
GLOBAL outb
GLOBAL inb
GLOBAL timer_tick

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

get_key:
	push rbp
	mov rbp,rsp
	mov rax,0
	in al,0x60
	mov rsp,rbp
	pop rbp
	ret

inb:                ; functions inb and outb to access the in/out map for the audio
    push rbp
    mov rbp, rsp

    mov rdx,rdi
    in al,dx        

    mov rsp, rbp
    pop rbp
    ret

outb:
    push rbp
    mov rbp, rsp

    mov rax, rsi
    mov rdx, rdi
    out dx, al

    mov rsp, rbp
    pop rbp
    ret

timer_tick:
	int 0x20
	ret

section .data
timeval:
	tv_sec dd 0
	tv_usec dd 0
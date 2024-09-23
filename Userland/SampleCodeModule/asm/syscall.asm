section .text
GLOBAL call_sys_drawWord
GLOBAL call_sys_read
GLOBAL call_sys_drawChar
GLOBAL call_sys_enter
GLOBAL call_sys_delete
GLOBAL call_sys_drawError
GLOBAL call_sys_commandEnter
GLOBAL call_sys_zoomIn
GLOBAL call_sys_zoomOut
GLOBAL call_sys_get_date
GLOBAL call_sys_clear
GLOBAL call_sys_getScale
GLOBAL call_sys_drawWithColor
GLOBAL call_sys_drawRegisters
GLOBAL call_sys_draw
GLOBAL call_sys_sleep
GLOBAL call_sys_sound
GLOBAL call_sys_checkHeight
call_sys_read:
    mov rax, 0
    int 80h
    ret
call_sys_drawWord:
    mov rax, 1
    int 80h
    ret

call_sys_drawChar:
    mov rax, 2
    int 80h
    ret

call_sys_enter:
    mov rax, 3
    int 80h
    ret

call_sys_delete:
    mov rax, 4
    int 80h
    ret

call_sys_drawError:
    mov rax, 5
    int 80h
    ret

call_sys_commandEnter:
    mov rax, 6
    int 80h
    ret

call_sys_zoomIn:
    mov rax, 7
    int 80h
    ret

call_sys_zoomOut:
    mov rax, 8
    int 80h
    ret

call_sys_clear:
    mov rax, 9
    int 80h
    ret

call_sys_getScale:
    mov rax, 10
    int 80h
    ret

call_sys_drawWithColor:
    mov rax, 11
    int 80h
    ret

call_sys_drawRegisters:
    mov rax, 12
    int 80h
    ret

call_sys_draw:
    mov rax, 13
    int 80h
    ret

call_sys_sleep:
    mov rax, 14
    int 80h
    ret

call_sys_get_date:
    push rbp
    mov rbp, rsp

    mov rax, rdi
    out 70h, al
    xor rax, rax
    in al, 71h

    mov rsp, rbp
    pop rbp
    ret

call_sys_sound:
    mov rax, 15
    int 80h
    ret


call_sys_checkHeight:
    mov rax, 16
    int 80h
    ret
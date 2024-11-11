section .text
GLOBAL call_sys_draw_word
GLOBAL call_sys_error
GLOBAL call_sys_read
GLOBAL call_sys_draw_char
GLOBAL call_sys_enter
GLOBAL call_sys_delete
GLOBAL call_sys_draw_error
GLOBAL call_sys_command_enter
GLOBAL call_sys_zoom_in
GLOBAL call_sys_zoom_out
GLOBAL call_sys_get_date
GLOBAL call_sys_clear
GLOBAL call_sys_get_scale
GLOBAL call_sys_draw_with_color
GLOBAL call_sys_draw_registers
GLOBAL call_sys_draw
GLOBAL call_sys_sleep
GLOBAL call_sys_sound
GLOBAL call_sys_check_height

GLOBAL call_sys_kill_process
GLOBAL call_sys_update_priority
GLOBAL call_sys_block_process
GLOBAL call_sys_unblock_process
GLOBAL call_sys_yield
GLOBAL call_sys_process_status
GLOBAL call_sys_create_process
GLOBAL call_sys_list_processes_state
GLOBAL call_sys_get_pid
GLOBAL call_sys_get_ppid
GLOBAL call_sys_wait_children
GLOBAL call_sys_draw_int

GLOBAL call_sys_halt
GLOBAL call_sys_alloc_memory
GLOBAL call_sys_free_memory
GLOBAL call_sys_sem_open
GLOBAL call_sys_sem_wait
GLOBAL call_sys_sem_post
GLOBAL call_sys_sem_close

GLOBAL call_sys_get_pipe_fd
GLOBAL call_sys_open_pipe
GLOBAL call_sys_close_pipe
GLOBAL call_sys_write_pipe
GLOBAL call_sys_read_pipe

GLOBAL call_sys_get_memory_info
GLOBAL call_sys_foreground

call_sys_read:
    mov rax, 0
    int 80h
    ret
call_sys_draw_word:
    mov rax, 1
    int 80h
    ret

call_sys_error:
    mov rax, 1
    int 80h
    ret

call_sys_draw_char:
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

call_sys_draw_error:
    mov rax, 5
    int 80h
    ret

call_sys_command_enter:
    mov rax, 6
    int 80h
    ret

call_sys_zoom_in:
    mov rax, 7
    int 80h
    ret

call_sys_zoom_out:
    mov rax, 8
    int 80h
    ret

call_sys_clear:
    mov rax, 9
    int 80h
    ret

call_sys_get_scale:
    mov rax, 10
    int 80h
    ret

call_sys_draw_with_color:
    mov rax, 11
    int 80h
    ret

call_sys_draw_registers:
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


call_sys_check_height:
    mov rax, 16
    int 80h
    ret


call_sys_kill_process:
    mov rax, 17
    int 80h
    ret

call_sys_update_priority:
    mov rax, 18
    int 80h
    ret

call_sys_block_process:
    mov rax, 19
    int 80h
    ret

call_sys_unblock_process:
    mov rax, 20
    int 80h
    ret

call_sys_yield:
    mov rax, 21
    int 80h
    ret

call_sys_process_status:
    mov rax, 22
    int 80h
    ret

call_sys_create_process:
    mov rax, 23
    int 80h
    ret

call_sys_list_processes_state:
    mov rax, 24
    int 80h
    ret
    

call_sys_get_pid:
    mov rax, 25
    int 80h
    ret

call_sys_get_ppid;
    mov rax, 26
    int 80h
    ret

call_sys_wait_children:
    mov rax, 27
    int 80h
    ret

call_sys_draw_int:
    mov rax, 28
    int 80h
    ret

call_sys_halt:
    mov rax, 29
    int 80h
    ret

call_sys_alloc_memory:
    mov rax, 30
    int 80h
    ret

call_sys_free_memory:
    mov rax, 31
    int 80h
    ret

call_sys_sem_open:
    mov rax, 32
    int 80h
    ret

call_sys_sem_wait:
    mov rax, 33
    int 80h
    ret

call_sys_sem_post:
    mov rax, 34
    int 80h
    ret

call_sys_sem_close:
    mov rax, 35
    int 80h
    ret

call_sys_get_pipe_fd:
    mov rax, 36
    int 80h
    ret

call_sys_open_pipe:
    mov rax, 37
    int 80h
    ret

call_sys_close_pipe:
    mov rax, 38
    int 80h
    ret

call_sys_write_pipe:
    mov rax, 39
    int 80h
    ret

call_sys_read_pipe:
    mov rax, 40
    int 80h
    ret

call_sys_get_memory_info:
    mov rax, 41
    int 80h
    ret

call_sys_foreground:
    mov rax, 42
    int 80h
    ret
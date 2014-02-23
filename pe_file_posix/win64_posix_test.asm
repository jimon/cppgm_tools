; win64 posix dll test

format PE64 console
entry start

section '.text' code readable executable

start:
	sub rsp,8*5 ; reserve stack for API use and make stack dqword aligned

	mov rcx, 1;
	mov rdx, 1;
	lea r8, [_message]
	mov r9, 5;
	call [syscall3]

	lea rcx, [_message]
	call [syscall_msg]

	mov rcx, 60; //exit
	mov rdx, 0;
	call [syscall0]

section '.data' data readable writeable
	_message db 'Hello World!', 0

section '.idata' import data readable writeable
	dd 0,0,0,RVA syscall_name, RVA syscall_table
	dd 0,0,0,0,0

	syscall_table:
		syscall0 dq RVA _syscall0
		syscall1 dq RVA _syscall1
		syscall2 dq RVA _syscall2
		syscall3 dq RVA _syscall3
		syscall4 dq RVA _syscall4
		syscall5 dq RVA _syscall5
		syscall6 dq RVA _syscall6
		syscall_msg dq RVA _syscall_msg
		dq 0

	syscall_name db 'win64_posix.dll', 0

	_syscall0 dw 0
		db 'syscall0',0
	_syscall1 dw 0
		db 'syscall1',0
	_syscall2 dw 0
		db 'syscall2',0
	_syscall3 dw 0
		db 'syscall3',0
	_syscall4 dw 0
		db 'syscall4',0
	_syscall5 dw 0
		db 'syscall5',0
	_syscall6 dw 0
		db 'syscall6',0
	_syscall_msg dw 0
		db 'syscall_msg',0
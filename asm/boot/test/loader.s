.code16
.text
.global start
start:
	cli
	mov      %cs, %ax
	mov      %ax, %ds
	mov      %ax, %es
	mov      %ax, %ss
	mov      $stack, %ax
	mov      %ax, %sp
	sti

	mov      $msg_loader_started, %si
	call     print
1:
	jmp      1b

msg_loader_started: .ascii "Loader started!"
stack:              .space 512


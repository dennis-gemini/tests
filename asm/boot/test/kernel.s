.code16
.text
start:
	mov      %cs, %ax
	mov      %ax, %ds
	mov      %ax, %ss
	mov      $top_of_stack, %ax
	mov      %ax, %sp

	mov      $0xb800, %ax
	mov      %ax, %es
	xor      %di, %di
	mov      $(80 * 25), %cx

	mov      $0x0F, %ah
	mov      $'.', %al

	cld
	rep      stosw

	mov      $msg_loader_started, %si
	call     print
1:
	jmp      1b

msg_loader_started: .ascii "Hello, world!\r\n\0"
stack: .space 1024
top_of_stack:

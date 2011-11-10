.code16
.text
start:
	mov      %cs, %ax
	mov      %ax, %ds
	mov      %ax, %es
	mov      %ax, %ss
	mov      $top_of_stack, %ax
	mov      %ax, %sp

	mov      $msg_loader_started, %si
	call     print

	cli
	lidtl    idt_desc
	lgdtl    gdt_desc

	#for 386
	mov      %cr0, %eax
	or       $1, %eax
	mov      %eax, %cr0

	#for 286
#	smsw     %ax
#	or       $1, %ax
#	lmsw     %ax

	ljmpl    $8, start32

.code   32
.balign 4
start32:

1:
	hlt
	jmp      1b

.code 16
msg_loader_started: .ascii "Hello, Real Mode!\r\n\0"
stack: .space 1024
top_of_stack:

.balign 4
gdt_desc:
	.word	 
	.long

.balign 4
idt_desc:
	.word 0
	.word 0, 0


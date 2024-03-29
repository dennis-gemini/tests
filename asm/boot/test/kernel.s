#
# type := [Pr Priv*2 1 Ex DC RW Ac]
#
# Pr   := {0: absent, 1: present}
#
# Priv := {0:ring 0 (highest), 1:ring 1, 2:ring 2, 3:ring 3(lowest)}
#
# Ex   := {1:exec, 0:data}
#
# DC   := direction bit for data {
#		0:grows up,
#		1:grows down (offset >= base)
#	  }
#	  confirming bit for code {
#		0: executed with cpl = Priv,
#		1: executed with cpl <= Priv,
#	  }
#
# RW   := readable for code selectors {
#		1: readable (write access is never allowed for code segment)
#	  }
#	  writable for data selectors {
#		1: writable (read access is always allowed for data segment)
#	  }
#
# Ac   := {0: initially set, not-accessed, 1: set by CPU if accessed}

#
# flags := [Gr Sz 00 0000]
#
# Gr    := {0: 1B, 1: 4K}
#
# Sz    := {0: 16bit, 1: 32bit}
#

.macro GDT_ENTRY base, limit, type, flags
	.byte ((\limit) >>  0) & 0xff
	.byte ((\limit) >>  8) & 0xff
	.byte ((\base ) >>  0) & 0xff
	.byte ((\base ) >>  8) & 0xff
	.byte ((\base ) >> 16) & 0xff
	.byte ((\type ) >>  0) & 0xff
	.byte ((\limit) >> 16) & 0x0f | ((\flags ) & 0xf0)
	.byte ((\base ) >> 24) & 0xff
.endm

.text
.code16
start16:
	mov      %cs, %ax
	mov      %ax, %ds
	mov      %ax, %es
	mov      %ax, %ss
	mov      $top_of_stack, %ax
	mov      %ax, %sp

	mov      $msg_loader_started, %si
	call     print

	#########################################
	# Fast A20 gate
	inb      $0x92, %al
	or       $2, %al
	outb     %al, $0x92

	# Use BIOS INT15, 2401
#	mov      $0x2401, %ax
#	int      $0x15

	# Alternative way to turn on A20
#1:	inb      $0x64, %al
#	testb    $0x02, %al
#	jnz      1b
#
#	movb     $0xd1, %al
#	outb     %al, $0x64
#
#2:	inb      $0x64, %al
#	testb    $0x02, %al
#	jnz      2b
#
#	movb     $0xdf, %al
#	outb     %al, $0x60

	#########################################
	# Load GDT and IDT
#	xor      %eax, %eax
#	mov      %ds, %ax
#	shl      $4, %eax
#	addl     $gdt_begin, %eax
#	mov      %eax, gdt_base

	xorw     %ax, %ax
	movw     %ax, %ds
	movw     %ax, %es
	movw     %ax, %ss

	cli
	cld
	lidtw    idt_desc
	lgdtw    gdt_desc

	#########################################
	# Enter protected mode
	# for 386
	mov      %cr0, %eax
	or       $1, %eax
	mov      %eax, %cr0

	# for 286
#	smsw     %ax
#	or       $1, %ax
#	lmsw     %ax

	#########################################
	# Align segment registers
	ljmpl    $0x08, $start32
	#########################################

.code32
start32:
	mov      $0x10, %ax
	mov      %ax, %ds
	mov      %ax, %es
	mov      %ax, %fs
	mov      %ax, %gs
	mov      %ax, %ss

	movb     $'*', %al
	movb     $0x0C, %ah
#	movl     $0xb8000, %ebx
#	movw     %ax, %es: (%ebx)

	movl     $(80 * 25), %ecx
	movl     $0xb8000, %edi
	cld
	repnz
	stosw

1:
	nop
	jmp      1b

.code16
msg_loader_started: .ascii "Hello, Real Mode!\r\n\0"
stack: .space 1024
top_of_stack:

.balign 4
gdt_begin:
	GDT_ENTRY 0x00000000, 0x00000, 0x00, 0x00	#0x00 dummy segment
	GDT_ENTRY 0x00000000, 0xfffff, 0x9a, 0xc0	#0x08 code segment
	GDT_ENTRY 0x00000000, 0xfffff, 0x92, 0xc0	#0x10 data segment
gdt_end:

gdt_desc:
gdt_len: .word (gdt_end - gdt_begin - 1)
gdt_base:.long gdt_begin

.balign 4
idt_desc:
	.word 0
	.long 0


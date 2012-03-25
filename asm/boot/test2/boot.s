#
# boot.s
#
#	boot into protected mode first
#

.include "boot.inc"
.include "gdt.inc"

.section .text
.code16
.global boot_start
boot_start:
	# Disable interrupt
	cli

	# Enable A20 line
	BOOT_ENABLE_A20_FAST
	#BOOT_ENABLE_A20_BIOS
	#BOOT_ENABLE_A20_SLOW

	# Load GDT descriptor
	xorw	%ax, %ax
	movw	%ax, %ds
	lgdt    boot_gdt_desc

	# Enter protected mode
	BOOT_ENTER_PROTECTED_MODE_I386
	#BOOT_ENTER_PROTECTED_MODE_I286

	ljmp    $0x08, $boot_start32

.code32
boot_start32:
	# Align data segment registers
	movw    $0x10, %ax
	movw    %ax, %ds
	movw    %ax, %es
	movw    %ax, %fs
	movw    %ax, %gs
	movw    %ax, %ss
	jmp     loader_start

#
# Default GDT descriptor for boot
#
.balign 4
boot_gdt_base:
	GDT_ENTRY_NULL                             #0x00
	GDT_ENTRY 0x00000000, 0xfffff, 0x9a, 0xc0  #0x08
	GDT_ENTRY 0x00000000, 0xfffff, 0x92, 0xc0  #0x10

boot_gdt_desc:
	.word   (boot_gdt_desc - boot_gdt_base - 1)
	.long   boot_gdt_base


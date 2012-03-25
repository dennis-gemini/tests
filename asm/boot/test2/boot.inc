#
# boot.inc
#
#

.macro BOOT_ENABLE_A20_FAST
	inb     $0x92, %al
	orb     $0x02, %al
	outb    %al, $0x92
.endm

.macro BOOT_ENABLE_A20_BIOS
	movw    $0x2401, %ax
	int     $0x15
.endm

.macro BOOT_ENABLE_A20_SLOW
1:	inb     $0x64, %al	# check busy state
	testb   $0x02, %al
	jnz     1b

	movb    $0xd1, %al
	outb    %al, $0x64

2:	inb     $0x64, %al
	testb   $0x02, %al
	jnz     2b

	movb    $0xdf, %al
	outb    %al, $0x60
.endm

.macro BOOT_ENTER_PROTECTED_MODE_I386
	movl    %cr0, %eax
	orl     $1, %eax
	movl    %eax, %cr0
.endm

.macro BOOT_ENTER_PROTECTED_MODE_I286
	smsw    %ax
	or      $1, %ax
	lmsw    %ax
.endm


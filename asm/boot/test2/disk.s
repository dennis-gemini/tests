#
# disk.s
#
#	Direct disk I/O handling
#

.include "disk.inc"

.text
.code32

#
# Input Regs:
# 	%dx: bus
#
# Output Regs:
#	%al: 0 = ready, 1 = error
#

.global ata_wait
ata_wait:
	add     $(ATA_PORT_COMMAND), %dx
1:	inb     %dx, %al
	testb   $0x80, %al               # command is executing
	jz      2f
	testb   $0x01, %al               # previous error has occurred
	jz      3f
	testb   $0x08, %al               # sector buffer is requiring servicing
	jz      3f
2:	pause
	jmp     1b
3:
	sub     $(ATA_PORT_COMMAND), %dx
	andb    $0x01, %al
	ret

#
# Input Regs:
#	%dx:  bus
#       %ebx: lba offset + drive + lba-/chs-mode 
#	%ecx: count in sectors
#	%edi: buffer
#
# Output Regs:
#	None
#

.global ata_readsector
ata_readsector:
	push    %ebx
	push    %ecx
	push    %edx
	push    %edi

	call    ata_wait

	andl    $0x0000000f, %ecx
	movb    %cl, %al
	DISK_OUTB ATA_PORT_SECTOR_COUNT

	movb    %bl, %al
	DISK_OUTB ATA_PORT_SECTOR

	shr     $8, %ebx
	movb    %bl, %al
	DISK_OUTB ATA_PORT_CYLINDER_LOW

	shr     $8, %ebx
	movb    %bl, %al
	DISK_OUTB ATA_PORT_CYLINDER_HIGH

	shr     $8, %ebx
	movb    %bl, %al
	DISK_OUTB ATA_PORT_DRIVE_SELECT

	call    ata_wait

	# read sectors into buffer
	shl     $7, %ecx	# %ecx * (512 / sizeof(int32)) = %ecx * 128 = %ecx * 2^7
	cld
	repnz
	insl

	push    %edi
	push    %edx
	push    %ecx
	push    %ebx
	ret

#
#
#
#
#

.global fdc_wait
fdc_wait:

	ret

#
#
#
#
#

.global fdc_readsector
fdc_readsector:

	ret


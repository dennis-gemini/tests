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
# Input Regs:
#	%dx:  bus
#	%ah:  flags
#
# Output Regs:
#	None
#

.global fdc_wait
fdc_wait:
	push    %eax
	addw    $(FDC_PORT_MSR), %dx

1:	inb     %dx, %al
	andb    %ah, %al
	cmpb    %ah, %al
	jnz     1b

	subw    $(FDC_PORT_MSR), %dx
	pop     %eax
	ret

#
# Input Regs:
#	%dx:  bus
#	%al:  command
#
# Output Regs:
#	None
#

.global fdc_command
fdc_command:
	push    %eax

	movb    $(FDC_MASK_MSR_RQM | FDC_MASK_MSR_CB), %ah
	call    fdc_wait

	DISK_OUTB FDC_PORT_DATA

	push    %eax
	ret

#
# Input Regs:
#	%dx:  bus
#
# Output Regs:
#	None
#

fdc_check_interrupt_status:
	push    %eax

	movb    $(FDC_CMD_CHECK_INTERRUPT_STATUS), %al
	call    fdc_command

	movb    $(FDC_MASK_MSR_RQM | FDC_MASK_MSR_DIO | FDC_MASK_MSR_CB), %ah

	call    fdc_wait
	DISK_INB FDC_PORT_DATA

	call    fdc_wait
	DISK_INB FDC_PORT_DATA

	pop     %eax
	ret

#
# Input Regs:
#	%dx:  bus
#
# Output Regs:
#	None
#

fdc_configure_drive:
	push    %eax

	# reset config control register
	xorb    %al, %al
	DISK_OUTB FDC_PORT_CCR

	movb    $(FDC_CMD_FIX_DRIVE_DATA), %al
	call    fdc_command

	movb    FDC_PARAM_STEPRATE_HEADUNLOAD, %al
	call    fdc_command

	movb    FDC_PARAM_HEADLOAD_NDMA, %al
	call    fdc_command

	pop     %eax
	ret

#
# Input Regs:
#	%dx:  bus
#	%al:  drive
#
# Output Regs:
#	None
#
fdc_calibrate_drive:
	push    %eax
	push    %ecx
	movb    %al, %ah
	movb    %al, %cl

	# turn on motor
	movb    $(FDC_MASK_DOR_MOTA), %al
	shl     %cl, %al
	orb     %ah, %al
	DISK_OUTB FDC_PORT_DOR

	# wait for command to be done
	movb    $(FDC_MASK_MSR_CB), %ah
	call    fdc_wait

	movb    $(FDC_CMD_CALIBRATE_DRIVE), %al
	call    fdc_command

	movb    %ah, %al
	call    fdc_command

	call    fdc_check_interrupt_status

	# turn off motor
	DISK_OUTB FDC_PORT_DOR

	pop     %ecx
	pop     %eax
	ret

#
# Input Regs:
#	%dx:  bus
#	%al:  drive
#
# Output Regs:
#	None
#

.global fdc_reset
	push    %eax

	# turn off all bits of digital output register
	andb    $(FDC_MASK_DOR_DSEL), %al
	mov     %al, %ah
	DISK_OUTB FDC_PORT_DOR

	# turn on reset bit to digital output register
	orb     $(FDC_MASK_DOR_RESET), %al
	DISK_OUTB FDC_PORT_DOR

	# wait for command to be done
	movb    $(FDC_MASK_MSR_CB), %ah
	call    fdc_wait

	mov     %ah, %al
	call    fdc_check_interrupt_status
	call    fdc_configure_drive
	call    fdc_calibrate_drive

	pop     %eax
	ret

#
#
#
#
#

.global fdc_readsector
fdc_readsector:

	ret


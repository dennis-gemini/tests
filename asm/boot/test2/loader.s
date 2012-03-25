#
# loader.s
#
#
#

.include "disk.inc"

.section .text
.code32
.global loader_start
loader_start:
	# save drive number: 0x00=fda, 0x01=fdb, 0x80=hda, 0x81=hdb
	movb    %dl, (bs_drvnum)

#	movw    $(ATA_BUS_PRIMARY), %dx
#	movl    $0, %ebx
#	orl     $(ATA_DRIVE_MASTER), %ebx
#	orl     $(ATA_MODE_LBA28), %ebx
#	movl    $10000, %edi     
#
#	call    disk_readsector

	movb    $0x0c, %ah
	movb    $'A', %al
	movl    $(80 * 25), %ecx
	movl    $0xb8000, %edi

	cld
	repnz
	stosw

	jmp     .


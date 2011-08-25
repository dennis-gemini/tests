.code16
.text
	jmp start
	nop
oem_name:		.ascii "-DENNIS-"
byte_per_sector:	.2byte 512
sector_per_cluster:	.byte  1
reserved_sector_count:	.2byte 1
num_of_fat:		.byte  2
root_entry_count:	.2byte 224
total_sector_16:	.2byte 2880
media:			.byte  0xf0
fat_size_16:		.2byte 9
sector_per_track:	.2byte 18
number_of_heads:	.2byte 2
hidden_sector:		.4byte 0
total_sector_32:	.4byte 0
drive_number:		.byte  0
reserved1:		.byte  0
boot_signature:		.byte  0x29
volume_id:		.4byte 0
volume_label:		.ascii "BootableDsk"
file_system_type:	.ascii "FAT12   "

start:
	mov  %cs, %ax
	mov  %ax, %ds
	mov  %ax, %es

	mov $booting, %ax
	mov $booting_len, %cx
	mov $0007, %bx
	mov $0000, %dx
	call print

	mov $0xb800, %ax
	mov %ax, %es
	movw %es:((80*12 + 40) * 2), %ax
	movb $0x71, %ah
	inc %al
	movw %ax, %es:((80*12 + 40) * 2)

halt:
	cli
	hlt
	jmp halt
//	mov $0xf000, %ax
//	mov %ax, %es
//	xor %bx, %bx
//	ljmp $0xffff, $0

print:
	//%ax: str
	//%cx: len
	//%bh: page
	//%bl: color
	//%dh: y
	//%dl: x
	mov %ax, %bp
	mov $0x1301, %ax
	int $0x10
	ret

booting:	.ascii	"Booting..."
.set booting_len, (. - booting)

.org	510
.word	0xaa55


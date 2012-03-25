#
# disk.inc
#
#
#

#
# See reference at: http://wiki.osdev.org/ATA_PIO_Mode
#
#  Port    Read/Write   Description
#------  ------------ -------------------------------------------------
#   1f0       r/w       data register, the bytes are written/read here
#   1f1       r         error register
#   1f2       r/w       sector count for reading/writing
#   1f3       r/w       sector number
#   1f4       r/w       cylinder low (0-1024)
#   1f5       r/w       cylinder high
#   1f6       r/w       drive/head
#                         bit 7-5 = 111b: 28-bit LBA, 010b: 48-bit LBA, 101b: CHS
#                         bit 4   = 0/1 drive number (0 or 1)
#                         bit 3-0 =     head number
#   1f7       r         status register
#                         bit 7   = 1   controller is executing a command
#                         bit 6   = 1   drive is ready
#                         bit 5   = 1   write fault
#                         bit 4   = 1   seek complete
#                         bit 3   = 1   sector buffer requires servicing
#                         bit 2   = 1   disk data read corrected
#                         bit 1   = 1   index - set to 1 each revolution
#                         bit 0   = 1   previous command ended in an error
#   1f7       w         command register
#                         commands:
#                           50h format track
#                           20h read sectors with retry
#                           21h read sectors without retry
#                           22h read long with retry
#                           23h read long without retry
#                           30h write sectors with retry
#                           31h write sectors without retry
#                           32h write long with retry
#                           33h write long without retry
#

.set ATA_BUS_PRIMARY,          0x1f0
.set ATA_BUS_SECONDARY,        0x170

.set ATA_PORT_DATA,            0x0
.set ATA_PORT_FEATURES,        0x1
.set ATA_PORT_SECTOR_COUNT,    0x2
.set ATA_PORT_SECTOR,          0x3
.set ATA_PORT_CYLINDER_LOW,    0x4
.set ATA_PORT_CYLINDER_HIGH,   0x5
.set ATA_PORT_DRIVE_SELECT,    0x6
.set ATA_PORT_COMMAND,         0x7

.set ATA_MODE_LBA28,           0xe0
.set ATA_MODE_LBA48,           0x40
.set ATA_MODE_CHS,             0xa0

.set ATA_DRIVE_MASTER,         0x00
.set ATA_DRIVE_SLAVE,          0x10

.set ATA_CMD_READSECTOR,       0x20
.set ATA_CMD_WRITESECTOR,      0x30

#
# Input Regs:
#	None
#
# Output Regs:
#	None
#
.macro DISK_OUTB	port
	addw    $(\port), %dx
	outb    %al, %dx
	subw    $(\port), %dx
.endm

#
# Input Regs:
# 	%dx: bus
#
# Output Regs:
#	None
#
.macro DISK_PREPARE_READSECTOR_LBA28	drive, offset, count
	DISK_OUTB ATA_PORT_DRIVE_SELECT , ((\offset >> 24) & 0xf) | ((\drive & 0x1) << 4) | ATA_MODE_LBA28
	DISK_OUTB ATA_PORT_SECTOR_COUNT , \count
	DISK_OUTB ATA_PORT_SECTOR       , (\offset >>  0) & 0xff
	DISK_OUTB ATA_PORT_CYLINDER_LOW , (\offset >>  8) & 0xff
	DISK_OUTB ATA_PORT_CYLINDER_HIGH, (\offset >> 16) & 0xff
	DISK_OUTB ATA_PORT_COMMAND      , ATA_CMD_READSECTOR
.endm


#
# disk.inc
#
#	Manipulate file access to hard disk and floppy disk
#

# 
# ATA PIO
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

#############################################################################

#
# Floppy PIO
#
# See reference at: http://wiki.osdev.org/Floppy_Disk_Controller
#

.set FDC_BUS_PRIMARY,          0x3f0
.set FDC_BUS_SECONDARY,        0x370

.set FDC_PORT_STATUS_A,        0x0	# status register a (read-only)
.set FDC_PORT_STATUS_B,        0x1	# status register b (read-only)
.set FDC_PORT_DOR,             0x2	# digital output register, which controls floppy drive motors, floppy drive selection, and resets.
.set FDC_PORT_TDR,             0x3	# tape drive register
.set FDC_PORT_MSR,             0x4	# main status register (read-only), which should be checked before each reading/writing with FIFO.
.set FDC_PORT_DSR,             0x4	# data-rate select register (write-only)
.set FDC_PORT_DATA_FIFO,       0x5	# data fifo
.set FDC_PORT_DIR,             0x7	# digital input register (read-only)
.set FDC_PORT_CCR,             0x7	# configuration control register (write-only)

.set FDC_MASK_DOR_DSEL,        0x03	# select drive number for next access
.set FDC_MASK_DOR_RESET,       0x04     # 0: enter reset mode, 1: normal operation
.set FDC_MASK_DOR_IRQ,         0x08     # 1: enable IRQ and DMA
.set FDC_MASK_DOR_MOTA,        0x10     # 1: turn on drive 0's motor
.set FDC_MASK_DOR_MOTB,        0x20     # 1: turn on drive 1's motor
.set FDC_MASK_DOR_MOTC,        0x40     # 1: turn on drive 2's motor
.set FDC_MASK_DOR_MOTD,        0x80     # 1: turn on drive 3's motor

.set FDC_MASK_MSR_ACTA,        0x01	# 1: Drive 0 is seeking
.set FDC_MASK_MSR_ACTB,        0x02	# 1: Drive 1 is seeking
.set FDC_MASK_MSR_ACTC,        0x04	# 1: Drive 2 is seeking
.set FDC_MASK_MSR_ACTD,        0x08	# 1: Drive 3 is seeking
.set FDC_MASK_MSR_CB,          0x10	# 1: Command busy (received), 0: end of result phase
.set FDC_MASK_MSR_NDMA,        0x20	# 1: set in execution phase of PIO mode read/write commands only
.set FDC_MASK_MSR_DIO,         0x40	# 1: set if fifo io port expects an IN opcode
.set FDC_MASK_MSR_RQM,         0x80	# 1: set if it's ok to exchange bytes with the fifo io port

#############################################################################

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


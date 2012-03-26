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

.set ATA_BUS_PRIMARY,                  0x1f0
.set ATA_BUS_SECONDARY,                0x170

.set ATA_PORT_DATA,                    0x0
.set ATA_PORT_FEATURES,                0x1
.set ATA_PORT_SECTOR_COUNT,            0x2
.set ATA_PORT_SECTOR,                  0x3
.set ATA_PORT_CYLINDER_LOW,            0x4
.set ATA_PORT_CYLINDER_HIGH,           0x5
.set ATA_PORT_DRIVE_SELECT,            0x6
.set ATA_PORT_COMMAND,                 0x7

.set ATA_MODE_LBA28,                   0xe0
.set ATA_MODE_LBA48,                   0x40
.set ATA_MODE_CHS,                     0xa0

.set ATA_DRIVE_MASTER,                 0x00
.set ATA_DRIVE_SLAVE,                  0x10

.set ATA_CMD_READSECTOR,               0x20
.set ATA_CMD_WRITESECTOR,              0x30

#############################################################################

#
# Floppy PIO
#
# See reference at:
#	http://wiki.osdev.org/Floppy_Disk_Controller
#	http://bos.asmhackers.net/docs/floppy/docs/floppy_tutorial.txt
#	http://bos.asmhackers.net/docs/floppy/docs/FLOPPY.TXT
#

.set FDC_PARAM_BASE,                   0x000fefc7
.set FDC_PARAM_STEPRATE_HEADUNLOAD,    (FDC_PARAM_BASE + 0x00)
.set FDC_PARAM_HEADLOAD_NDMA,          (FDC_PARAM_BASE + 0x01)
.set FDC_PARAM_MOTOR_DELAY_OFF,        (FDC_PARAM_BASE + 0x02)	#in clock ticks
.set FDC_PARAM_BYTES_PER_SECTOR,       (FDC_PARAM_BASE + 0x03)
.set FDC_PARAM_SECTORS_PER_TRACK,      (FDC_PARAM_BASE + 0x04)
.set FDC_PARAM_GAP_LENGTH,             (FDC_PARAM_BASE + 0x05)
.set FDC_PARAM_DATA_LENGTH,            (FDC_PARAM_BASE + 0x06)	#used only when bytes per sector = 0
.set FDC_PARAM_FORMAT_GAP_LENGTH,      (FDC_PARAM_BASE + 0x07)
.set FDC_PARAM_FILLER,                 (FDC_PARAM_BASE + 0x08)
.set FDC_PARAM_HEAD_SETTLE_TIME,       (FDC_PARAM_BASE + 0x09)	#in milliseconds
.set FDC_PARAM_MOTOR_START_TIME,       (FDC_PARAM_BASE + 0x0a)	#in 1/8 seconds

.set FDC_BUS_PRIMARY,                  0x3f0
.set FDC_BUS_SECONDARY,                0x370

.set FDC_PORT_STATUS_A,                0x0	# status register a (read-only)
.set FDC_PORT_STATUS_B,                0x1	# status register b (read-only)
.set FDC_PORT_DOR,                     0x2	# digital output register, which controls floppy drive motors, floppy drive selection, and resets.
.set FDC_PORT_TDR,                     0x3	# tape drive register
.set FDC_PORT_MSR,                     0x4	# main status register (read-only), which should be checked before each reading/writing with FIFO.
.set FDC_PORT_DSR,                     0x4	# data-rate select register (write-only)
.set FDC_PORT_DATA,                    0x5	# data fifo
.set FDC_PORT_DIR,                     0x7	# digital input register (read-only)
.set FDC_PORT_CCR,                     0x7	# configuration control register (write-only)

.set FDC_MASK_DOR_DSEL,                0x03	# select drive number for next access
.set FDC_MASK_DOR_RESET,               0x04	# 0: enter reset mode, 1: normal operation
.set FDC_MASK_DOR_IRQ,                 0x08	# 1: enable IRQ and DMA
.set FDC_MASK_DOR_MOTA,                0x10	# 1: turn on drive 0's motor
.set FDC_MASK_DOR_MOTB,                0x20	# 1: turn on drive 1's motor
.set FDC_MASK_DOR_MOTC,                0x40	# 1: turn on drive 2's motor
.set FDC_MASK_DOR_MOTD,                0x80	# 1: turn on drive 3's motor

.set FDC_MASK_MSR_ACTA,                0x01	# drive 0 is seeking/busy
.set FDC_MASK_MSR_ACTB,                0x02	# drive 1 is seeking/busy
.set FDC_MASK_MSR_ACTC,                0x04	# drive 2 is seeking/busy
.set FDC_MASK_MSR_ACTD,                0x08	# drive 3 is seeking/busy
.set FDC_MASK_MSR_CB,                  0x10	# FDC read/write command in progress
.set FDC_MASK_MSR_NDMA,                0x20	# FDC is in non-DMA mode
.set FDC_MASK_MSR_DIO,                 0x40	# I/O direction: 1 = FDC to CPU, 0 = CPU to FDC
.set FDC_MASK_MSR_RQM,                 0x80	# data reg ready for I/O to/from CPU (request for master)

.set FDC_CMD_FIX_DRIVE_DATA,           0x03
.set FDC_CMD_CHECK_DRIVE_STATUS,       0x04
.set FDC_CMD_CALIBRATE_DRIVE,          0x07
.set FDC_CMD_CHECK_INTERRUPT_STATUS,   0x08 
.set FDC_CMD_FORMAT_TRACK,             0x4d
.set FDC_CMD_READ_SECTOR,              0x66
.set FDC_CMD_READ_DELETE_SECTOR,       0xcc
.set FDC_CMD_READ_SECTOR_ID,           0x4a
.set FDC_CMD_READ_TRACK,               0x42
.set FDC_CMD_SEEK_TRACK,               0x0f
.set FDC_CMD_WRITE_SECTOR,             0xc5
.set FDC_CMD_WRITE_DELETE_SECTOR,      0xc9

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

#
# Input Regs:
#	None
#
# Output Regs:
#	None
#
.macro DISK_INB		port
	addw    $(\port), %dx
	inb     %dx, %al
	subw    $(\port), %dx
.endm


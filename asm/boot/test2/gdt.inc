#
# gdt.inc
#
#
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

.macro GDT_ENTRY_NULL
	GDT_ENTRY 0x00000000, 0x00000, 0x00, 0x00
.endm


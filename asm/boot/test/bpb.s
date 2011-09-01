# Reference of FAT12/FAT16/FAT32 formats:
# 	http://www.microsoft.com/whdc/system/platform/firmware/fatgen.mspx

.code16
.text
	jmp start                     # eb <offset16>
	nop                           # 90

	.ascii "-DENNIS-"             #OEM name
	.short 512                    #Bytes per sector
	.byte  1                      #Sectors per cluster
	.short 1                      #Reserved sector count
	.byte  2                      #Number of FATs
	.short 224                    #Root entry count
	.short 2880                   #Total sector (16-bit)
	.byte  0xf0                   #Media
	.short 9                      #FAT size (16-bit)
	.short 18                     #Sectors per track
	.short 2                      #Number of heads
	.int   0                      #Hidden sector
	.int   0                      #Total sector (32-bit)
	.byte  0                      #Drive number
	.byte  0                      #Reserved
	.byte  0x29                   #Boot signature         
	.int   0                      #Volume ID
	.ascii "BootableDsk"          #Volume label
	.ascii "FAT12   "             #File system type
start:

.code16
.text
	jmp start
	nop

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
	.word  0                      #Hidden sector
	.word  0                      #Total sector (32-bit)
	.byte  0                      #Drive number
	.byte  0                      #Reserved
	.byte  0x29                   #Boot signature         
	.word  0                      #Volume ID
	.ascii "BootableDsk"          #Volume label
	.ascii "FAT12   "             #File system type
start:

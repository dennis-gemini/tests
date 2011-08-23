
	org 07c00h

	jmp short start
	nop

	BS_OEMName		DB	'Dennis_C'
	BPB_BytesPerSector	DW	512
	BPB_SectorsPerCluster	DB	1
	BPB_ReservedSectorCount	DW	1
	BPB_NumberOfFAT		DB	2
	BPB_RootEntryCount	DW	224
	BPB_TotalSection16	DW	2880
	BPB_Media		DB	0xF0
	BPB_FATSize16		DW	9
	BPB_SectorPerTrack	DW	18
	BPB_NumberOfHeads	DW	2
	BPB_HiddenSectors	DD	0
	BPB_TotalSector32	DD	0
	BS_DriveNumber		DB	0
	BS_Reserved1		DB	0
	BS_BootSignature	DB	29h
	BS_VolumeID		DD	0
	BS_VolumeLabel		DB	'Dennis Chen'
	BS_FileSysType		DB	'FAT12   '

start:


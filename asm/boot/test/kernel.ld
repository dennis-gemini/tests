SECTIONS {
	. = 0x0500;

	.text : {
		kernel.o (.text)
		* (.text)
	}
}

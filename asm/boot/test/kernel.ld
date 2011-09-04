SECTIONS {
	. = 0x0000;

	.text : {
		kernel.o (.text)
		* (.text)
	}
}

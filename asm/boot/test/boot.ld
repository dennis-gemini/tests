SECTIONS {
	. = 0x7c00;

	.text : {
		.text_begin = .;
		bpb.o     (.text);
		boot.o    (.text);
		. = .text_begin + 508;
		trailer.o (.text);
	}
}

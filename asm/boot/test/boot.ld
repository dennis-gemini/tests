SECTIONS {
	. = 0x7c00;

	.text : {
		start = .;
		bpb.o     (.text);
		boot.o    (.text);
		. = start + 508;
		trailer.o (.text);
	}
}

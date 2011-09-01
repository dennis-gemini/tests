SECTIONS {
	. = 0x7c00;

	.text : {
		.begin = .;
		bpb.o     (.text);
		boot.o    (.text);
		print.o   (.text);
		. = .begin + 508;
		trailer.o (.text);
	}
}

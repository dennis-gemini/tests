SECTIONS {
	. = 0x7c00;

	.text : {
		.begin = .;
		bpb.o     (.text);
		boot.o    (.text);
		disk.o    (.text);
		console.o (.text);
		. = .begin + 510;
		bpb.o     (.signature);
	}
}

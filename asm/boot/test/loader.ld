SECTIONS {
	. = 0x0000;
	.text : {
		loader.o  (.text)
		console.o (.text)
	}
}

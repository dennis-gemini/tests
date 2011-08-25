CC=gcc
LD=ld
LDFILE=boot.ld
OBJCOPY=objcopy

all: boot.img

boot.o: boot.S
	$(CC) -c boot.S -o $@

boot.elf: boot.o
	$(LD) boot.o -o boot.elf -T$(LDFILE)
#	$(LD) boot.o -o boot.elf -Ttext 0x7c00

boot.bin: boot.elf
	$(OBJCOPY) -R .pdr -R .comment -R .note -S -O binary boot.elf boot.bin

boot.img: boot.bin
	dd if=boot.bin of=boot.img bs=512 count=1
	dd if=/dev/zero of=boot.img seek=1 bs=512 count=2879

clean:
	rm -f boot.o boot.elf boot.bin boot.img bochsout.txt

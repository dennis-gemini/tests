TARGET = boot.img

SRCS   = bpb.s \
         boot.s \
         trailer.s

OBJS   = $(SRCS:.s=.o)

all:	$(TARGET)

.SUFFIXES: .s
.s.o:
	$(AS) $^ -o $@

%.elf:
	$(LD) -o $@ -T $(@:.elf=.ld)

%.bin:
	objcopy -j .text -O binary $^ $@

%.img:
	dd if=$^ of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=$$((2880 - 1))

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.bin *.elf

-include *.d

bpb.o: bpb.s
boot.o: boot.s
trailer.o: trailer.s
boot.elf: bpb.o boot.o trailer.o
boot.bin: boot.elf
boot.img: boot.bin


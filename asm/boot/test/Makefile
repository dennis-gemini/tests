TARGET         = boot.img

SRCS           = bpb.s     \
                 boot.s    \
                 disk.s    \
                 console.s \
                 kernel.s

OBJS           = $(SRCS:.s=.o)

CROSS_COMPILE ?=
AS             = $(CROSS_COMPILE)as
LD             = $(CROSS_COMPILE)ld
OBJCOPY        = $(CROSS_COMPILE)objcopy

all:	$(TARGET)

.SUFFIXES: .s
.s.o:
	$(AS) -g $^ -o $@

%.elf:
	$(LD) -g -o $@ -T $(@:.elf=.ld) $^

%.bin:
	$(OBJCOPY) -j .text -O binary $^ $@

%.img:
	dd if=$< of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=$$((2880 - 1))
	mcopy -i $@ $(wordlist 2, $(words $^), $^) ::/

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.bin *.elf

-include *.d

bpb.o: bpb.s
boot.o: boot.s
disk.o: disk.s
console.o: console.s
boot.elf: bpb.o boot.o disk.o console.o
boot.bin: boot.elf
kernel.o: kernel.s
kernel.elf: kernel.o console.o
kernel.bin: kernel.elf
boot.img: boot.bin kernel.bin


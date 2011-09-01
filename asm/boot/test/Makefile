TARGET = boot.bin

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

$(TARGET):
	objcopy -j .text -O binary $^ $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.elf

-include *.d

bpb.o: bpb.s
boot.o: boot.s
trailer.o: trailer.s
boot.elf: bpb.o boot.o trailer.o
boot.bin: boot.elf


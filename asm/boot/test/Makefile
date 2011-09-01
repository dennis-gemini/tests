TARGET = boot.bin

SRCS   = header.s \
         boot.s \
         trailer.s

OBJS   = $(SRCS:.s=.o)

all:	$(TARGET)

.SUFFIXES: .s
.s.o:
	$(AS) $^ -o $@

%.elf:
	$(LD) -o $@ -T ld.script

$(TARGET):
	objcopy -j .text -O binary $^ $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.bin

-include *.d

header.o: header.s
boot.o: boot.s
trailer.o: trailer.s
boot.elf: header.o boot.o trailer.o
boot.bin: boot.elf


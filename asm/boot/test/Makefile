TARGET = boot.mbr

SRCS   = header.s \
         boot.s \
         trailer.s

OBJS   = $(SRCS:.s=.o)

all:	$(TARGET)

.SUFFIXES: .s
.s.o:
	$(AS) $^ -o $@

%.bin:
	$(LD) -o $@ $^ -T ld.script

$(TARGET):
	objcopy -j .text $^ $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) *.bin

-include *.d

header.o: header.s
boot.o: boot.s
trailer.o: trailer.s
boot.bin: header.o boot.o trailer.o
boot.mbr: boot.bin


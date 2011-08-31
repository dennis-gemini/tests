TARGET = boot.bin

SRCS   = header.s \
         boot.s \
         trailer.s

OBJS   = $(SRCS:.s=.o)

all:	$(TARGET)

.s.o:
	$(AS) $^ -o $@

$(TARGET):
	$(LD) -o $@ $< -T ld.script

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

-include *.d

header.o: header.s
boot.o: boot.s
trailer.o: trailer.s
mbr.bin: header.o boot.o trailer.o


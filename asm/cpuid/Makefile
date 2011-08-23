
BINS=test_cpuid
LDFLAGS=
CFLAGS=-MD -g

all:	$(BINS)

$(BINS):
	$(CC) -o $@ $^ $(LDFLAGS)

.s.o:
	$(CC) -o $@ -c $< $(CFLAGS)

.c.o:
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *.d $(BINS)

-include *.d

# dependencies
test_cpuid:	test_cpuid.o cpuid.o
test_cpuid.o:	test_cpuid.c
cpuid.o:	cpuid.s


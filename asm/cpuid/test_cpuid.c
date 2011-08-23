#include <stdio.h>

extern const char* cpuid();

int
main()
{
        printf("CPUID: %s\n", cpuid());
        return 0;
}

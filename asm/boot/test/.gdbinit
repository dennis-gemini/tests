target remote | exec qemu -gdb stdio -fda boot.img
symbol-file boot.elf loader.elf

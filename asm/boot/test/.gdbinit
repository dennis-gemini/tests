target remote | exec qemu -no-kvm -gdb stdio -fda boot.img
symbol-file boot.elf kernel.elf

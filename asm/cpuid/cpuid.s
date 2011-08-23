.section .bss
	.comm outbuf, 13

.section .text
.type cpuid, @function
.globl cpuid

cpuid:
	pushl %ebp
	movl  %esp, %ebp
	push  %ebx
	movl  $0, %eax
	cpuid
	movl  $outbuf, %edi
	movl  %ebx, (%edi)
	movl  %edx, 4 (%edi)
	movl  %ecx, 8 (%edi)
	movl  $outbuf, %eax
	popl  %ebx
	movl  %ebp, %esp
	popl  %ebp
	ret


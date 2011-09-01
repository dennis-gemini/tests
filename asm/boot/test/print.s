.code16
.text

#############################################
#
# print string
#
# input:
# 	%ds:%si = asciiz string
#
#############################################

.global print
print:
	lodsb
	orb   %al, %al
	jz    1f
	mov   $0x0e, %ah
	int   $0x10
	jmp   print
1:
	ret


#############################################
#
# show the rolling bar at cursor
#
#############################################

.global step_it
step_it:
	push    %ax
	push    %bx
	push    %cx
	push    %si
	mov     $(msg_step), %si
	mov     $(cur_step), %bx
	movb    (%si), %al
	mov     $4, %cx
1:
	cmpb    %al, (%bx)
	jz      2f
	inc     %bx
	dec     %cx
	jcxz    3f
	jmp     1b
2:
	sub     $(cur_step), %bx
	add     $(next_step), %bx
	movb    (%bx), %al
	movb    %al, (%si)
	call    print
3:
	pop     %si
	pop     %cx
	pop     %bx
	pop     %ax
	ret

cur_step:    .byte  '\\', '|', '/', '-'
next_step:   .byte  '|' , '/', '-', '\\'
msg_step:    .ascii "\\\b\0"


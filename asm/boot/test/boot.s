.code16
.text

.set root_dir_base, 0x0200	#ajacent to 0x7c00:0x01ff

#############################################
# main procedure
#############################################
.global start
start:
	/************************************
	 * Initialize segment and registers
	 ************************************/
	cli
	mov     %cx, %ax
	mov     %ax, %ds
	mov     %ax, %es

	xor     %ax, %ax
	mov     %ax, %ss
	mov     $0xffff, %sp
	sti

	call    load_root_dir

1:
	call    step_it
	jmp     1b

#############################################
# load root directory
#############################################

load_root_dir:
	mov     $(msg_loading), %si
	call    print

	mov     $(root_dir_base), %bx
	#call   load_root_sector

	ret

msg_loading: .ascii "Loading...\0"


.code16
.text

.set STACK_SEGMENT, 0x0000
.set STACK_BASE   , 0xffff
.set ROOT_DIR_BASE, 0x0200	#ajacent to 0x7c00:0x01ff

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

	mov     $STACK_SEGMENT, %ax
	mov     %ax, %ss
	mov     $STACK_BASE, %sp
	sti

	call    load_root_dir

1:
	call    step_it
	jmp     1b

#############################################
# load root directory
#############################################

load_root_dir:
	mov     $msg_loading, %si
	call    print

	mov     $ROOT_DIR_BASE, %bx
	#call   load_root_sector

	ret

msg_loading: .ascii "Loading...\0"


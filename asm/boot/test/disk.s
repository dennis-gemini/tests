.code16
.text

#############################################
# Address transition between CHS and LBA
############################################# 

#
# INPUT:
#    AX    = Sector number
#    CX    = Count
#    ES:BX = Buffer
#
# OUTPUT:
#    AX    = result
#    CX    = Count
#
read_sectors:
	/*
	 * LBA to CHS
	 *     Sector = (LBA % BPB_SecPerTrk) + 1
	 *       Head = (LBA / BPB_SecPerTrk) % BPB_NumHeads
	 *   Cylinder = (LBA / BPB_SecPerTrk) / BPB_NumHeads
	 */
	push    %cx

	xor     %dx, %dx
	divw    (BPB_SecPerTrk)
	inc     %dl                  
	mov     %dl, %cl             # cl = sector

	xor     %dx, %dx
	divw    (BPB_NumHeads)
	mov     %al, %ch             # ch = cylinder(track)
	mov     %dl, %dh             # dh = head

	movb    (BS_DrvNum), %dl    # dl = drive

	pop     %ax
	mov     $0x02, %ah           #INPUT  al: number of sectors, ch: track, cl: sector, dh: head, dl: drive, es:bx: buffer
	int     $0x13                #OUTPUT ah: return code, al: number of sectors read

	mov     %al, %cl
	mov     %ah, %al
	xor     %ah, %ah
	xor     %ch, %ch
	ret


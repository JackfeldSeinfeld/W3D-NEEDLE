; ID_VL.ASM

	IDEAL
	MODEL	MEDIUM,C

	INCLUDE	'ID_VL.EQU'

SCREENSEG	=	0a000h

	DATASEG

	EXTRN	TimeCount:WORD		; incremented every 70th of a second
	EXTRN	linewidth:WORD

starttime	dw	?

	CODESEG

;===========================================================================


;==============
;
; VL_WaitVBL			******** NEW *********
;
; Wait for the vertical retrace (returns before the actual vertical sync)
;
;==============

PROC	VL_WaitVBL  num:WORD
PUBLIC	VL_WaitVBL
@@wait:

	mov	dx,STATUS_REGISTER_1

	mov	cx,[num]
;
; wait for a display signal to make sure the raster isn't in the middle
; of a sync
;
@@waitnosync:
	in	al,dx
	test	al,8
	jnz	@@waitnosync


@@waitsync:
	in	al,dx
	test	al,8
	jz	@@waitsync

	loop	@@waitnosync

	ret

ENDP

;===========================================================================

;==============
;
; VL_SetScreen
;
;==============

PROC	VL_SetScreen  crtc:WORD, pel:WORD
PUBLIC	VL_SetScreen


	mov	cx,[timecount]		; if timecount goes up by two, the retrace
	add	cx,2				; period was missed (an interrupt covered it)

	mov	dx,STATUS_REGISTER_1

;
; wait for a display signal to make sure the raster isn't in the middle
; of a sync
;
@@waitdisplay:
	in	al,dx
	test	al,1	;1 = display is disabled (HBL / VBL)
	jnz	@@waitdisplay


@@loop:
	sti
	jmp	$+2
	cli

	cmp	[timecount],cx		; will only happen if an interrupt is
	jae	@@setcrtc			; straddling the entire retrace period

;
; when several succesive display not enableds occur,
; the bottom of the screen has been hit
;

	in	al,dx
	test	al,8
	jnz	@@waitdisplay
	test	al,1
	jz	@@loop

	in	al,dx
	test	al,8
	jnz	@@waitdisplay
	test	al,1
	jz	@@loop

	in	al,dx
	test	al,8
	jnz	@@waitdisplay
	test	al,1
	jz	@@loop

	in	al,dx
	test	al,8
	jnz	@@waitdisplay
	test	al,1
	jz	@@loop

	in	al,dx
	test	al,8
	jnz	@@waitdisplay
	test	al,1
	jz	@@loop


@@setcrtc:


;
; set CRTC start
;
; for some reason, my XT's EGA card doesn't like word outs to the CRTC
; index...
;
	mov	cx,[crtc]
	mov	dx,CRTC_INDEX
	mov	al,0ch		;start address high register
	out	dx,al
	inc	dx
	mov	al,ch
	out	dx,al
	dec	dx
	mov	al,0dh		;start address low register
	out	dx,al
	mov	al,cl
	inc	dx
	out	dx,al

;
; set horizontal panning
;
	mov	dx,ATR_INDEX
	mov	al,ATR_PELPAN or 20h
	out	dx,al
	jmp	$+2
	mov	al,[BYTE pel]		;pel pan value
	out	dx,al

	sti

	ret

ENDP


;===========================================================================


;============================================================================
;
; VL_ScreenToScreen
;
; Basic block copy routine.  Copies one block of screen memory to another,
; using write mode 1 (sets it and returns with write mode 0).  bufferofs is
; NOT accounted for.
;
;============================================================================

PROC	VL_ScreenToScreen	source:WORD, dest:WORD, wide:WORD, height:WORD
PUBLIC	VL_ScreenToScreen
USES	SI,DI

	pushf
	cli

	mov	dx,SC_INDEX
	mov	ax,SC_MAPMASK+15*256
	out	dx,ax
	mov	dx,GC_INDEX
	mov	al,GC_MODE
	out	dx,al
	inc	dx
	in	al,dx
	and	al,NOT 3
	or	al,1
	out	dx,al

	popf

	mov	bx,[linewidth]
	sub	bx,[wide]

	mov	ax,SCREENSEG
	mov	es,ax
	mov	ds,ax

	mov	si,[source]
	mov	di,[dest]				;start at same place in all planes
	mov	dx,[height]				;scan lines to draw
	mov	ax,[wide]

@@lineloop:
	mov	cx,ax
	rep	movsb
	add	si,bx
	add	di,bx

	dec	dx
	jnz	@@lineloop

	mov	dx,GC_INDEX+1
	in	al,dx
	and	al,NOT 3
	out	dx,al

	mov	ax,ss
	mov	ds,ax					;restore turbo's data segment

	ret

ENDP


;===========================================================================

   PROC   VL_VideoID
   PUBLIC VL_VideoID

	; Get display combination code.
	; See RBIL INTERRUP.A - V-101A00.

	mov	ax,1A00h
	int	10h

	cmp	al,1Ah	; AL = 1Ah if function is supported.
	jne	@@novga

   ; BL = Active display code.
	; BH = Alternate display code.

	cmp	bl,8		; Is color analog VGA active?
	jne	@@novga

	; Make sure an ATI EGA Wonder isnt lying to us.
	; See RBIL INTERRUP.A - V-101A00 and V-101C.

	mov	ax,1C00h	; Video save/restore state function.
	mov	cx,2		 ; Only video hardware (CX=0) is supported on EGA Wonder.
	int	10h		  ; So lets check for color registers and DAC state. (CX=2)

	cmp	al,1Ch	  ; AL = 1Ch if function is supported.
	jne	@@novga

	; Yes, we have an active color analog VGA!

	mov	ax,5	 ; Original Wolf sources expect (5) to indicate VGA support.
	jmp	@@done

@@novga:

	xor	ax,ax	; Indicate failure, no VGA was detected!

@@done:

	ret

   ENDP

END
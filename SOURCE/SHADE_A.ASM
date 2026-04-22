; WOLFHACK.ASM

.386
IDEAL
MODEL FLAT

SCREENSEG  = 0a000h
FRACBITS   = 16
TILESHIFT  = 16
TILEGLOBAL = 65536
MAPSHIFT   = 6
FLATSPLANE = 2

USE_SHADE = 1

DATASEG

IF USE_SHADE
EXTRN _shadetable:BYTE
ENDIF
EXTRN _PMPages:DWORD
EXTRN _mapsegs:DWORD

EXTRN _mr_count:DWORD
EXTRN _mr_xstep:DWORD
EXTRN _mr_ystep:DWORD
EXTRN _mr_dest:DWORD
IF USE_SHADE
EXTRN _mr_shade:DWORD
ENDIF

xfrac dd ?
yfrac dd ?

CODESEG

;
; EBX:  toprow to bottomrow delta
; ESI:  xfrac
; EDI:  yfrac
;
PROC MapRow_
PUBLIC MapRow_

    ;
    ; patch in the row offset
    ;
    mov eax,OFFSET patch2+2
    mov [eax],ebx

    sub ecx,ecx

countloop:

    ;
    ; initialize ECX (xpix) to largest possible value
    ;
    ; ECX will be 0 after drawing the first span,
    ; so DEC will make it wrap to 0xFFFFFFFF
    ;
    dec ecx

    ;
    ; Find which frac value will step into a new tile
    ; first. The span length will be the smaller of the
    ; resulting xpix/ypix pair
    ;
    ; If one of the steps is 0, its frac will never step
    ; into another tile, so the other pix value will
    ; always be used
    ;

    ;
    ; calculate xpartial
    ;
    mov ebx,[_mr_xstep]
    or  ebx,ebx
    jz  xstepzero

    mov eax,esi
    js  xstepnegative       ; flags unchanged after MOV
    xor eax,TILEGLOBAL - 1
    inc eax
    neg ebx                 ; counter NEG so we don't have to JMP

xstepnegative:
    neg ebx

    and eax,TILEGLOBAL - 1  ; mask off high bits

    ;
    ; EAX: xpartial
    ; EBX: absstep
    ; ECX: xpix
    ;
    mov ecx,1               ; at least 1 pixel to draw

    cmp eax,ebx
    jbe usexpix

    sub edx,edx
    div ebx

    neg edx
    adc eax,0
    mov ecx,eax

    ;
    ; calculate ypartial
    ;
xstepzero:
    mov ebx,[_mr_ystep]
    or  ebx,ebx
    jz  usexpix

    mov eax,edi
    js  ystepnegative       ; flags unchanged after MOV
    xor eax,TILEGLOBAL - 1
    inc eax
    neg ebx                 ; counter NEG so we don't have to JMP

ystepnegative:
    neg ebx

    and eax,TILEGLOBAL - 1  ; mask off high bits

    ;
    ; EAX: ypartial
    ; EBX: absstep
    ; EDX: ypix
    ;
    mov edx,1               ; at least 1 pixel to draw

    cmp eax,ebx
    jbe useypix

    sub edx,edx
    div ebx

    neg edx
    adc eax,0
    mov edx,eax

    ;
    ; smaller value is the span length
    ;
    cmp ecx,edx
    jb  usexpix
useypix:
    xchg ecx,edx    ; ypix is smaller

usexpix:
    mov eax,[_mr_count]
    cmp eax,ecx
    jge countleft
    mov ecx,eax     ; last span, use whatever's left of the row

countleft:
    sub eax,ecx
    mov [_mr_count],eax

    ;
    ; increment & save off fracs and pack x / y step values
    ;
    ; high word of ECX must be 0 for the 32 bit multiply
    ;
    mov eax,[_mr_ystep]
    mov ebx,eax
    shl ebx,FRACBITS

    imul ecx
    add eax,edi
    mov [yfrac],eax     ; yfrac += mr_ystep * spanlength

    mov eax,[_mr_xstep]
    add ebx,eax

    imul ecx
    add eax,esi
    mov [xfrac],eax     ; xfrac += mr_xstep * spanlength

    mov eax,OFFSET patch1+2
    mov [eax],ebx       ; patch in the step value

    ;
    ; pack x / y frac values
    ;
    mov ebp,edi
    shl ebp,FRACBITS
    add ebp,esi

    ;
    ; get spot in map plane
    ;
    shr esi,TILESHIFT
    shr edi,TILESHIFT - MAPSHIFT
    and edi,NOT (1 SHL MAPSHIFT) - 1
    add edi,esi

    mov ebx,[_mapsegs+(FLATSPLANE * 4)]     ; plane start index in BYTES
    mov si,[ebx+edi*2]                      ; WORD lookup

    ;
    ; get the source pages
    ;
    ; each page starts on a 4096 byte boundary
    ;
    mov eax,[_PMPages]
    mov edx,esi
    and edx,NOT 0ffh    ; wipe out the floor bits
    shl edx,4           ; index into PMPages
    add edx,eax
    and esi,0ffh        ; wipe out the ceiling bits
    shl esi,12          ; index into PMPages
    add eax,esi

    mov edi,[_mr_dest]
IF USE_SHADE
    mov ebx,[_mr_shade]
ENDIF

;
; draw this span
;
; EAX:  floor source
; EBX:  color translation table & pixel values
; ECX:  loop counter
; EDX:  ceiling source
; EBP:  packed x / y fractional values
; ESI:  scratch offset
; EDI:  dest pointer
;
@@pixloop:
    shld esi,ebp,22         ; shift y units in
    shld esi,ebp,6          ; shift x units in
    and esi,00fffh          ; mask off extra high bits

patch1:
    add ebp,012345678h      ; frac += fracstep

    mov bl,[edx+esi]        ; get ceiling pixel
IF USE_SHADE
    mov bl,[ebx]            ; color translate it
ENDIF
    mov [edi],bl            ; write ceiling pixel

    mov bl,[eax+esi]        ; get floor pixel
IF USE_SHADE
    mov bl,[ebx]            ; color translate it
ENDIF
patch2:
    mov [edi+012345678h],bl ; write floor pixel
    inc edi
    dec ecx
    jnz @@pixloop

    mov [_mr_dest],edi
    mov esi,[xfrac]
    mov edi,[yfrac]

    cmp [_mr_count],0
    jnz countloop

    ret
ENDP

END
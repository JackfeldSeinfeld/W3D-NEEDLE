; WOLFHACK.ASM

.386C
IDEAL
MODEL MEDIUM,C

SCREENSEG  = 0a000h
FRACBITS   = 16
TILESHIFT  = 16
TILEGLOBAL = 65536
MAPSHIFT   = 6
FLATSPLANE = 2
SHADEPLANE = 3

USE_SHADE = 1


EXTRN PM_GetFlatTexturePage:FAR

EXTRN shadetable:BYTE

DATASEG

EXTRN mapsegs:WORD

EXTRN mr_count:WORD
EXTRN mr_xstep:DWORD
EXTRN mr_ystep:DWORD
EXTRN mr_xfrac:DWORD
EXTRN mr_yfrac:DWORD
EXTRN mr_rowofs:WORD
EXTRN mr_dest:WORD
EXTRN mr_shade:WORD

lastceilingpage dw ?
lastfloorpage dw ?

xfrac dd ?
yfrac dd ?

CODESEG

PROC MapRow
PUBLIC MapRow

    push si
    push di
    push bp

    ;
    ; initialize variables
    ;
    mov ax,0ffffh
    mov [lastceilingpage],ax
    mov [lastfloorpage],ax

    mov esi,[mr_xfrac]
    mov edi,[mr_yfrac]

countloop:
    ;
    ; get spot in map plane
    ;
    ; AX: tilex
    ; BX: tiley
    ; BP: spot
    ;
    shld eax,esi,TILESHIFT
    shld ebx,edi,TILESHIFT

    shl bx,MAPSHIFT
    add bx,ax
    shl bx,1                            ; WORD lookup
    mov ax,[mapsegs+(FLATSPLANE * 2)]   ; plane start index in BYTES
    mov es,ax
    mov bp,[es:bx]

    ;
    ; get the source segments
    ;
    ; Borland will preserve DI, SI, BP, FS & GS between
    ; function calls
    ;
    ; PM_GetFlatTexturePage takes the pagenum in BX and
    ; returns a _seg pointer in AX
    ;
    mov bx,bp
    shr bx,8        ; ceiling is in the high byte

    cmp [lastceilingpage],bx
    je  gotceiling
    mov [lastceilingpage],bx
    call PM_GetFlatTexturePage
    mov fs,ax

gotceiling:
    mov bx,bp
    sub bh,bh       ; floor is in the low byte

    cmp [lastfloorpage],bx
    je  gotfloor
    mov [lastfloorpage],bx
    call PM_GetFlatTexturePage
    mov gs,ax

gotfloor:

    ;
    ; load ES & clear high word of ECX
    ;
    mov ecx,SCREENSEG
    mov es,cx

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
    mov ebx,[mr_xstep]
    or  ebx,ebx
    jz  xstepzero

    movzx eax,si
    js  xstepnegative       ; flags unchanged after MOV
    xor ax,TILEGLOBAL - 1
    inc ax
    neg ebx                 ; counter NEG so we don't have to JMP

xstepnegative:
    neg ebx

    ;
    ; EAX: xpartial
    ; EBX: absstep
    ; CX:  xpix
    ;
    mov cx,1                ; at least 1 pixel to draw

    cmp eax,ebx
    jbe usexpix

    sub edx,edx
    div ebx

    neg edx
    adc ax,0
    mov cx,ax

    ;
    ; calculate ypartial
    ;
xstepzero:
    mov ebx,[mr_ystep]
    or  ebx,ebx
    jz  usexpix

    movzx eax,di
    js  ystepnegative       ; flags unchanged after MOV
    xor ax,TILEGLOBAL - 1
    inc ax
    neg ebx                 ; counter NEG so we don't have to JMP

ystepnegative:
    neg ebx

    ;
    ; EAX: ypartial
    ; EBX: absstep
    ; DX:  ypix
    ;
    mov dx,1                ; at least 1 pixel to draw

    cmp eax,ebx
    jbe useypix

    sub edx,edx
    div ebx

    neg edx
    adc ax,0
    mov dx,ax

    ;
    ; smaller value is the span length
    ;
    cmp cx,dx
    jb  usexpix
useypix:
    xchg cx,dx      ; ypix is smaller

usexpix:
    mov ax,[mr_count]
    cmp ax,cx
    jge countleft
    mov cx,ax       ; last span, use whatever's left of the row

countleft:
    sub ax,cx
    mov [mr_count],ax

    ;
    ; increment & save off fracs and pack x / y step values
    ;
    ; high word of ECX must be 0 for the 32 bit multiply
    ;
    mov eax,[mr_ystep]
    shrd ebx,eax,FRACBITS

    imul ecx
    add eax,edi
    mov [yfrac],eax     ; yfrac += mr_ystep * spanlength

    mov eax,[mr_xstep]
    add ebx,eax

    imul ecx
    add eax,esi
    mov [xfrac],eax     ; xfrac += mr_xstep * spanlength

    mov edx,ebx

    ;
    ; pack x / y frac values
    ;
    shrd eax,edi,FRACBITS
    add eax,esi

    mov di,[mr_dest]
    mov bp,[mr_rowofs]
IF USE_SHADE
    mov bx,[mr_shade]
    mov si,SEG shadetable
    mov ds,si
ENDIF

;
; draw this span
;
; eax   packed x / y fractional values
; bx    pixel values
; cx    loop counter
; edx   packed x / y step values
; bp    toprow to bottomrow delta
; esi   scratch offset
; fs:si ceiling source
; gs:si floor source
; es:di dest
; ds:bx color translation table
;
@@pixloop:
    shld esi,eax,22         ; shift y units in
    shld esi,eax,6          ; shift x units in
    and si,00fffh           ; mask off extra high bits

    add eax,edx             ; frac += fracstep

    mov bl,[fs:si]          ; get ceiling pixel
IF USE_SHADE
    mov bl,[bx]             ; color translate it
ENDIF
    mov [es:di],bl          ; write ceiling pixel

    mov bl,[gs:si]          ; get floor pixel
IF USE_SHADE
    mov bl,[bx]             ; color translate it
ENDIF
    mov [es:di+bp],bl       ; write floor pixel
    inc di
    dec cx
    jnz @@pixloop
IF USE_SHADE
    mov ax,ss
    mov ds,ax
ENDIF
    mov [mr_dest],di
    mov esi,[xfrac]
    mov edi,[yfrac]

    cmp [mr_count],0
    jnz countloop

    pop bp
    pop di
    pop si

    ret
ENDP

END
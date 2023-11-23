; ***************************************************************************************
; ***************************************************************************************
;
;                         Write Character to Display
;
; ***************************************************************************************
; ***************************************************************************************

        .if     NEO!=0
Le3d5:  pha
        and     #$7F
        jsr     $FFF1
        pla
        rts
        nop
        .else
; Send character to display. Char is in A.
Le3d5:  BIT     DSP          ; See if display ready
        BMI     Le3d5        ; Loop if not
        STA     DSP          ; Write display data
        RTS                  ; and return
        .endif



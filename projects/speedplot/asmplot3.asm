*=$0

lp:

        stx $ff04
        sty $ff06         
        lda #5
        sta $ff01
        sta $ff00

        dex 
        bne lp 
        dey 
        bne lp 
        rts 
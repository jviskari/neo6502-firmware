; ************************************************************************************************
; ************************************************************************************************
;
;		Name:		string.asm
;		Purpose:	Unary string 'function' (e.g. [[EXPRING]])
;		Created:	1st December 2023
;		Reviewed:   No
;		Author:		Paul Robson (paul@robsons.org.uk)
;
; ************************************************************************************************
; ************************************************************************************************

; ************************************************************************************************
;
;							Inline immutable string
;
; ************************************************************************************************

		.section code	

EXPUnaryInlineString: ;; [!!str]
		clc 								; physical address -> TOS
		tya 								
		adc 	codePtr
		sta 	XSNumber0,x
		lda 	codePtr+1
		adc 	#0
		sta 	XSNumber1,x
		stz 	XSNumber2,x 				; fill in the rest & type
		stz 	XSNumber3,x
		lda 	#XS_ISSTRING 				; make it a string.
		sta 	XSControl,x
		;
		tya	 								; skip over it.
		sec
		adc 	(codePtr),y
		tay
		;
		rts

		.send code
		


				
; ************************************************************************************************
;
;									Changes and Updates
;
; ************************************************************************************************
;
;		Date			Notes
;		==== 			=====
;
; ************************************************************************************************


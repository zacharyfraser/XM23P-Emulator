;
; Loader Test 04 Overwriting Data To Instruction and Data Memory
;

	Data
	org	#800

	Word	#EFBE
	org #800
	Word	#FACE
;
;
Start
	Code
	org #800

	Movlz	#20,R1	; R1 <- #0020

	org #800
	Add		#0,R1	; R1 += #0000
end	Start



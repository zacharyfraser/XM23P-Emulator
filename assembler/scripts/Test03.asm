;
; Loader Test 03 Loading Data To Instruction and Data Memory
;

	Data
	org	#800
;
; Initialize registers
;
	Word	#EFBE
;
;
Start
	Code
	org #800

	Movlz	#20,R1	; R1 <- #0020
end	Start



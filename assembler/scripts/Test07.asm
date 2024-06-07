;
; Loader Test 07 MovX Instruction Decoding
;

        org #100
Start

        movl    #ffbe,R0
        movh    #efff,R1
        movlz   #fffa,R2
        movls   #00ce,R3
        end Start

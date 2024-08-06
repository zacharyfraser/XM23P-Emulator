;Test 30 - Bubble
;
;If a bubble is correctly inserted, R3 should contain #FACE
;Otherwise, R3 will contain #BEEF

        Data
        ORG #1000
ADDR    word #0200
        Code
        ORG #0100
Start
        MOVL    #FACE,R0        ;Initialize R0 to FACE
        MOVH    #FACE,R0
        MOVL    #BEEF,R1        ;Initialize R1 to BEEF
        MOVH    #BEEF,R1
        MOVL    #1000,R2        ;Initialize R2 to 1000
        MOVH    #1000,R2
        LD.W    R2,R7           ;PC <- #0200
;Bubble Expected
        MOV     R1,R0           ;R0 <- #BEEF : Should be ignored

        org #200
        MOV     R0,R3           ;R3 <- #FACE
        
        END Start

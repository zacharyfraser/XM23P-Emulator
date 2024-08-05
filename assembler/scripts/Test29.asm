;Test 29 - Load
        org #100
Start
        MOVL    #1000,R0        ;Initialize R0 with #1000
        MOVH    #1000,R0
        MOVL    #FACE,R1        ;Initialize R1 with #FACE
        MOVH    #FACE,R1
        ST.B    R1,+R0          ;#1001 <- #00CE, R0 <- #1001
        STR.W   R1,R0,#10       ;#1011 <- #FACE
        LD.W    R0-,R2          ;R2 <- #00CE, R0 <- #0FFF
        LDR.B   R0,#12,R3       ;R3 <- #00CE
        END Start

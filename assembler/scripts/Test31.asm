;Test 31 - Negative Offsets

        org #100
Start
        MOVL    #FACE,R0        ;Initialize R0 to FACE
        MOVH    #FACE,R0
        MOVL    #1000,R2        ;Initialize R2 to 1000
        MOVH    #1000,R2
        STR.W   R0,R2,$-64      ;#0FC0 <- #FACE
        LDR.W   R2,$-64,R3      ;R3 <- #FACE
        END Start

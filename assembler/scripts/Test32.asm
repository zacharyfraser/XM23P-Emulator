;Test 32 - Execute Pipeline

        org #100
Start
        ADD     R0,R1           ;No E1
        STR.W   R0,R2,$-64      ;E1
        LDR.W   R2,$-64,R3      ;E1
        SUB     R1,R2           ;No E1
        END Start

;Test 32 - Execute Pipeline

        org #100
Start
        SETCC   VNZ
        CEX     GE,$3,$1
        BNE     SUM
        ADD     #1,R0
        SUB     #1,R1
        ADD     #1,R2

        org #20
SUM
        ADD     #1,R3
        ADD     #1,R3
        ADD     #1,R3
        ADD     #1,R3
        ADD     #1,R3
        END Start

;Test 32 - Execute Pipeline

        org #100
Start
        MOVLS   #FF,R0
        SETCC   VN
        CEX     GE,$2,$1
        ADD     #1,R0
        SUB     #1,R0
        END Start

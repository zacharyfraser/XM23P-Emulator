;Test 38 - True Condition

        org #100
Start
        MOVLS   #FF,R0
        MOVLS   #FF,R1
        SETCC   VN
        CEX     GE,$2,$2
        ADD     #1,R0
        ADD     #1,R1
        SUB     #1,R0
        SUB     #1,R1

        END Start

;Test 38 - True Condition

        org #100
Start
        CMP     $1,R0
        CEX     EQ,$0,$0
        ADD     #1,R0
        CMP     $1,R0
        CEX     EQ,$0,$0
        ADD     #1,R1

        END Start

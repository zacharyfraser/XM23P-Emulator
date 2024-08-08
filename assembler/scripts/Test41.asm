;Test 41 - CEX Branch

        org #100
Start
        CMP     $1,R0
        CEX     NE,$4,$1
        BRA     Branch
        ADD     #1,R0
        CMP     $1,R0
        CEX     EQ,$0,$0
        ADD     #1,R1

        org #80
Branch
        ADD     $1,R2
        ADD     $1,R2
        ADD     $1,R2
        ADD     $1,R2
        ADD     $1,R2
        END Start

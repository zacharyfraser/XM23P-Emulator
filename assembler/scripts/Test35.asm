;Test 35 - Bubble

        org #100
Start
        CMP     $1,R0
        BEQ     Branch_1
        CMP     $0,R0
        BEQ     Branch_2

        org #110
Branch_2
        MOV      R0,R0

        org #70
        MOV     R0,R2           ;Should Not Execute
Branch_1
        MOVL    #FACE,R3        ;Should Not Execute
        MOVH    #FACE,R3        ;Should Not Execute

        END Start

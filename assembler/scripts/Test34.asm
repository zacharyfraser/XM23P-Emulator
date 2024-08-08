;Test 33 - Execute Pipeline

        org #100
Start
        MOVL    #BEEF,R0
        MOVH    #BEEF,R0
        CMP     R0,R0
        BNE     Branch          ;Should Not Branch
        MOV     R0,R1           ;Should Execute

        org #70
        MOV     R0,R2           ;Should Not Execute
Branch
        MOVL    #FACE,R3        ;Should Not Execute
        MOVH    #FACE,R3        ;Should Not Execute

        END Start

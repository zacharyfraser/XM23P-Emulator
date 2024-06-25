;Test 18 - Addition Instructions
        org #100
Start
        movl #abcd,R0
        movh #abcd,R0
        add  R0,R1      ;R1 <- abcd, Negative = 1
        add  $-1,R1     ;R1 <- abcc, Negative = 1, Carry = 1
        subc $-1,R1     ;R1 <- abcd, Negative = 1
        end Start
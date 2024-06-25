;Test 21 - Sign Extend/Swap Bytes
        org #100
Start
        movl  #abcd,R0
        movh  #abcd,R0
        movlz $-128,R1  ;R1 <- 0080
        sxt   R1        ;R1 <- ff80, Negative = 1
        swpb  R0        ;R0 <- cdab, Negative = 1
        end Start
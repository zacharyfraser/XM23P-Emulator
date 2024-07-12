;Test 19 - Decimal Addition Instructions
        org #100
Start
        movl  #9999,R0
        movh  #9999,R0
        movlz $1,R2
        dadd  R0,R1    ;R1 <- 9999
        dadd  R2,R1    ;R1 <- 0000, Carry = 1, Zero = 1
        end Start
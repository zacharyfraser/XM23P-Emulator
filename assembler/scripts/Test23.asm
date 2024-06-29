;Test 21 - Sign Extend/Swap Bytes
        org #100
Start
        movl    #8000,R0        ;R0 <- XX00
        movh    #8000,R0        ;R0 <- 8000
        mov     R0,R1           ;R1 <- 8000
        add     R0,R1           ;R1 <- 0000, Carry, Overflow, Zero
        sub     $1,R1          ;R1 <- ffff, Negative
        end Start

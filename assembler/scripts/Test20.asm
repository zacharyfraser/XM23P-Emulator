;Test 20 - Swap Instructions
        org #100
Start
        movl  #9999,R0
        movh  #9999,R0
        movlz $1,R1
        swap  R0,R1     ;R0 <- $1, R1 <- 9999
        mov   R1,R2     ;R2 <- 9999
        end Start
        DATA
        org     #100
DVal    word    $10
        word    $5
        word    $64
        word    $-1     ; End-of-list indicator

        CODE
        org     #200
SumMain
; R0 <- address of DVal
        movl    DVal,R0
        movh    DVal,R0
;
        movlz   #0,R1   ; Sum <- 0
;
Loop    ld      R0+,R2  ; R2 <- mem[R0]
        cmp     #0,R2
        bn Done         ; If negative, exit
        add     R2,R1   ; Add value to sum
        bra     Loop    ; Repeat for next value
; End of loop - Result in R1
Done
;        bra Done
;
        end SumMain



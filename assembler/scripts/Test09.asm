;
; Loader Test 09 Unimplemented Instructions
;

        org #100
Start
;Branch Instructions

Branch  bl      CPU
        beq     CPU
        bz      CPU
        bne     CPU
        bnz     CPU
        bc      CPU
        bhs     CPU
        bnc     CPU
        blo     CPU
        bn      CPU
        bge     CPU
        blt     CPU
        bra     CPU

CPU     setpri  $1
        svc     $2
        setcc   VSNZC
        clrcc   VSNZC
        cex     ne,$1,$0
LDST    ld      R0,R1
        st      R0,R1
        ldr     R0,R1,R2
        str     R0,R1,$1
        
        end Start
        

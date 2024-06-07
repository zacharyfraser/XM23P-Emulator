;
; Loader Test 08 Register Instruction Decoding
;

        org #100
Start

        add     R0,R1
        add.b   $2,R3
        addc    R4,R5
        addc.b  $32,R7
        sub     R0,R1
        sub.b   $2,R3
        subc    R4,R5
        subc.b  $32,R7
        dadd    R0,R1
        dadd.b  $2,R3
        cmp     R4,R5
        cmp.b   $32,R7
        xor     R0,R1
        xor.b   $2,R3
        and     R4,R5
        and.b   $32,R7
        or      R0,R1
        or.b    $2,R3
        bit     R4,R5
        bit.b   $32,R7
        bic     R0,R1
        bic.b   $2,R3
        bis     R4,R5
        bis.b   $32,R7
        mov     R0,R1
        mov.b   R2,R3
        swap    R4,R5
        sra     R6
        sra.b   R7
        rrc     R0
        rrc.b   R1
        swpb    R2
        sxt     R3
        end Start
